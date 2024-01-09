#pragma once

class HnConverter {
public:
    HnConverter() = delete;

    static QString uint32ToIpString(uint32_t ip);
    static QString msSinceMidntUtcToTimeString(uint32_t milliseconds);
    static QString time_tToString(std::clock_t time);
};

