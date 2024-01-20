#pragma once

#include <cstdio>
#include <stdint.h>
#include <packet/packets/hn_packet.h>

typedef char* pBuffer;
struct raw_packet {
    int id;
    std::clock_t time;
    uint8_t* data;
    size_t length;

    ~raw_packet() { delete[] data; }
};

class HnCaptureFile {
public:
    HnCaptureFile();
    ~HnCaptureFile();

    bool open(std::string fileName);
    bool create();
    std::FILE* fileHandle() const;
    bool isValid() const;
    bool isLiveCapture() const;
    size_t size() const;
    size_t filePos() const;

    raw_packet* getNextPacketToDissect(size_t* packetOffsetBuff);
    size_t writeRawPacket(raw_packet* packet);

    HnPacket* readPacket(size_t offset = 0LL, bool restoreFilePos = false) const;
    
    bool saveFile(std::string fileName) const;
    bool recreate();

private:
    raw_packet* readRawPacket(size_t offset, bool restoreFilePos = false) const;
    uint8_t* readRawData(size_t length) const;

    void removeFile();

    std::FILE* file_ = nullptr;
    std::string fileName_ = "";
    const std::string fileExtension_ = ".hnw";

    size_t currOffset_ = 0;
    size_t fileSize_ = 0;
    bool isLiveCapture_ = false;

    int packetsToDissect_ = 0;
    size_t currDissectedPacketOffset_ = 0;

    mutable std::mutex mutex_;
    std::condition_variable cond_var_;
};
