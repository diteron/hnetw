#pragma once

#include <queue>
#include <mutex>
#include <condition_variable>

typedef char* pBuffer;
struct raw_packet {
    int id;
    std::clock_t time;
    pBuffer buffer;
    int length;
};

class HnPacketQueue {
public:
    HnPacketQueue();
    ~HnPacketQueue();

    void enqueue(raw_packet rPacket);
    raw_packet dequeue();
    bool empty() const;
    void clear();

private:
    std::queue<raw_packet> queue_;
    mutable std::mutex mutex_;
    std::condition_variable cond_var_;
};
