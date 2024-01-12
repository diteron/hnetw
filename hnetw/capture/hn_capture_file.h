#pragma once

#include <cstdio>
#include <stdint.h>
#include <packet/packets/hn_packet.h>

typedef char* pBuffer;
struct raw_packet {
    int id;
    std::clock_t time;
    uint8_t* data;
    int length;

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
    long size() const;
    long filePos() const;

    raw_packet* getNextPacketToDissect(long* packetOffsetBuff);
    int writeRawPacket(raw_packet* packet);

    HnPacket* readPacket(long offset = 0L, bool restoreFilePos = false) const;
    
    bool saveFile(std::string fileName) const;
    bool recreate();

private:
    raw_packet* readRawPacket(long offset = 0L, bool restoreFilePos = false) const;
    uint8_t* readRawData(int length) const;

    void removeFile();

    std::FILE* file_ = nullptr;
    std::string fileName_ = "";
    const std::string fileExtension_ = ".hnw";

    long currOffset_ = 0;
    long fileSize_ = 0;
    bool isLiveCapture_ = false;

    int packetsToDissect_ = 0;
    long currDissectedPacketOffset_ = 0;

    mutable std::mutex mutex_;
    std::condition_variable cond_var_;
};
