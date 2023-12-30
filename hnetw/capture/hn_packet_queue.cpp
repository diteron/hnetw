#include <stdafx.h>

#include "hn_packet_queue.h"


HnPacketQueue::HnPacketQueue() : queue_(), mutex_(), cond_var_()
{}

HnPacketQueue::~HnPacketQueue()
{}

void HnPacketQueue::enqueue(raw_packet rawPacket)
{
    std::lock_guard<std::mutex> lock(mutex_);
    queue_.push(rawPacket);
    cond_var_.notify_one();
}

raw_packet HnPacketQueue::dequeue()
{
    std::unique_lock<std::mutex> lock(mutex_);
    while (queue_.empty()) {
        cond_var_.wait(lock);
    }

    raw_packet rawPacket = queue_.front();
    queue_.pop();

    return rawPacket;
}

bool HnPacketQueue::empty() const
{
    std::lock_guard<std::mutex> lock(mutex_);
    bool isEmpty = queue_.empty();
    return isEmpty;
}

void HnPacketQueue::clear()
{
    std::lock_guard<std::mutex> lock(mutex_);
    queue_ = std::queue<raw_packet>();
    cond_var_.notify_one();
}
