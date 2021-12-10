#include <iostream>
#include <fstream>
#include <stack>
#include <string>
#include <unordered_map>

#include "../Lib/CommonLib.h"

//const char* InputData = "TestInputData.txt";
const char* InputData = "MyInput.txt";


int32_t CalculateScoreOfLine(const std::string& InBuffer, std::unordered_map<char, std::pair<char, int32_t>>& InTags)
{
    std::stack<char> stack;
    stack.push(InBuffer[0]);

    for (int charIndex = 1; charIndex < InBuffer.length(); ++charIndex)
    {
        switch (InBuffer[charIndex])
        {
        case '(':
        case '[':
        case '{':
        case '<':
            {
                stack.push(InBuffer[charIndex]);
                break;
            }
        case ')':
        case ']':
        case '}':
        case '>':
            {
                if (stack.top() != InTags[InBuffer[charIndex]].first)
                    return InTags[InBuffer[charIndex]].second;
                stack.pop();
                break;
            }
        }
    }
    return 0;
}

int main(int argc, char* argv[])
{
    Timer fullTime("Day10 full time");

    std::ifstream inputDataStream;
    inputDataStream.open(InputData);
    
    if (!inputDataStream.is_open())
    {
        std::cerr << "Incorrect path to input data" << std::endl;
        return 1;
    }

    std::string buffer;
    std::unordered_map<char, std::pair<char, int32_t>> matchingCloseTags;
    matchingCloseTags[')'] = std::pair('(', 3);
    matchingCloseTags[']'] = std::pair('[', 57);
    matchingCloseTags['}'] = std::pair('{', 1197);
    matchingCloseTags['>'] = std::pair('<', 25137);

    int32_t score = 0;
    while (std::getline(inputDataStream, buffer))
    {
        score += CalculateScoreOfLine(buffer, matchingCloseTags);
    }
    std::cout << "Score: " << score << std::endl;
    return 0;
}
