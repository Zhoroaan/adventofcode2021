#include <algorithm>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#include "../Lib/CommonLib.h"

const char* InputData = "TestInputData.txt";
// const char* InputData = "MyInput.txt";

inline uint64_t CalculateCost(int InStepCount)
{
    uint64_t totalCost = 0;
    for (int step = 1; step <= InStepCount; step++)
        totalCost += step;
    
    return totalCost;
}

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

    std::vector<uint32_t> crabStartPositions;
    std::string buffer;
    
    while (std::getline(inputDataStream, buffer, ','))
    {
		crabStartPositions.push_back(std::atoi(buffer.c_str()));
	}

    std::sort(crabStartPositions.begin(), crabStartPositions.end());

    const int32_t maxNumber = *(crabStartPositions.end() - 1);
    uint64_t minCost = std::numeric_limits<uint64_t>::max();
    int32_t bestPosition = std::numeric_limits<int32_t>::min();;
    for (int32_t x = crabStartPositions[0]; x <= maxNumber; ++x)
    {
        uint64_t count = 0;
        for (const int32_t crabStartPosition : crabStartPositions)
        {
            count += CalculateCost(std::abs(crabStartPosition - x));
        }
        if (count < minCost)
        {
            minCost = count;
            bestPosition = x;
        }
    }

    std::cout << "Best position is " << bestPosition << " with cost " << minCost << std::endl;
    
    return 0;
}
