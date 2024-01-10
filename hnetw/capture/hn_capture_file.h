#pragma once

#include <cstdio>
#include <stdint.h>
#include <packet/packets/hn_packet.h>

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

    HnPacket* readPacket(long offset = 0L, bool restoreFilePos = false) const;
    int writePacket(HnPacket* packet);
    bool saveFile(std::string fileName) const;
    bool recreate();

private:
    uint8_t* readRawData(int length) const;
    void removeFile();

    std::FILE* file_ = nullptr;
    std::string fileName_ = "";
    const std::string fileExtension_ = ".hnw";

    long currOffset_ = 0;
    long fileSize_ = 0;
    bool isLiveCapture_ = false;

    mutable std::mutex mutex_;
    std::condition_variable cond_var_;
};
