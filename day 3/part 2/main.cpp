#include <fstream>
#include <iostream>
#include <string>
#include <vector>

static std::vector<std::uint64_t> FindMostCommonBitNumbers(const std::vector<std::uint64_t>& Numbers, std::uint64_t BitIndex)
{
	std::uint64_t NumBits = 0;

	for (auto Number : Numbers)
	{
		auto test = 1 << BitIndex;
		auto test2 = Number & test;
		NumBits += (Number & (1 << BitIndex)) != 0;
	}

	const bool MostCommonBit  = NumBits * 2 >= Numbers.size();

	std::vector<std::uint64_t> OutMostCommonBitNumbers;

	for (auto Number : Numbers)
	{
		const bool CurrentBit = Number & (1 << BitIndex);
		if (CurrentBit == MostCommonBit)
		{
			OutMostCommonBitNumbers.push_back(Number);
		}
	}

	return OutMostCommonBitNumbers;
}

static std::vector<std::uint64_t> FindLeastCommonBitNumbers(const std::vector<std::uint64_t>& Numbers, std::uint64_t BitIndex)
{
	std::uint64_t NumBits = 0;

	for (auto Number : Numbers)
	{
		NumBits += (Number & (1 << BitIndex)) != 0;
	}

	const bool LeasstCommonBit  = NumBits * 2 < Numbers.size();

	std::vector<std::uint64_t> OutLeastCommonBitNumbers;

	for (auto Number : Numbers)
	{
		const bool CurrentBit = Number & (1 << BitIndex);
		if (CurrentBit == LeasstCommonBit)
		{
			OutLeastCommonBitNumbers.push_back(Number);
		}
	}

	return OutLeastCommonBitNumbers;
}

int main()
{
	std::ifstream Input("input.txt");

	std::uint64_t BitsNum;
	std::string StringBuffer;

	// Find the total number of bits
	Input >> StringBuffer;
	Input.clear();
	Input.seekg(0);
	BitsNum = StringBuffer.length();

	// We'll store all numbers
	std::vector<std::uint64_t> Numbers;

	// Read each line as a string and interpret it as a binary number
	while (Input >> StringBuffer)
	{
		const std::uint64_t Number = std::stoi(StringBuffer, nullptr, 2);
		Numbers.push_back(Number);
	}

	std::vector<std::uint64_t> MostCommonBitNumbers = Numbers;
	std::vector<std::uint64_t> LeastCommonBitNumbers = Numbers;

	std::uint64_t BitIndex = BitsNum - 1;

	do
	{
		if (MostCommonBitNumbers.size() > 1)
		{
			MostCommonBitNumbers = FindMostCommonBitNumbers(MostCommonBitNumbers, BitIndex);
		}
		
		if (LeastCommonBitNumbers.size() > 1)
		{
			LeastCommonBitNumbers = FindLeastCommonBitNumbers(LeastCommonBitNumbers, BitIndex);
		}
	} while (BitIndex --> 0);

	const std::uint64_t OxygenGeneratorRating = MostCommonBitNumbers[0];
	const std::uint64_t CO2ScrubberRating = LeastCommonBitNumbers[0];
	const std::uint64_t LifeSupportRating = OxygenGeneratorRating * CO2ScrubberRating;

	std::cout << "Oxygen generator rating: " << OxygenGeneratorRating << std::endl;
	std::cout << "CO2 scrubber rating: " << CO2ScrubberRating << std::endl;
	std::cout << "Life support rating: " << LifeSupportRating << std::endl;
}