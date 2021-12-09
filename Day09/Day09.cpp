#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>

#include "../Lib/CommonLib.h"

//const char* InputData = "TestInputData.txt";
const char* InputData = "MyInput.txt";

struct PointInfo
{
    PointInfo ()
        : PointX(0), PointY(0), Height(0)
    {}
    PointInfo(uint8_t InPointX, uint8_t InPointY, uint32_t InHeight)
    : PointX(InPointX), PointY(InPointY), Height(InHeight)
    { }
    uint8_t PointX;
    uint8_t PointY;
    uint32_t Height;

    uint16_t GetHash() const
    {
        return PointY << 8 | PointX;
    }
};

using BasinMap = std::unordered_map<uint16_t, PointInfo>;

void CreateBasin(
    const std::vector<std::vector<uint8_t>>& InHeights,
    int InColumn, int InRow, size_t InXSize, size_t InYSize,
    BasinMap& InOutCurrentBasin)
{
    if (InRow < 0 || InRow >= InYSize)
        return;

    if (InColumn < 0 || InColumn >= InXSize)
        return;

    const uint8_t currentHeight = InHeights[InRow][InColumn];
    if (currentHeight == 9)
        return;

    const PointInfo newPoint(InColumn, InRow, currentHeight);
    if (InOutCurrentBasin.contains(newPoint.GetHash()))
        return;

    InOutCurrentBasin[newPoint.GetHash()] = newPoint;
    CreateBasin(InHeights, InColumn, InRow - 1, InXSize, InYSize, InOutCurrentBasin); // Up
    CreateBasin(InHeights, InColumn, InRow + 1, InXSize, InYSize, InOutCurrentBasin); // Down
    CreateBasin(InHeights, InColumn - 1, InRow, InXSize, InYSize, InOutCurrentBasin); // Left
    CreateBasin(InHeights, InColumn + 1, InRow, InXSize, InYSize, InOutCurrentBasin); // Right
}

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
    std::vector<BasinMap> basinHeights;
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
                BasinMap currentBasinPoints;
                CreateBasin(heights, column, row, xSize, ySize, currentBasinPoints);
                basinHeights.push_back(std::move(currentBasinPoints));
                //std::cout << "Smallest found: (" << column << ", " << row << ") height: " << currentHeight << std::endl;
            }
        }
    }

    std::ranges::sort(basinHeights, [](const BasinMap& InBasinA, const BasinMap& InBasinB) {
        return InBasinA.size() > InBasinB.size();
    });

    std::cout << "Multiplied size " << basinHeights[0].size() * basinHeights[1].size() * basinHeights[2].size() << std::endl;
    
    return 0;
}
