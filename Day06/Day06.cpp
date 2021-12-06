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

    std::string buffer;
    std::vector<uint8_t> fishDb;
    fishDb.reserve(10000);
    while (std::getline(inputDataStream, buffer, ','))
    {
        fishDb.push_back(std::atoi(buffer.c_str()));
    }

    constexpr int GenerationCount = 80;
    for (int generation = 0; generation < GenerationCount; ++generation)
    {
        const size_t startFishCount = fishDb.size();
        for (int fishIndex = 0; fishIndex < startFishCount; ++fishIndex)
        {
            if (fishDb[fishIndex] == 0)
            {
                fishDb[fishIndex] = 6;
                fishDb.push_back(8);
            }
            else
                fishDb[fishIndex]--;
        }
        // std::cout << "After " << generation + 1 << "days: ";
        // for (auto fish : fishDb)
        // {
        //     std::cout << static_cast<int>(fish) << ',';
        // }
        // std::cout << std::endl;
    }

    std::cout << "Number of fish: " << fishDb.size() << std::endl;
    
    return 0;
}
