#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <array>

struct BingoNumber
{
	std::uint64_t Number;
	bool IsMarked;
};

template <std::uint64_t SizeX, std::uint64_t SizeY>
class BingoBoard
{
public:
	void MarkNumber(std::uint64_t Number)
	{
		for (auto& BoardY : Board)
		{
			for (auto& BoardX : BoardY)
			{
				if (BoardX.Number == Number)
				{
					BoardX.IsMarked = true;
				}
			}
		}
	}

	std::uint64_t GetSumOfUnmarkedNumbers()
	{
		std::uint64_t TotalSum = 0;
		for (auto& BoardY : Board)
		{
			for (auto& BoardX : BoardY)
			{
				if (!BoardX.IsMarked)
				{
					TotalSum += BoardX.Number;
				}
			}
		}
		return TotalSum;
	}

	bool IsWon()
	{
		bool IsLineComplete;

		// check rows
		for (auto& BoardY : Board)
		{
			IsLineComplete = true;

			for (auto& BoardX : BoardY)
			{
				if (!BoardX.IsMarked)
				{
					IsLineComplete = false;
					break;
				}
			}

			if (IsLineComplete)
			{
				return true;
			}
		}

		// check columns (stinky)
		for (std::uint64_t XIndex = 0; XIndex != SizeX; XIndex++)
		{
			IsLineComplete = true;

			for (const auto& BoardY : Board)
			{
				if (!BoardY[XIndex].IsMarked)
				{
					IsLineComplete = false;
					break;
				}
			}

			if (IsLineComplete)
			{
				return true;
			}
		}

		return false;
	}

	friend std::ifstream& operator>> (std::ifstream& Input, BingoBoard& Value)
	{
		for (auto& BoardY : Value.Board)
		{
			for (auto& BoardX : BoardY)
			{
				Input >> BoardX.Number;
				BoardX.IsMarked = false;
			}
		}
		return Input;
	}

private:
	// Just in case, let's organize numbers by rows
	std::array<std::array<BingoNumber, SizeX>, SizeY> Board;
};

int main()
{
	std::ifstream Input("input.txt");

	std::vector<std::uint64_t> DrawnNumbers;

	std::string StringBuffer;

	// Get the entire first line in a stringstream
	std::getline(Input, StringBuffer);
	std::stringstream StringStreamBuffer(StringBuffer);

	// Separate the line into numbers
	while (std::getline(StringStreamBuffer, StringBuffer, ','))
	{
		const std::uint64_t Number = std::stoi(StringBuffer);
		DrawnNumbers.push_back(Number);
	}

	// Read all of the bingo boards
	using BingoBoard5x5 = BingoBoard<5,5>;
	std::vector<BingoBoard5x5> Boards;
	BingoBoard5x5 BoardBuffer;

	while(Input >> BoardBuffer)
	{
		Boards.push_back(BoardBuffer);
	}

	std::uint64_t WinningScore = 0;
	bool IsGameFinished = false;

	// All of the data has been processed, let's play bingo
	for (const auto& DrawnNumber : DrawnNumbers)
	{
		for (auto& Board : Boards)
		{
			Board.MarkNumber(DrawnNumber);

			if (Board.IsWon())
			{
				WinningScore = Board.GetSumOfUnmarkedNumbers() * DrawnNumber;
				IsGameFinished = true;
				break;
			}
		}

		if (IsGameFinished)
		{
			break;
		}
	}

	std::cout << "Winning score: " << WinningScore << std::endl;
}