#include <iostream>
#include <fstream>
#include <string>

#include "../Lib/CommonLib.h"

const char* InputData = "TestInputData.txt";
//const char* InputData = "MyInput.txt";

void TryFlashOcto(std::vector<std::vector<int32_t>>& InOctoEnergyLevels, const int InX, const int InY, int32_t& InOutFlashCount)
{
    if (InX < 0 || InX >= InOctoEnergyLevels[0].size())
        return;

    if (InY < 0 || InY >= InOctoEnergyLevels.size())
        return;

    int& currentEnergyLevel = InOctoEnergyLevels[InY][InX];
    if (currentEnergyLevel == 0)
        return;

    if (currentEnergyLevel < 9)
    {
        currentEnergyLevel++;
        return;
    }

    // Ready to flash!
    currentEnergyLevel = 0;
    InOutFlashCount += 1;

    for (int y = -1; y <= 1; ++y)
    {
        for (int x = -1; x <= 1; ++x)
        {
            if (y == 0 && x == 0)
                continue;

            TryFlashOcto(InOctoEnergyLevels, InX + x, InY + y, InOutFlashCount);
        }
    }
}

int main(int argc, char* argv[])
{
    Timer fullTime("Day11 full time");

    std::ifstream inputDataStream;
    inputDataStream.open(InputData);
    
    if (!inputDataStream.is_open())
    {
        std::cerr << "Incorrect path to input data" << std::endl;
        return 1;
    }

    std::string buffer;

    std::vector<std::vector<int32_t>> octopusEnergyLevels;
    
    while (std::getline(inputDataStream, buffer))
    {
        std::vector<int32_t> rowData;

        for (const char currentChar : buffer)
        {
            if (currentChar < '0' && currentChar > '9')
                continue;;

            rowData.push_back(currentChar - '0');
        }

        octopusEnergyLevels.push_back(std::move(rowData));
    }

    const int32_t genCount = 100;
    int32_t totalFlashCount = 0;
    for (int32_t gen = 0; gen < genCount; ++gen)
    {
        // Increase all energy levels
        for (std::vector<int32_t>& row : octopusEnergyLevels)
        {
            for (int32_t& octoEnergyLevel : row)
                octoEnergyLevel+=1;
        }

        for (int row = 0; row < octopusEnergyLevels.size(); ++ row)
        {
            for (int col = 0; col < octopusEnergyLevels[row].size(); ++col)
            {
                if (octopusEnergyLevels[row][col] > 9)
                    TryFlashOcto(octopusEnergyLevels, col, row, totalFlashCount);
            }
        }

        /*std::cout << std::endl << "After step " << gen + 1 << std::endl;
        for (const std::vector<int32_t>& row : octopusEnergyLevels)
        {
            for (const int32_t& octoEnergyLevel : row)
                std::cout << octoEnergyLevel;
            std::cout << std::endl;
        }*/
    }
    std::cout << "Total flash count: " << totalFlashCount << std::endl;
    
    return 0;
}
