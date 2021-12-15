#include <fstream>
#include <iostream>
#include <vector>

struct TileInfo
{
	std::uint8_t RiskLevel;
	std::size_t PathRating;
};

int main()
{
	std::ifstream Input("input.txt");

	std::vector<std::vector<TileInfo>> Map;

	std::size_t MaxYSize = 0;
	std::size_t MaxXSize = 0;

	char CharacterBuffer;

	while (Input >> CharacterBuffer)
	{
		Map.resize(MaxYSize + 1);

		TileInfo NewTile;
		NewTile.RiskLevel = CharacterBuffer - '0';
		NewTile.PathRating = -1;

		Map[MaxYSize].push_back(NewTile);

		if (Input.peek() == '\n')
		{
			MaxXSize = std::max(MaxXSize, Map[MaxYSize].size());
			MaxYSize++;
		}
	}

	// Map traversing algorithm

	Map[0][0].PathRating = 0;

	for (std::size_t YIndex = 0; YIndex != MaxYSize; ++YIndex)
	{
		for (std::size_t XIndex = 0; XIndex != MaxXSize; ++XIndex)
		{
			const TileInfo& CurrentPoint = Map[YIndex][XIndex];

			if (YIndex != 0)
			{
				TileInfo& UpPoint = Map[YIndex - 1][XIndex];
				const std::size_t NewRating = CurrentPoint.PathRating + UpPoint.RiskLevel;
				if (NewRating < UpPoint.PathRating)
				{
					UpPoint.PathRating = NewRating;
					YIndex--;
					XIndex--;
					continue;
				}
			}

			if (XIndex != 0)
			{
				TileInfo& LeftPoint = Map[YIndex][XIndex - 1];
				const std::size_t NewRating = CurrentPoint.PathRating + LeftPoint.RiskLevel;
				if (NewRating < LeftPoint.PathRating)
				{
					XIndex -= 2;
					LeftPoint.PathRating = NewRating;
					continue;
				}
			}

			if (YIndex != MaxYSize - 1)
			{
				TileInfo& DownPoint = Map[YIndex + 1][XIndex];
				DownPoint.PathRating = std::min(DownPoint.PathRating, CurrentPoint.PathRating + DownPoint.RiskLevel);
			}

			if (XIndex != MaxXSize - 1)
			{
				TileInfo& RightPoint = Map[YIndex][XIndex + 1];
				RightPoint.PathRating = std::min(RightPoint.PathRating, CurrentPoint.PathRating + RightPoint.RiskLevel);
			}
		}
	}

	TileInfo& EndTile = Map[MaxYSize -1][MaxXSize - 1];

	std::cout << EndTile.PathRating << std::endl;
}