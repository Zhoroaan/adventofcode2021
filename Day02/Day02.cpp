#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>

#include "../Lib/CommonLib.h"

// const char* InputData = "TestInputData.txt";
const char* InputData = "MyInput.txt";

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

    int_fast32_t position = 0;
    int_fast32_t depth = 0;
    
    std::string buffer;
    while (std::getline(inputDataStream, buffer))
    {
        const size_t index = buffer.find(' ');
        if (index == std::string::npos)
            continue;
        // std::atoi will return 0 if the value is invalid, I could use use the safe version that throws an
        // exception if it fails to parse the data.
        int_fast32_t count = std::atoi(buffer.c_str() + index);
        switch (buffer[0])
        {
        case 'f':
            position += count;
            break;
        case 'd':
            depth -= count;
            break;
        default: // up
            depth += count;
        }
    }
    
    std::cout << "Position is " << position << std::endl;
    std::cout << "Depth is " << depth << std::endl;
    std::cout << "Answer is " << position * (-depth) << std::endl;
    return 0;
}
