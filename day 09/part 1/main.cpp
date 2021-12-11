#include <fstream>
#include <iostream>
#include <vector>

int main()
{
	std::ifstream Input("input.txt");
	char CharacterBuffer;

	std::vector<std::vector<std::uint8_t>> Heightmap;
	std::size_t HeightIndex = 0;
	std::uintmax_t SumOfRiskLevels = 0;

	// ifstream skips newline characters by default, turn that off
	Input >> std::noskipws;

	while (Input >> CharacterBuffer)
	{
		// Read from file digit by digit, move one line down with each newline
		if (CharacterBuffer != '\n')
		{
			if (HeightIndex + 1 > Heightmap.size())
			{
				Heightmap.resize(HeightIndex + 1);
			}
			Heightmap[HeightIndex].push_back(CharacterBuffer - '0');
		}
		else
		{
			HeightIndex++;
		}
	}

	for (std::size_t YIndex = 0; YIndex != Heightmap.size(); ++YIndex)
	{
		for (std::size_t XIndex = 0; XIndex != Heightmap[YIndex].size(); ++XIndex)
		{
			const std::uint8_t CurrentPoint = Heightmap[YIndex][XIndex];

			// 9 can never be a low point
			if (CurrentPoint == 9)
			{
				continue;
			}

			// Check top
			if ((YIndex != 0)
				&& Heightmap[YIndex - 1][XIndex] <= CurrentPoint)
			{
				continue;
			}

			// Check left
			if ((XIndex != 0)
				&& Heightmap[YIndex][XIndex - 1] <= CurrentPoint)
			{
				continue;
			}

			// Check bottom
			if ((YIndex != Heightmap.size() - 1)
				&& Heightmap[YIndex + 1][XIndex] <= CurrentPoint)
			{
				continue;
			}

			// Check right
			if ((XIndex != Heightmap[YIndex].size() - 1)
				&& Heightmap[YIndex][XIndex + 1] <= CurrentPoint)
			{
				continue;
			}

			// if we're here then this is a low point
			SumOfRiskLevels += CurrentPoint + 1;
		}
	}

	std::cout << SumOfRiskLevels << std::endl;
}