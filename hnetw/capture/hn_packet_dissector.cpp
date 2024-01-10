#include <stdafx.h>
#include "hn_packet_dissector.h"

#include <packet/proto_headers.h>
#include <packet/packets/hn_packet.h>
#include <packet/hn_packet_factory.h>
#include <ui/qtmodels/hn_packet_list_row.h>


HnPacketDissector::HnPacketDissector() : packetQueue_()
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

void HnPacketDissector::enqueuePacket(raw_packet rawPacket)
{
    packetQueue_.enqueue(rawPacket);
}

void HnPacketDissector::startDissection()
{
    isCapturePermitted_ = true;

    if (!captureFile_) {
        return;
    }

    std::thread dissection(&HnPacketDissector::dissectPackets, this);
    dissection.detach();
}

void HnPacketDissector::stopDissection()
{
    isCapturePermitted_ = false;
}

void HnPacketDissector::dissectPackets()
{
    while (isCapturePermitted_.load()) {
        raw_packet rawPacket = packetQueue_.dequeue();

        int id = rawPacket.id;
        std::clock_t currentPacketTime = rawPacket.time;
        pBuffer buffer = rawPacket.buffer;
        int readBytesCnt = rawPacket.length;

        ipv4_hdr* ipHeader = reinterpret_cast<ipv4_hdr*>(buffer);
        uint8_t* rawData = new uint8_t[readBytesCnt];      // Cleanup is in the captured packet
        std::memcpy(rawData, buffer, readBytesCnt);

        HnPacket* capturedPacket = HnPacketFactory::instance()->buildPacket(ipHeader->protocol, id);
        delete[] buffer;
        if (capturedPacket == nullptr) {
            delete[] rawData;
            continue;
        }

        capturedPacket->setPacketData(rawData, readBytesCnt);
        capturedPacket->setArrivalTime(currentPacketTime);
        ++dissectedPacketsCnt_;

        if (isCapturePermitted_.load()) {
            captureFile_->writePacket(capturedPacket->rawData(), capturedPacket->length());
            HnPacketListRow* newRow = new HnPacketListRow(capturedPacket, currentPacketOffset_);
            currentPacketOffset_ += readBytesCnt;
            packetListModel_->appendRow(newRow);
        }

        delete capturedPacket;
    }

    packetQueue_.clear();
}

void HnPacketDissector::reset()
{
    currentPacketOffset_ = 0;
    packetQueue_.clear();
}
