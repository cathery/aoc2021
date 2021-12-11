#include <fstream>
#include <iostream>
#include <array>

int main()
{
	std::ifstream Input("input.txt");

	constexpr std::uintmax_t NumDays = 256;
	constexpr std::uint8_t FishSpawnInterval = 7;
	constexpr std::uint8_t NewFishSpawnInterval = 9;
	constexpr std::uint8_t MaxFishSpawnInterval = std::max(FishSpawnInterval, NewFishSpawnInterval) + 1;

	std::array<std::uintmax_t, MaxFishSpawnInterval> Fishes{};
	std::uintmax_t InputNumber;

	while (Input >> InputNumber)
	{
		Fishes[InputNumber]++;
		Input.ignore(1);
	}

	for (std::uintmax_t Day = 0; Day != NumDays; ++Day)
	{
		for (std::uintmax_t FishIndex = 0; FishIndex != Fishes.size(); ++FishIndex)
		{
			if (FishIndex == 0)
			{
				Fishes[FishSpawnInterval] += Fishes[FishIndex];
				Fishes[NewFishSpawnInterval] += Fishes[FishIndex];
			}
			else
			{
				Fishes[FishIndex - 1] = Fishes[FishIndex];
				Fishes[FishIndex] = 0;
			}
		
		}
	}

	std::uintmax_t SumTotal = 0;
	for (const auto& Fish : Fishes)
	{
		SumTotal += Fish;
	}

	std::cout << SumTotal << std::endl;
}