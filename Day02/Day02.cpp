#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>

#include "../Lib/CommonLib.h"

const char* InputData = "TestInputData.txt";
//const char* InputData = "MyInput.txt";

int main(int argc, char* argv[])
{
    Timer fullProgramTime("Full Program Time");

    std::ifstream inputDataStream;
    inputDataStream.open(InputData);

    if (!inputDataStream.is_open())
    {
        std::cerr << "Incorrect path to input data" << std::endl;
        return 1;
    }

    int_fast64_t position = 0;
    int_fast64_t depth = 0;
    int_fast64_t aim = 0;
    
    std::string buffer;
    while (std::getline(inputDataStream, buffer))
    {
        const size_t index = buffer.find(' ');
        if (index == std::string::npos)
            continue;

        const int_fast32_t count = std::atoi(buffer.c_str() + index);
        if (buffer[0] == 'f')
        {
            position += count;
            depth += aim * count;
        }
        else // up or down
            aim += buffer[0] == 'd' ? -count : count;
    }
    
    std::cout << "Position is " << position << std::endl;
    std::cout << "Depth is " << depth << std::endl;
    std::cout << "Aim is " << depth << std::endl;
    std::cout << "Answer is " << position * (-depth) << std::endl;
    return 0;
}
