#include <fstream>
#include <iostream>
#include <vector>
#include <array>

static std::vector<std::vector<std::uint8_t>> Heightmap;

struct Basin_t
{
	std::size_t Size;
};

static Basin_t GetBasin(std::size_t YIndex, std::size_t XIndex)
{
	Basin_t CurrentBasin{};

	if (Heightmap[YIndex][XIndex] == 9)
	{
		return CurrentBasin;
	}

	// clear the height so it doesn't get counted twice
	Heightmap[YIndex][XIndex] = 9;
	CurrentBasin.Size++;

	if (YIndex != 0)
	{
		CurrentBasin.Size += GetBasin(YIndex - 1, XIndex).Size;
	}
	if (XIndex != 0)
	{
		CurrentBasin.Size += GetBasin(YIndex, XIndex - 1).Size;
	}
	if (YIndex != Heightmap.size() - 1)
	{
		CurrentBasin.Size += GetBasin(YIndex + 1, XIndex).Size;
	}
	if (XIndex != Heightmap[YIndex].size() - 1)
	{
		CurrentBasin.Size += GetBasin(YIndex, XIndex + 1).Size;
	}

	return CurrentBasin;
}

int main()
{
	std::ifstream Input("input.txt");
	char CharacterBuffer;

	std::size_t HeightIndex = 0;

	std::array<Basin_t, 3> LargestBasins{};

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

	auto* SmallestOfLargestBasins = &LargestBasins[0];
	std::vector<Basin_t> Basins;

	for (std::size_t YIndex = 0; YIndex != Heightmap.size(); ++YIndex)
	{
		for (std::size_t XIndex = 0; XIndex != Heightmap[YIndex].size(); ++XIndex)
		{
			const Basin_t Basin = GetBasin(YIndex, XIndex);

			// if basin size is large enough to earn a place in the largest basins, replace the smallest one with it
			if (Basin.Size > SmallestOfLargestBasins->Size)
			{
				*SmallestOfLargestBasins = Basin;

				// find the smallest of largest basins (we only need to do that when largest basins change)
				for (auto& LargestBasin : LargestBasins)
				{
					if (LargestBasin.Size < SmallestOfLargestBasins->Size)
					{
						SmallestOfLargestBasins = &LargestBasin;
					}
				}
			}
		}
	}

	std::uintmax_t ProductOfLargestBasins = 1;
	for (const auto& LargestBasin : LargestBasins)
	{
		const auto test = LargestBasin.Size;
		ProductOfLargestBasins *= LargestBasin.Size;
	}

	std::cout << ProductOfLargestBasins << std::endl;
}