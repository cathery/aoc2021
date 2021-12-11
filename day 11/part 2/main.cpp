#include <fstream>
#include <iostream>
#include <vector>

constexpr std::size_t YSize = 10;
constexpr std::size_t XSize = 10;

static std::vector<std::uint8_t> OctopusMap;

void ProcessOctopus(std::size_t YIndex, std::size_t XIndex)
{
	auto& Octopus = OctopusMap[YIndex * YSize + XIndex];

	Octopus++;

	if (Octopus == 10)
	{
		for (std::int8_t NewYIndex = YIndex - 1; NewYIndex != YIndex + 2; ++NewYIndex)
		{
			if (NewYIndex != -1 && NewYIndex != YSize)
			{
				for (std::int8_t NewXIndex = XIndex - 1; NewXIndex != XIndex + 2; ++NewXIndex)
				{
					if (NewXIndex != -1 && NewXIndex != XSize)
					{
						if (NewYIndex != YIndex || NewXIndex != XIndex)
						{
							ProcessOctopus(NewYIndex, NewXIndex);
						}
					}
				}
			}
		}
	}
}

int main()
{
	std::ifstream Input("input.txt");
	char CharacterBuffer;

	while (Input >> CharacterBuffer)
	{
		OctopusMap.push_back(CharacterBuffer - '0');
	}

	for (std::uintmax_t Step = 1; ; ++Step)
	{
		for (std::size_t Index = 0; Index != OctopusMap.size(); ++Index)
		{
			ProcessOctopus(Index / YSize, Index % YSize);
		}

		bool AreAllFlashed = true;
		for (std::size_t Index = 0; Index != OctopusMap.size(); ++Index)
		{
			auto& Octopus = OctopusMap[Index];
			if (Octopus > 9)
			{
				Octopus = 0;
			}
			else
			{
				AreAllFlashed = false;
			}
		}

		if (AreAllFlashed)
		{
			std::cout << "The board synced up at step " << Step << std::endl;
			break;
		}
	}
}