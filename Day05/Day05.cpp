#include <iostream>
#include <fstream>
#include <regex>
#include <stdbool.h>

#include "../Lib/CommonLib.h"

const char* InputData = "TestInputData.txt";
//const char* InputData = "MyInput.txt";

class Diagram
{
public:
    void GrowToFitY(int InY)
    {
        while(InY + 1 > Height)
        {
            std::vector<int> startValues;
            startValues.reserve(Width);
            for (int x = 0; x < Height; ++x)
            {
                startValues.push_back(0);
            }
            Board.push_back(startValues);
            Height++;
        }
    }
    void GrowToFitX(int InX)
    {
        Width = std::max(Width,  InX + 1);
        for (std::vector<int>& row : Board)
        {
            row.reserve(Width);
            for (size_t x = row.size(); x < Width; ++x)
            {
                row.push_back(0);
            }
        }
    }

    void AddLine(int InX1, int InY1, int InX2, int InY2)
    {
        GrowToFitX(InX1);
        GrowToFitY(InY1);
        GrowToFitX(InX2);
        GrowToFitY(InY2);
        const bool isDiagonal = InX1 != InX2 && InY1 != InY2;
        const bool isHorizontal = !isDiagonal &&  InX1 != InX2;
        if (isDiagonal)
        {
            int minX = std::min(InX1, InX2);
            int startY = minX == InX1 ? InY1 : InY2;
            int incrementYValue = (startY == InY1 ? InY1 < InY2 : InY2 < InY1) ? 1 : -1;
            int maxX = std::max(InX1, InX2);
            for (int x = minX, y = startY; x <= maxX; ++x, y+=incrementYValue)
            {
                Board[y][x]++; // Y will remain the same
            }
        }
        else if (isHorizontal)
        {
            int maxX = std::max(InX1, InX2);
            for (int x = std::min(InX1, InX2); x <= maxX; ++x)
            {
                Board[InY1][x]++; // Y will remain the same
            }
        }
        else
        {
            int maxY = std::max(InY1, InY2);
            for (int y = std::min(InY1, InY2); y <= maxY; ++y)
            {
                Board[y][InX1]++; // X will remain the same
            }
        }
    }

    void WriteDebug()
    {
        constexpr bool writeDebug = false;
        std::cout << "Playfield width: " << Width << " height: " << Height << std::endl;
        int32_t counter = 0;
        for (auto row : Board)
        {
            for (auto count : row)
            {
                if (count >= 2)
                    counter++;

                if constexpr (writeDebug)
                {
                    if (count == 0)
                        std::cout << ".";
                    else
                        std::cout << count;
                }
            }
            if constexpr (writeDebug)
            {
                std::cout << std::endl;
            }
                
        }
        std::cout << "Number of overlaps: " << counter << std::endl;
    }
protected:
    int Width = 0;
    int Height = 0;
    std::vector<std::vector<int>> Board;
};

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
    std::smatch regexMatch;
    std::string bingoLine = "(\\d*),(\\d*)[\\s->]*(\\d*),(\\d*)";
    std::regex fieldData(bingoLine, std::regex_constants::ECMAScript);
    Diagram diagram;
    while (std::getline(inputDataStream, buffer))
    {
        if (!std::regex_search(buffer, regexMatch, fieldData))
            continue;
        //numbersToDraw.push_back(std::atoi(buffer.c_str()));
        diagram.AddLine(
            std::atoi(regexMatch[1].str().c_str()),
            std::atoi(regexMatch[2].str().c_str()),
            std::atoi(regexMatch[3].str().c_str()),
            std::atoi(regexMatch[4].str().c_str())
        );
    }

    diagram.WriteDebug();
    
    return 0;
}
