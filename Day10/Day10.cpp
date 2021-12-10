#include <iostream>
#include <fstream>
#include <stack>
#include <string>
#include <unordered_map>

#include "../Lib/CommonLib.h"

//const char* InputData = "TestInputData.txt";
const char* InputData = "MyInput.txt";


int64_t CalculateScoreOfLine(const std::string& InBuffer, std::unordered_map<char, std::pair<char, int32_t>>& InTags)
{
    std::stack<char> stack;
    stack.push(InBuffer[0]);

    int64_t score = 0;

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
                if (stack.top() != InTags[InBuffer[charIndex]].first) // This is an invalid line
                    return 0;
                stack.pop();
                break;
            }
        }
    }

    while (!stack.empty())
    {
        auto firstMatch = std::ranges::find_if(InTags, [character = stack.top()](const std::pair<char, std::pair<char, int32_t>>& InElement)
        {
            return InElement.second.first == character;
        });
        if (firstMatch == InTags.end())
            continue;
        //std::cout << firstMatch->first;
        int32_t pointForChar = firstMatch->second.second;
        score = (score * 5) + pointForChar;
        stack.pop();
    }
    //std::cout << " - " << score << " total points." << std::endl;
    
    return score;
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
    matchingCloseTags[')'] = std::pair('(', 1);
    matchingCloseTags[']'] = std::pair('[', 2);
    matchingCloseTags['}'] = std::pair('{', 3);
    matchingCloseTags['>'] = std::pair('<', 4);

    std::vector<int64_t> scores;
    while (std::getline(inputDataStream, buffer))
    {
        const int64_t score = CalculateScoreOfLine(buffer, matchingCloseTags);
        // A score of 0 is an invalid line
        if (score != 0)
            scores.push_back(score);
    }
    std::ranges::sort(scores);
    std::cout << "Middle score: " << scores[scores.size() / 2] << std::endl;
    return 0;
}
