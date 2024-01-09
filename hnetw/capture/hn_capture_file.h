#pragma once

#include <cstdio>
#include <stdint.h>
#include <packet/packets/hn_packet.h>

class HnCaptureFile {
public:
    HnCaptureFile();
    ~HnCaptureFile();

    bool createFile();
    std::FILE* fileHandle() const;
    bool isValid() const;

    HnPacket* readPacket(long offset, int packetLen) const;
    bool writePacket(const uint8_t* rawData, int packetLen);
    bool saveFile(std::string fileName) const;
    bool reset();

private:
    uint8_t* readRawData(long offset, int packetLen) const;
    void closeFile();

    std::FILE* file_ = nullptr;
    std::string fileName_ = "";
    const std::string fileExtension_ = ".hnw";

    long currOffset_ = 0;
    long fileSize_ = 0;

    mutable std::mutex mutex_;
    std::condition_variable cond_var_;
};
