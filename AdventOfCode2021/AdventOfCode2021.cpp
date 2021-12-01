#include <fstream>
#include <iostream>
#include <vector>

#include "../Lib/CommonLib.h"

const char* InputData = "TestDepthInput.txt";
//const char* InputData = "MyInput.txt";

int32_t LoadInputDepths(std::vector<int32_t>& OutDepths)
{
    std::ifstream inputDataStream;
    inputDataStream.open(InputData);

    if (!inputDataStream.is_open())
    {
        std::cerr << "Incorrect path to input data" << std::endl;
        return 1;
    }
    std::string buffer;
    while (std::getline(inputDataStream, buffer))
    {
        // std::atoi will return 0 if the value is invalid, I could use use the safe version that throws an
        // exception if it fails to parse the data.
        OutDepths.push_back(std::atoi(buffer.c_str()));
    }
    
    return 0;
}

int main(int argc, char* argv[])
{
    std::vector<int32_t> depths;
    if (const int32_t returnValue = LoadInputDepths(depths); returnValue != 0)
        return returnValue;

    int32_t increasedCount = 0;
    for (auto index = 0; index < depths.size(); ++index)
    {
        //std::cout << depths[index];
        if (index == 0)
        {
            //std::cout << " (N/A - no previous measurement)" << std::endl;
            continue;
        }

        const bool hasIncreased = depths[index] > depths[index-1];

        if (hasIncreased)
            increasedCount++;
        /*const char* increasedText = hasIncreased ? " (increased)" : " (decreased)";
        std::cout << increasedText << std::endl;*/
    }
    
    std::cout << "Number increased: " << increasedCount << std::endl;
    return 0;
}
