#include <stdafx.h>
#include <thread>
#include <chrono>

#include <packet/hn_packet_factory.h>
#include <ui/qtmodels/hn_packet_list_model.h>
#include <ui/qtmodels/hn_packet_list_row.h>
#include "hn_capture_file.h"

HnCaptureFile::HnCaptureFile() : mutex_(), cond_var_()
{}

HnCaptureFile::~HnCaptureFile()
{
    if (isLiveCapture_)         // If capture is live - remove temporary file
        removeFile();
    else
        std::fclose(file_);     // Otherwise, close the saved file that we are viewing 
}

bool HnCaptureFile::open(std::string fileName)
{
    std::lock_guard<std::mutex> lock(mutex_);

    if (isLiveCapture_)
        removeFile();

    file_ = std::fopen(fileName.c_str(), "rb");
    if (!file_) return false;

    fileName_ = fileName;

    isLiveCapture_ = false;

    fseek(file_, 0L, SEEK_END);
    fileSize_ = ftell(file_);
    std::rewind(file_);

    return true;
}

bool HnCaptureFile::create()
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
    
    isLiveCapture_ = true;
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

bool HnCaptureFile::isLiveCapture() const
{
    return isLiveCapture_;
}

long HnCaptureFile::size() const
{
    std::lock_guard<std::mutex> lock(mutex_);
    return fileSize_;
}

long HnCaptureFile::filePos() const
{
    return std::ftell(file_);
}

raw_packet* HnCaptureFile::getNextPacketToDissect(long* packetOffsetBuff)
{
    std::unique_lock<std::mutex> lock(mutex_);
    while (packetsToDissect_ == 0) {
        cond_var_.wait(lock);
    }

    raw_packet* rawPacket = readRawPacket(currDissectedPacketOffset_, true);

    if (rawPacket != nullptr) {
        *packetOffsetBuff = currDissectedPacketOffset_;
        currDissectedPacketOffset_ += rawPacket->length + sizeof(rawPacket->id) 
                                      + sizeof(rawPacket->length) + sizeof(rawPacket->time);
        --packetsToDissect_;
    }

    return rawPacket;
}

int HnCaptureFile::writeRawPacket(raw_packet* packet)
{
    std::lock_guard<std::mutex> lock(mutex_);

    int writtenCnt = 0;
    int totalBytesWritten = 0;

    int packetLen = packet->length;
    int id = packet->id;
    clock_t arrivTime = packet->time;
    const uint8_t* rawData = packet->data;

    // Write packet length
    writtenCnt = std::fwrite(&packetLen, sizeof(packetLen), 1, file_);
    if (writtenCnt < 1) goto write_err;
    totalBytesWritten += sizeof(packetLen);

    // Write packet id
    writtenCnt = std::fwrite(&id, sizeof(id), 1, file_);
    if (writtenCnt < 1) goto write_err;
    totalBytesWritten += sizeof(id);

    // Write packet arrival time
    writtenCnt = std::fwrite(&arrivTime, sizeof(arrivTime), 1, file_);
    if (writtenCnt < 1) goto write_err;
    totalBytesWritten += sizeof(arrivTime);

    // Write packet raw data
    writtenCnt = std::fwrite(rawData, sizeof(uint8_t), packetLen, file_);
    if (writtenCnt < packetLen) goto write_err;
    totalBytesWritten += writtenCnt;

    fileSize_ += packetLen;
    ++packetsToDissect_;

    delete packet;
    cond_var_.notify_one();

    return totalBytesWritten;

    write_err:
        delete packet;
        cond_var_.notify_one();
        return 0;
}

HnPacket* HnCaptureFile::readPacket(long offset, bool ​restoreFilePos) const
{
    std::lock_guard<std::mutex> lock(mutex_);

    /* We don't need to restore the file position indicator if we are reading entire saved capture file 
       But when we need to read a random packet, we must restore indicator for a correct write operation */
    long prevOffset = 0;
    if (​restoreFilePos) {                      
        prevOffset = std::ftell(file_);
        std::fseek(file_, offset, SEEK_SET);
    }

    int readCnt = 0;

    int packetLen = 0;
    readCnt = std::fread(&packetLen, sizeof(packetLen), 1, file_);
    if (readCnt < 1) return nullptr;

    int id = -1;
    readCnt = std::fread(&id, sizeof(id), 1, file_);
    if (readCnt < 1) return nullptr;

    clock_t arrivTime = -1;
    readCnt = std::fread(&arrivTime, sizeof(arrivTime), 1, file_);
    if (readCnt < 1) return nullptr;

    uint8_t* rawData = readRawData(packetLen);
    if (rawData == nullptr) return nullptr;

    ipv4_hdr* ipHdr = reinterpret_cast<ipv4_hdr*>(rawData);

    HnPacket* packet = HnPacketFactory::instance()->buildPacket(ipHdr->protocol, id);
    packet->setPacketData(rawData, packetLen);
    packet->setArrivalTime(arrivTime);

    if (​restoreFilePos)
        std::fseek(file_, prevOffset, SEEK_SET);

    return packet;
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
    currDissectedPacketOffset_ = 0;
    
    if (isLiveCapture_)         // If capture is live - remove temporary file
        removeFile();
    else
        std::fclose(file_);     // Otherwise, close the saved file that we are viewing 

    if (!create()) return false;

    return true;
}

raw_packet* HnCaptureFile::readRawPacket(long offset, bool restoreFilePos) const
{
    /* We don't need to restore the file position indicator if we are reading entire saved capture file
       But when we need to read a random packet, we must restore indicator for a correct write operation */
    long prevOffset = 0;
    if (restoreFilePos) {
        prevOffset = std::ftell(file_);
        std::fseek(file_, offset, SEEK_SET);
    }

    int readCnt = 0;

    int packetLen = 0;
    readCnt = std::fread(&packetLen, sizeof(packetLen), 1, file_);
    if (readCnt < 1) return nullptr;

    int id = -1;
    readCnt = std::fread(&id, sizeof(id), 1, file_);
    if (readCnt < 1) return nullptr;

    clock_t arrivTime = -1;
    readCnt = std::fread(&arrivTime, sizeof(arrivTime), 1, file_);
    if (readCnt < 1) return nullptr;

    uint8_t* rawData = readRawData(packetLen);
    if (rawData == nullptr) return nullptr;

    ipv4_hdr* ipHdr = reinterpret_cast<ipv4_hdr*>(rawData);

    raw_packet* packet = new raw_packet{ id, arrivTime, rawData, packetLen };

    if (restoreFilePos)
        std::fseek(file_, prevOffset, SEEK_SET);

    return packet;
}

uint8_t* HnCaptureFile::readRawData(int length) const
{

    uint8_t* packetData = new uint8_t[length];
    int readBytesCnt = std::fread((uint8_t*)packetData, sizeof(uint8_t), length, file_);

    if (readBytesCnt < length) {
        delete[] packetData;
        return nullptr;
    }
    
    return packetData;
}

void HnCaptureFile::removeFile()
{
    if (file_) {
        std::fclose(file_);
        std::remove(fileName_.c_str());
    }
}
