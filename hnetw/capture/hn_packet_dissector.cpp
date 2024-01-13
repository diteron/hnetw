#include <stdafx.h>
#include "hn_packet_dissector.h"

#include <packet/proto_headers.h>
#include <packet/packets/hn_packet.h>
#include <packet/hn_packet_factory.h>
#include <ui/qtmodels/hn_packet_list_row.h>


HnPacketDissector::HnPacketDissector()
{}

HnPacketDissector::~HnPacketDissector()
{}

void HnPacketDissector::setPacketListModel(HnPacketListModel* pListModel)
{
    packetListModel_ = pListModel;
}

void HnPacketDissector::setCaptureFile(HnCaptureFile* capFile)
{
    captureFile_ = capFile;
}

void HnPacketDissector::writePacket(raw_packet* rawPacket)
{
    captureFile_->writeRawPacket(rawPacket);
}

bool HnPacketDissector::startDissection()
{
    if (!captureFile_) {
        return false;
    }

    isCapturePermitted_ = true;

    std::thread dissection(&HnPacketDissector::dissectPackets, this);
    dissection.detach();

    return true;
}

void HnPacketDissector::stopDissection()
{
    isCapturePermitted_ = false;
}

void HnPacketDissector::dissectPackets()
{
    raw_packet* rawPacket = nullptr;
    ipv4_hdr* ipHeader = nullptr;
    HnPacket* capturedPacket = nullptr;
    uint8_t* rawDataCopy = nullptr;
    HnPacketListRow* newRow = nullptr;

    int id = -1;
    std::clock_t currentPacketTime = -1;
    uint8_t* rawData = nullptr;
    int readBytesCnt = 0;
    int writtenBytesCnt = 0;
    long packetOffsetBuff = 0;

    while (isCapturePermitted_.load()) {
        rawPacket = captureFile_->getNextPacketToDissect(&packetOffsetBuff);
        if (rawPacket == nullptr) continue;
        currentPacketOffset_ = packetOffsetBuff;

        id = rawPacket->id;
        currentPacketTime = rawPacket->time;
        rawData = rawPacket->data;
        readBytesCnt = rawPacket->length;

        ipHeader = reinterpret_cast<ipv4_hdr*>(rawData);

        capturedPacket = HnPacketFactory::instance()->buildPacket(ipHeader->protocol, id);
        if (capturedPacket == nullptr) {
            delete rawPacket;
            continue;
        }

        rawDataCopy = new uint8_t[readBytesCnt];
        std::memcpy(rawDataCopy, rawData, readBytesCnt);

        capturedPacket->setPacketData(rawDataCopy, readBytesCnt);
        capturedPacket->setArrivalTime(currentPacketTime);
        ++dissectedPacketsCnt_;

        if (isCapturePermitted_.load()) {
            newRow = new HnPacketListRow(capturedPacket, currentPacketOffset_.load());
            packetListModel_->appendRow(newRow);
        }

        delete capturedPacket;
        delete rawPacket;
    }
}

void HnPacketDissector::reset()
{
    currentPacketOffset_ = 0;
}
