#include <fstream>
#include <iostream>
#include <array>
#include <limits>

int main()
{
	std::ifstream Input("input.txt");

	std::string StringBuffer;

	std::uintmax_t SumCount = 0;

	while (Input)
	{
		constexpr std::size_t NumSignalPatterns = 10;
		constexpr std::size_t NumOutputDigits = 4;

		for (std::size_t Index = 0; Index != NumSignalPatterns; ++Index)
		{
			Input >> StringBuffer;
		}

		Input.ignore(std::numeric_limits<std::streamsize>::max(), '|');

		for (std::size_t Index = 0; Index != NumOutputDigits; ++Index)
		{
			Input >> StringBuffer;
			const std::size_t SegmentCount = StringBuffer.size();
			if (SegmentCount == 2 || SegmentCount == 3 || SegmentCount == 4 || SegmentCount == 7)
			{
				SumCount++;
			}
		}
	}
	std::cout << SumCount << std::endl;
}