#include <iostream>
#include <fstream>
#include <string>

#include "../Lib/CommonLib.h"

const char* InputData = "TestInputData.txt";
// const char* InputData = "MyInput.txt";

int main(int argc, char* argv[])
{
    Timer fullTime("Day08 full time");

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
    }
    return 0;
}
