#include <bitset>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "../Lib/CommonLib.h"

//const char* InputData = "TestInputData.txt";
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
    std::vector<uint_fast32_t> bitCount;
    std::string buffer;
    std::getline(inputDataStream, buffer);
    
    uint_fast32_t inverseRelevance = 0;
    for (const char currentBit : buffer)
    {
        if (currentBit != '0' && currentBit != '1')
            continue;
        inverseRelevance |= 1 << bitCount.size();
        bitCount.insert(bitCount.begin(), currentBit == '0' ? 0 : 1);
    }
    int_fast32_t rowCount = 0;
    while (std::getline(inputDataStream, buffer))
    {
        rowCount += 1;
        for(int32_t charIndex = static_cast<int32_t>(buffer.size()) - 1; charIndex >= 0 ; charIndex--)
        {
            if (buffer[charIndex] == '1')
                bitCount[charIndex] += 1;
        }
    }

    uint_fast32_t gamma = 0;
    const uint_fast32_t minCount = rowCount / 2 + 1;
    for(auto bitIndex = 0; bitIndex < bitCount.size(); ++bitIndex)
    {
        if (bitCount[bitIndex] > minCount)
            gamma |= 1 << bitCount.size() - bitIndex - 1;
    }    
    std::bitset<32> y(gamma);
    std::cout << "Accumulator : " << y << std::endl;
    std::cout << "Gamma : " << gamma << std::endl;
    const uint_fast32_t epsilon = (~gamma & inverseRelevance);
    std::cout << "Epsilon : " << epsilon << std::endl;
    std::cout << "Answer : " << epsilon * gamma << std::endl; 
    
    std::cout << "Day 3" << std::endl;
    return 0;
}
