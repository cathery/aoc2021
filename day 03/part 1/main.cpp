#include <fstream>
#include <iostream>
#include <string>
#include <vector>

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

	std::vector<std::uint64_t> CommonBits;
	CommonBits.resize(BitsNum);

	// Count the bits in each number
	for (auto Number : Numbers)
	{
		for (std::uint64_t Index = 0; Number; Number >>= 1)
		{
			auto test = CommonBits[Index];
			auto test2 = Number & 1;
			test += test2;
			CommonBits[Index] += Number & 1;
			Index++;
		}
	}

	const std::size_t NumbersNum = Numbers.size();
	const std::uint64_t Bitmask = (1 << BitsNum) - 1;

	std::uint64_t GammaRate = 0;

	// Reverse iterate through the common bits array and set the common bits to gamme and epsilon
	for (auto It = CommonBits.rbegin(); It != CommonBits.rend(); It++)
	{
		auto Bit = *It;

		// Not sure what should be done in case both bits are equally common
		bool MostCommonBit = Bit*2 >= NumbersNum;

		GammaRate <<= 1;
		GammaRate += MostCommonBit;
	}

	const std::uint64_t EpsilonRate = ~GammaRate & Bitmask;

	std::cout << "Gamma rate: " << GammaRate << std::endl;
	std::cout << "Epsilon rate: " << EpsilonRate << std::endl;
	std::cout << "Gamma multiplied by epsilon: " << GammaRate * EpsilonRate << std::endl;
}