#include "analyzer.h"

bool parseHour(const std::string& line,int start, int end,int& hour){
    int i = start;
    while(i < end && line[i] != ' '){i++;}
    if (i + 1 >= end){return false;}
    i++;
    int h = line[i] - '0';
    i++;
    if (i < end && line[i] >= '0' && line[i] <= '9'){
        h = h * 10 + (line[i] - '0');
        i++;
    }
    if (i >= end || line[i] != ':' || h > 23){return false;}
    hour = h;
    return true;
}

bool compareZones(const ZoneCount& a, const ZoneCount& b){
    if (a.count != b.count){
        return a.count > b.count;
    } 

    return a.zone < b.zone;
}

bool compareSlots(const SlotCount& a, const SlotCount& b){
    if (a.count != b.count){
        return a.count > b.count;
    }

    if (a.zone != b.zone){
       return a.zone < b.zone; 
    }

    return a.hour < b.hour;
}

void TripAnalyzer::ingestFile(const std::string& csvPath) {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::ifstream file(csvPath);
    if (!file.is_open()) return;

    std::string line;
    std::string zone;

    if(!std::getline(file, line)){return;}

    while (std::getline(file, line)){

        int firstComma  = line.find(',');
        int secondComma = line.find(',', firstComma + 1);
        int thirdComma  = line.find(',', secondComma + 1);
        int fourthComma = line.find(',', thirdComma + 1);
        int fifthComma  = line.find(',', fourthComma + 1);

        if (firstComma  == -1 ||secondComma == -1 ||thirdComma  == -1 ||fourthComma == -1 ||fifthComma  == -1){continue;}

        zone.assign(line.data() + firstComma + 1,secondComma - firstComma - 1);
        if(zone.empty()){continue;}

        int hour;
        if (!parseHour(line, thirdComma + 1, fourthComma, hour)){continue;}

        auto& hours = slotCounts[zone];
        hours[hour]++;
        hours[24]++;
    }
}

std::vector<ZoneCount> TripAnalyzer::topZones(int k) const {
    std::vector<ZoneCount> result;

    for (auto& p : slotCounts){
        uint32_t total = p.second[24];
        if (total > 0){
            result.push_back({p.first, total});
        }
    }

    int limit = std::min(k, static_cast<int>(result.size()));
    std::partial_sort(result.begin(),result.begin() + limit,result.end(),compareZones);

    result.resize(limit);
    return result;
}

std::vector<SlotCount> TripAnalyzer::topBusySlots(int k) const {
    std::vector<SlotCount> result;
    result.reserve(slotCounts.size() * 4);

    for (auto& p : slotCounts) {
        auto& hours = p.second;
        for (int h = 0; h < 24; ++h){
            if (hours[h] > 0){
                result.push_back({p.first, h, hours[h]});
            }
        }
    }

    int limit = std::min(k, static_cast<int>(result.size()));
    std::partial_sort(result.begin(),result.begin() + limit,result.end(),compareSlots);

    result.resize(limit);
    return result;
}



