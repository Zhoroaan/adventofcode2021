#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>

#include "../Lib/CommonLib.h"

using CaveConnectionList =  std::unordered_map<std::string, std::vector<std::string>>;

const char* InputData = "TestInputData.txt";
// const char* InputData = "MyInput.txt";

bool IsAllUppercase(std::string& InsString)
{
    for (const char element : InsString)
    {
        if (!std::isupper(element))
            return false;
    }
    return true;
}

int32_t CalcNumberOfFoundPaths(const CaveConnectionList& InCaveConnections, std::vector<std::string> InCurrentPath)
{
    if (InCurrentPath.size() > 100)
        return 0;
    
    std::string currentNode = InCurrentPath.back();
    int32_t pathCount = 0;

    bool isDone = false;
    for (std::string connectingNode : InCaveConnections.at(currentNode))
    {
        if (connectingNode == "end")
        {
            isDone = true;
           continue;
        }
        
        const bool nodeVisitedBefore = std::ranges::find(InCurrentPath, connectingNode) != InCurrentPath.end();
        const bool nodeMultiVisit = IsAllUppercase(connectingNode);
        if (nodeVisitedBefore && !nodeMultiVisit)
            continue;

        std::vector newPath(InCurrentPath);
        newPath.push_back(connectingNode);
        pathCount += CalcNumberOfFoundPaths(InCaveConnections, newPath);
    }
    if (isDone)
    {
        /*for (auto& node : InCurrentPath)
        {
            std::cout << node << ',';
        }
        std::cout << "end" << std::endl;*/
        pathCount += 1;
    }
    return pathCount;
}

int main(int argc, char* argv[])
{
    Timer fullTime("Day12 full time");

    std::ifstream inputDataStream;
    inputDataStream.open(InputData);
    
    if (!inputDataStream.is_open())
    {
        std::cerr << "Incorrect path to input data" << std::endl;
        return 1;
    }

    std::string buffer;

   CaveConnectionList caveConnections;
    
    while (std::getline(inputDataStream, buffer))
    {
        auto t = buffer.find_first_of('-');
        std::string from(buffer.c_str(), t);
        std::string to(buffer.c_str() + t + 1, buffer.size() - t - 1);
        caveConnections[from].push_back(to);
        caveConnections[to].push_back(from);
    }

    int32_t numPathsFound = CalcNumberOfFoundPaths(caveConnections, {"start"});

    std::cout << "Num paths found: " << numPathsFound << std::endl;
    
    return 0;
}
