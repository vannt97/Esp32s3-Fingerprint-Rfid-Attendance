// ChipInfo.h
#pragma once
#include <string>
#include <vector>
#include "esp_chip_info.h"

class ChipInfo {
public:
    static std::string getCpuModel();
    static std::string getCores();
    static std::string getCpuFreq();
    static std::string getChipRevision();
    static std::string getPsramSize();
    static std::string getFreeHeap();
    static std::string getFlashSize();

    // Trả về toàn bộ thông số dưới dạng vector, dùng thẳng cho _lines
    static std::vector<std::string> getAllInfo();
};