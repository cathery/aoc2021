#include <fstream>
#include <iostream>
#include <vector>

int main()
{
	std::ifstream Input("input.txt");

	std::size_t NumberBuffer;

	std::vector<std::size_t> YPositions;
	std::vector<std::size_t> XPositions;

	std::size_t MaxYIndex = 0;
	std::size_t MaxXIndex = 0;

	while (Input)
	{
		Input >> NumberBuffer;
		Input.ignore(1);

		MaxXIndex = std::max(MaxXIndex, NumberBuffer);
		XPositions.push_back(NumberBuffer);

		Input >> NumberBuffer;
		Input.ignore(1);

		MaxYIndex = std::max(MaxYIndex, NumberBuffer);
		YPositions.push_back(NumberBuffer);

		if (Input.peek() == '\n')
		{
			break;
		}
	}

	Input.ignore(12);

	// Real input always starts with X but for the sake of keeping it compatible with the test input we'll keep it a condition
	const bool IsYAxis = Input.get() == 'y';

	// Fold line always happens to be halfway across the axis
	const std::size_t FoldLinePosition = (IsYAxis ? MaxYIndex : MaxXIndex) / 2;

	const std::size_t SmallerAxisMaxSize = (IsYAxis ? MaxXIndex : MaxYIndex) + 1;

	const auto& BiggerAxis = IsYAxis ? YPositions : XPositions;
	const auto& SmallerAxis = IsYAxis ? XPositions : YPositions;

	std::vector<bool> DotMap;
	DotMap.resize(FoldLinePosition * SmallerAxisMaxSize);

	std::size_t TotalDots = 0;

	for (std::size_t Index = 0; Index != YPositions.size(); ++Index)
	{
		std::size_t BiggerIndex = BiggerAxis[Index];
		const std::size_t SmallerIndex = SmallerAxis[Index];

		// If we cross the fold line, mirror the index
		if (BiggerIndex > FoldLinePosition)
		{
			BiggerIndex = FoldLinePosition*2 - BiggerIndex;
		}

		const std::size_t ActualIndex = BiggerIndex * SmallerAxisMaxSize + SmallerIndex;

		TotalDots += !DotMap[ActualIndex];
		DotMap[ActualIndex] = true;
	}

	std::cout << TotalDots << std::endl;
}