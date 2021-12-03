#include <bitset>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "../Lib/CommonLib.h"

const char* InputData = "TestInputData.txt";
//const char* InputData = "MyInput.txt";

int32_t CountSetInColumn(const std::vector<std::bitset<32>>& InRows, int InColumn)
{
    uint32_t count = 0;
    for (const auto& element : InRows)
    {
        if (element.test(InColumn))
            count++;
    }
    return count;
}

int_fast32_t FindRatings(std::vector<std::bitset<32>> InRows, size_t InNumBits, bool InCheckOxygenRating)
{
    for(int index = static_cast<int>(InNumBits) - 1; index >= 0; --index)
    {
        auto count = CountSetInColumn(InRows, index);
        auto isSet = InCheckOxygenRating ? count * 2 > InRows.size() : count * 2 < InRows.size();
        auto isEqual = count * 2 == InRows.size();
        std::erase_if(InRows, [InCheckOxygenRating, isSet, isEqual, index](std::bitset<32> InLine) // check rating
        {
            if (isEqual)
                return InLine.test(index) != InCheckOxygenRating;
            return InLine.test(index) != isSet;
        });
        if (InRows.size() == 1)
        {
            return InRows[0].to_ulong();
        }
        int breakHere = 2;
    }
    std::cerr << "Did not find one match, found [" << InRows.size() << "] matches" << std::endl;
    return 0;
}

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
    std::vector<std::bitset<32>> rows;
    uint32_t firstRow = 0;
    for(int32_t charIndex = static_cast<int32_t>(buffer.size()) - 1; charIndex >= 0 ; charIndex--)
    {
        const auto currentBit = buffer[charIndex];
        if (currentBit != '0' && currentBit != '1')
            continue;
        inverseRelevance |= 1 << bitCount.size();
        const bool isSet = currentBit == '1';
        if (isSet)
            firstRow |= 1 << bitCount.size();
        bitCount.insert(bitCount.begin(), isSet ? 1 : 0);
    }
    
    rows.push_back(firstRow); // First row
    
    int_fast32_t rowCount = 0;
    while (std::getline(inputDataStream, buffer))
    {
        rowCount += 1;
        uint32_t currentRow = 0;
        for(int32_t charIndex = static_cast<int32_t>(buffer.size()) - 1; charIndex >= 0 ; charIndex--)
        {
            if (buffer[charIndex] == '1')
            {
                bitCount[charIndex] += 1;
                currentRow |= 1 << (buffer.size() - charIndex - 1);
            }
        }
        rows.push_back(currentRow);
    }

    uint_fast32_t mostCommonColumns = 0;
    uint_fast32_t equalColumns = 0;
    const uint_fast32_t minCount = rowCount / 2 + 1;
    for(auto bitIndex = 0; bitIndex < bitCount.size(); ++bitIndex)
    {
        if (bitCount[bitIndex] > minCount)
            mostCommonColumns |= 1 << (bitCount.size() - bitIndex - 1);
        else if (bitCount[bitIndex] == minCount)
            equalColumns |= 1 << (bitCount.size() - bitIndex - 1);
    }

    std::bitset<32> y(mostCommonColumns);
    std::cout << "Accumulator : " << y << std::endl;
    std::cout << "Gamma : " << mostCommonColumns << std::endl;
    const uint_fast32_t epsilon = (~mostCommonColumns & inverseRelevance);
    std::cout << "Epsilon : " << epsilon << std::endl;

    const int_fast32_t oxygenRating = FindRatings(rows, bitCount.size(), true);
    const int_fast32_t co2ScrubberRating = FindRatings(rows, bitCount.size(), false);
    std::cout << "Oxygen generation rating: " << oxygenRating << std::endl;
    std::cout << "CO2 scrubbing rating: " << co2ScrubberRating << std::endl;
    std::cout << "Answer : " << oxygenRating * co2ScrubberRating << std::endl; 
    return 0;
}
