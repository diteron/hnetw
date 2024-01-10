#include <stdafx.h>
#include <thread>
#include <chrono>

#include <packet/hn_packet_factory.h>
#include "hn_capture_file.h"

HnCaptureFile::HnCaptureFile()
{}

HnCaptureFile::~HnCaptureFile()
{
    if (file_) {
        std::fclose(file_);
        std::remove(fileName_.c_str());
    }
}

bool HnCaptureFile::createFile()
{
    char cFileName[L_tmpnam];
    char* fnamePtr = std::tmpnam(cFileName);
    if (fnamePtr == nullptr) {
        return false;
    }
    
    fileName_ = fnamePtr;
    fileName_ += fileExtension_;

    file_ = std::fopen(fileName_.c_str(), "wb+");
    if (!file_) return false;
    
    return true;
}

std::FILE* HnCaptureFile::fileHandle() const
{
    return file_;
}

bool HnCaptureFile::isValid() const
{
    return file_;
}

long HnCaptureFile::size() const
{
    std::lock_guard<std::mutex> lock(mutex_);
    return fileSize_;
}

HnPacket* HnCaptureFile::readPacket(long offset, int packetLen) const
{
    uint8_t* rawData = readRawData(offset, packetLen);
    ipv4_hdr* ipHdr = reinterpret_cast<ipv4_hdr*>(rawData);

    HnPacket* packet = HnPacketFactory::instance()->buildPacket(ipHdr->protocol);
    packet->setPacketData(rawData, packetLen);

    return packet;
}

bool HnCaptureFile::writePacket(const uint8_t* rawData, int packetLen)
{
    std::lock_guard<std::mutex> lock(mutex_);

    int writtenBytesCnt = std::fwrite((const uint8_t*)rawData, sizeof(uint8_t), packetLen, file_);

    if (writtenBytesCnt < packetLen)
        return false;

    fileSize_ += packetLen;
    return true;
}

bool HnCaptureFile::saveFile(std::string fileName) const
{
    std::lock_guard<std::mutex> lock(mutex_);

    std::FILE* savingFile = std::fopen(fileName.c_str(), "wb+");
    if (!savingFile) return false;

    long prevOffset = std::ftell(file_);
    std::fseek(file_, 0, SEEK_SET);

    const int buffSize = 524'288;
    uint8_t* buff = new uint8_t[buffSize];
    int readBytesCnt = 0;
    int writtenBytesCnt = 0;

    while ((readBytesCnt = std::fread(buff, sizeof(uint8_t), buffSize, file_)) > 0) {
        writtenBytesCnt = std::fwrite(buff, sizeof(uint8_t), readBytesCnt, savingFile);
        if (writtenBytesCnt < readBytesCnt) {
            goto error;
        }
    }

    if (readBytesCnt < 0) {
        goto error;
    }

    std::fclose(savingFile);
    std::fseek(file_, prevOffset, SEEK_SET);
    delete[] buff;

    return true;

error:
    std::fclose(savingFile);
    std::fseek(file_, prevOffset, SEEK_SET);
    delete[] buff;
    return false;
}

bool HnCaptureFile::recreate()
{
    std::lock_guard<std::mutex> lock(mutex_);
    fileSize_ = 0;
    closeFile();
    if (!createFile()) return false;

    return true;
}

uint8_t* HnCaptureFile::readRawData(long offset, int packetLen) const
{
    std::lock_guard<std::mutex> lock(mutex_);

    long prevOffset = std::ftell(file_);
    std::fseek(file_, offset, SEEK_SET);

    uint8_t* packetData = new uint8_t[packetLen];
    int readBytesCnt = std::fread((uint8_t*)packetData, sizeof(uint8_t), packetLen, file_);

    if (readBytesCnt < packetLen) {
        delete[] packetData;
        std::fseek(file_, prevOffset, SEEK_SET);
        return nullptr;
    }

    std::fseek(file_, prevOffset, SEEK_SET);
    return packetData;
}

void HnCaptureFile::closeFile()
{
    if (file_) {
        std::fclose(file_);
        std::remove(fileName_.c_str());
    }
}
