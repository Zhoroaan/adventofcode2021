#include <fstream>
#include <iostream>
#include <random>
#include <vector>

#include "../Lib/CommonLib.h"

const char* InputData = "TestDepthInput.txt";
//const char* InputData = "MyInput.txt";
//const char* InputData = "BigInput.txt";
constexpr int_fast32_t GenerateCount = 100000000;

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

int32_t ExperimentalWipLoadInputDepths(std::vector<int32_t>& OutDepths)
{
    OutDepths.reserve(100000000);
    std::ifstream inputDataStream;
    inputDataStream.open(InputData);

    if (!inputDataStream.is_open())
    {
        std::cerr << "Incorrect path to input data" << std::endl;
        return 1;
    }
    std::string buffer;
    constexpr int32_t BufferSize = 1024 * 512;
    char inputData[BufferSize];
    static_assert(1024 * 512 == sizeof(inputData));
    int32_t numNewLines = 0;
    int32_t iteration = 0;
    inputDataStream.read(inputData, sizeof(inputData));
    while (inputDataStream && inputDataStream.gcount() != 0)
    {
        std::cout << inputDataStream.gcount() << " bytes read" << std::endl;
        iteration++;
        // std::atoi will return 0 if the value is invalid, I could use use the safe version that throws an
        // exception if it fails to parse the data.
        //OutDepths.push_back(std::atoi(buffer.c_str()));
        int32_t numBytesToCut = 0;
        for (std::streamsize i = inputDataStream.gcount(); inputData[i] != '\n'; i--)
            numBytesToCut++;

        for (int i = 0; i != inputDataStream.gcount(); i++)
        {
            if (inputData[i] == '\n')
                numNewLines++;
        }
        
        if (numBytesToCut != 0)
            inputDataStream.seekg(-numBytesToCut, std::ios::cur);

        inputDataStream.read(inputData, sizeof(inputData));
    }

    std::cout << "Missing newlines: " << GenerateCount - numNewLines  << " iteration " << iteration<< std::endl;
    
    return 0;
}

std::vector<int32_t> GenerateSlidingWindowDepths(const std::vector<int32_t>& InDepths)
{
    std::vector<int32_t> slidingWindowDepths;

    if (InDepths.size() < 3)
        return std::move(slidingWindowDepths);

    slidingWindowDepths.reserve(InDepths.size() - 2);
    
    int32_t currentCount = InDepths[0] + InDepths[1];
    
    for (int index = 0; index < InDepths.size() - 2; ++index)
    {
        currentCount += InDepths[index + 2];

        slidingWindowDepths.push_back(currentCount);
        
        currentCount -= InDepths[index];
    }
    
    return std::move(slidingWindowDepths);
}

void GenerateRandomInput()
{
    std::ofstream inputDataStream("BigInput.txt");
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution<int32_t> dist(200, 999);
    for (int i = 0; i < GenerateCount; ++i)
    {
        inputDataStream << dist(mt);
        if (i + 1 != GenerateCount)
            inputDataStream << '\n';
    }
}

int main(int argc, char* argv[])
{
    std::vector<int32_t> depths;
    {
        Timer applicationTime("Read file");
        if (const int32_t returnValue = LoadInputDepths(depths); returnValue != 0)
            return returnValue;
    }

    int32_t increasedCount = 0;

    Timer _tt("Generate sliding window depths");
    std::vector<int32_t> slidingWindowDepths = GenerateSlidingWindowDepths(depths);
    for (auto index = 0; index < slidingWindowDepths.size(); ++index)
    {
        // std::cout << slidingWindowDepths[index];
        if (index == 0)
        {
            // std::cout << " (N/A - no previous measurement)" << std::endl;
            continue;
        }

        const bool hasIncreased = slidingWindowDepths[index] > slidingWindowDepths[index-1];

        if (hasIncreased)
            increasedCount++;

        // const bool isSame =  slidingWindowDepths[index] == slidingWindowDepths[index-1];
        // const char* increasedText = hasIncreased ? " (increased)" : isSame ? " (no change)" : " (decreased)";
        // std::cout << increasedText << std::endl;
    }
    
    std::cout << "Number increased: " << increasedCount << std::endl;
    return 0;
}
