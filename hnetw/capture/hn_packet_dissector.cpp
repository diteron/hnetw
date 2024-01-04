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

void HnPacketDissector::enqueuePacket(raw_packet rawPacket)
{
    packetQueue_.enqueue(rawPacket);
}

void HnPacketDissector::setCaptureInProgress(bool inProgress)
{
    captureInProgress_ = inProgress;
}

void HnPacketDissector::startDissection()
{
    captureInProgress_ = true;
    std::thread dissection(&HnPacketDissector::dissectPackets, this);
    dissection.detach();
}

void HnPacketDissector::stopDissection()
{
    captureInProgress_ = false;
}

void HnPacketDissector::dissectPackets()
{
    while (captureInProgress_.load()) {
        raw_packet rawPacket = packetQueue_.dequeue();

        int id = rawPacket.id;
        std::clock_t currentPacketTime = rawPacket.time;
        pBuffer buffer = rawPacket.buffer;
        int bytesRead = rawPacket.length;

        ipv4_hdr* ipHeader = reinterpret_cast<ipv4_hdr*>(buffer);
        uint8_t* rawData = new uint8_t[bytesRead];      // Cleanup is in the captured packet
        std::memcpy(rawData, buffer, bytesRead);

        HnPacket* capturedPacket = HnPacketFactory::instance()->buildPacket(ipHeader->protocol, id);
        delete[] buffer;
        if (capturedPacket == nullptr) {
            delete[] rawData;
            continue;
        }

        capturedPacket->setPacketData(rawData, bytesRead);
        capturedPacket->setArrivalTime(currentPacketTime);
        ++dissectedPacketsCnt_;

        if (captureInProgress_.load()) {
            HnPacketListRow* newRow = new HnPacketListRow(capturedPacket);
            packetListModel_->appendRow(newRow);
        }
    }

    packetQueue_.clear();
}
