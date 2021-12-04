#include <array>
#include <iostream>
#include <fstream>
#include <regex>

#include "../Lib/CommonLib.h"

//const char* InputData = "TestInputData.txt";
const char* InputData = "MyInput.txt";

using BingoRow = std::array<int, 5>;

class BingoBoard
{
public:
    void AddRow(BingoRow InNewRow)
    {
        Numbers.push_back(InNewRow);
    }

    bool IsFull() const
    {
        return Numbers.size() == 5;
    }

    const std::vector<BingoRow>& GetNumbers() const
    {
        return Numbers;
    }

    bool IsNumberSet(int InNumber) const
    {
        return std::ranges::find(MatchingNumbersFound, InNumber)
                    != MatchingNumbersFound.end();
    }

    bool HasHorizontalBingo() const
    {
        for (int row = 0; row < Numbers.size(); ++row)
        {
            for (int col = 0; col < Numbers[row].size(); ++col)
            {
                if (!IsNumberSet(Numbers[row][col]))
                    break;

                if (col == 4)
                    return true;
            }
        }
        return false;
    }

    bool HasVerticalBingo() const
    {
        for (int col = 0; col < Numbers[0].size(); ++col)
        {
            for (int row = 0; row < Numbers.size(); ++row)
            {
                if (!IsNumberSet(Numbers[row][col]))
                    break;

                if (row == 4)
                    return true;
            }
        }
        return false;
    }

    bool HasBingo() const
    {
        return HasHorizontalBingo() || HasVerticalBingo();
    }

    bool DrawNumber(int InNewNumber)
    {
        MatchingNumbersFound.push_back(InNewNumber);
        return HasBingo();
    }
    void CalculateAndShowAnswer(int32_t InLastNumberDrawn) const
    {
        int numberTotal = 0;
        for (auto row : Numbers)
        {
            for (auto number : row)
            {
                if (!IsNumberSet(number))
                    numberTotal += number;
            }
        }
        std::cout << "Sum of unmarked numbers: " << numberTotal << std::endl;
        std::cout << "Answer: " << numberTotal * InLastNumberDrawn << std::endl;
    }

protected:
    std::vector<BingoRow> Numbers;
    std::vector<int> MatchingNumbersFound;
};

std::ostream& operator<<(std::ostream& OutStream, const BingoBoard& InBoard)
{
    OutStream << "Bingo board" << std::endl;
    for (const BingoRow& row : InBoard.GetNumbers())
    {
        for (auto number : row)
        {
            OutStream << number << " ";
        }
        OutStream << std::endl;
    }
    return OutStream;
}

int main(int argc, char* argv[])
{
    Timer fullRuntime("Full runtime");

    std::ifstream inputDataStream;
    inputDataStream.open(InputData);
    
    if (!inputDataStream.is_open())
    {
        std::cerr << "Incorrect path to input data" << std::endl;
        return 1;
    }

    std::string firstRow;
    std::getline(inputDataStream, firstRow);

    std::stringstream ss(firstRow);
    std::string buffer;
    std::vector<int32_t> numbersToDraw;
    std::smatch regexMatch;
    while (std::getline(ss, buffer, ','))
    {
        numbersToDraw.push_back(std::atoi(buffer.c_str()));
    }

    std::string bingoLine = "(\\d{1,3})\\s*(\\d{1,3})\\s*(\\d{1,3})\\s*(\\d{1,3})\\s*(\\d{1,3})\\s*";

    std::regex fieldData(bingoLine, std::regex_constants::ECMAScript);
    ss.clear();
    std::shared_ptr<BingoBoard> currentBingoBoard;
    std::vector<std::shared_ptr<BingoBoard>> bingoBoards;
    while (std::getline(inputDataStream, buffer))
    {
        if (buffer.size() < 4)
            continue;

        if (!std::regex_search(buffer, regexMatch, fieldData))
            continue;

        if (currentBingoBoard == nullptr || currentBingoBoard->IsFull())
        {
            currentBingoBoard = std::make_shared<BingoBoard>();
            bingoBoards.push_back(currentBingoBoard);
        }
        currentBingoBoard->AddRow(
        {
            std::atoi(regexMatch[1].str().c_str()),
            std::atoi(regexMatch[2].str().c_str()),
            std::atoi(regexMatch[3].str().c_str()),
            std::atoi(regexMatch[4].str().c_str()),
            std::atoi(regexMatch[5].str().c_str())
        });
    }

    for (int32_t numberToDraw : numbersToDraw)
    {
        for (auto board : bingoBoards)
        {
            if (board->DrawNumber(numberToDraw))
            {
                board->CalculateAndShowAnswer(numberToDraw);
                return 0;
            }
        }
    }

    std::cerr << "No matching board found!" << std::endl;
    return 1;
}
