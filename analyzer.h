#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <cstdint>
#include <fstream>
#include <algorithm>
#include <iostream>

struct ZoneCount {
    std::string zone;
    long long count;
};

struct SlotCount {
    std::string zone;
    int hour;
    long long count;
};

class TripAnalyzer {
private:
    std::unordered_map<std::string, uint32_t[25]> slotCounts;
public:
    void ingestFile(const std::string& csvPath);

    std::vector<ZoneCount> topZones(int k = 10) const;

    std::vector<SlotCount> topBusySlots(int k = 10) const;
};




