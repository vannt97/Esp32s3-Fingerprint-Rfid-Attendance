

// ChipInfo.cpp
#include "ChipInfo.h"
#include <Arduino.h>

std::string ChipInfo::getCpuModel()
{
    return "CPU: " + std::string(ESP.getChipModel());
}

std::string ChipInfo::getCores()
{
    return "Cores: " + std::to_string(ESP.getChipCores());
}

std::string ChipInfo::getCpuFreq()
{
    return "Freq: " + std::to_string(ESP.getCpuFreqMHz()) + "MHz";
}

std::string ChipInfo::getChipRevision()
{
    return "ChipRev: " + std::to_string(ESP.getChipRevision());
}

std::string ChipInfo::getPsramSize()
{
    return "PSRAM: " + std::to_string(ESP.getPsramSize() / 1024) + "KB";
}

std::string ChipInfo::getFreeHeap()
{
    return "RAM: " + std::to_string(ESP.getFreeHeap() / 1024) + "KB";
}

std::string ChipInfo::getFlashSize()
{
    return "Flash: " + std::to_string(ESP.getFlashChipSize() / 1024 / 1024) + "MB";
}

std::vector<std::string> ChipInfo::getAllInfo()
{
    return {
        getCpuModel(),
        getCores(),
        getCpuFreq(),
        getChipRevision(),
        getPsramSize(),
        getFreeHeap(),
        getFlashSize(),
    };
}