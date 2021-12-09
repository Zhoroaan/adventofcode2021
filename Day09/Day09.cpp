#include <iostream>
#include <fstream>
#include <stdbool.h>
#include <string>

#include "../Lib/CommonLib.h"

//const char* InputData = "TestInputData.txt";
const char* InputData = "MyInput.txt";

int main(int argc, char* argv[])
{
    Timer fullTime("Day09 full time");

    std::ifstream inputDataStream;
    inputDataStream.open(InputData);
    
    if (!inputDataStream.is_open())
    {
        std::cerr << "Incorrect path to input data" << std::endl;
        return 1;
    }

    std::string buffer;
    std::vector<std::vector<uint8_t>> heights;
    while (std::getline(inputDataStream, buffer))
    {
        std::vector<uint8_t> rowData;
        for (int charIndex = 0; charIndex < buffer.size(); ++charIndex)
        {
            const char currentChar = buffer[charIndex];
            if (currentChar < '0' &&currentChar > '9')
                continue;;

            rowData.push_back(currentChar - '0');
        }
        heights.push_back(std::move(rowData));
    }
    const auto xSize = heights[0].size();
    const auto ySize = heights.size();
    int32_t totalRisk = 0;
    for (int row = 0; row < heights.size(); ++row)
    {
        for (int column = 0; column < xSize; ++column)
        {
            const int currentHeight = heights[row][column];
            bool smallest = true;
            smallest &= row == 0 || heights[row - 1][column] > currentHeight; // Up
            smallest &= row == ySize - 1 || heights[row + 1][column] > currentHeight; // Down
            smallest &= column == 0 || heights[row][column - 1] > currentHeight; // Left
            smallest &= column == xSize - 1 || heights[row][column+ 1 ] > currentHeight; // Right
            if (smallest)
            {
                totalRisk += currentHeight + 1;
                //std::cout << "Smallest found: (" << column << ", " << row << ") height: " << currentHeight << std::endl;
            }
        }
    }
    std::cout << "Total risk level: " << totalRisk << std::endl;
    
    return 0;
}
