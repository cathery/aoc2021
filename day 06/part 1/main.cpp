#include <fstream>
#include <iostream>
#include <vector>

int main()
{
	std::ifstream Input("input.txt");

	constexpr std::uintmax_t NumDays = 80;
	constexpr std::uint8_t FishSpawnInterval = 7;
	constexpr std::uint8_t NewFishSpawnInterval = 9;

	std::vector<std::uint8_t> Fishes;
	std::uintmax_t InputNumber;

	while (Input >> InputNumber)
	{
		Fishes.push_back(InputNumber);
		Input.ignore(1);
	}

	for (std::uintmax_t Day = 0; Day != NumDays; ++Day)
	{
		for (std::uintmax_t FishIndex = 0; FishIndex != Fishes.size(); ++FishIndex)
		{
			auto& Fish = Fishes[FishIndex];
			if (Fish == 0)
			{
				Fish = FishSpawnInterval;
				Fishes.push_back(NewFishSpawnInterval);
			}

			Fishes[FishIndex]--;
		}
	}

	std::cout << Fishes.size() << std::endl;
}