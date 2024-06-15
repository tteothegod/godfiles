#pragma once
#include "maths.h"

struct Properties;

void processRecordedData(Properties* props);

struct MouseProcessData
{
    std::vector<float> x;
    std::vector<float> y;
    std::vector<float> times;
};

struct SavedProcessData
{
    std::vector<v2> positions;
    std::vector<float> durations;
};

void savePositionDataToFile(const std::vector<SavedProcessData>& data, const std::string& filename);

void deleteFunctionContentAndReplace(const std::string& filePath);



