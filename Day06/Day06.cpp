#include <array>
#include <iostream>
#include <fstream>

#include "../Lib/CommonLib.h"

const char* InputData = "TestInputData.txt";
//const char* InputData = "MyInput.txt";

int main(int argc, char* argv[])
{
    Timer fullTime("Full time");

    std::ifstream inputDataStream;
    inputDataStream.open(InputData);
    
    if (!inputDataStream.is_open())
    {
        std::cerr << "Incorrect path to input data" << std::endl;
        return 1;
    }

    std::array<uint64_t, 9> fishBuckets = {0};
    std::string buffer;
    
    while (std::getline(inputDataStream, buffer, ','))
    {
        const int currentBucket = std::atoi(buffer.c_str());
        fishBuckets[currentBucket]++;
    }

    constexpr int GenerationCount = 256;
    for (int generation = 0; generation < GenerationCount; ++generation)
    {
        const uint64_t countToAdd = fishBuckets[0];
        for (int bucketCount = 1; bucketCount < 9; ++bucketCount)
        {
            fishBuckets[bucketCount -1] = fishBuckets[bucketCount];
        }
        fishBuckets[6] += countToAdd;
        fishBuckets[8] = countToAdd;
    }

    uint64_t totalFishCount = 0;
    for (const auto fishCount : fishBuckets)
    {
        totalFishCount += fishCount;
    }

    std::cout << "Number of fish: " << totalFishCount << std::endl;
    
    return 0;
}
