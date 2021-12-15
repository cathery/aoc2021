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

	std::size_t TileYSize = 0;
	std::size_t TileXSize = 0;

	char CharacterBuffer;

	while (Input >> CharacterBuffer)
	{
		Map.resize(TileYSize + 1);

		TileInfo NewTile;
		NewTile.RiskLevel = CharacterBuffer - '0';
		NewTile.PathRating = -1;

		Map[TileYSize].push_back(NewTile);

		if (Input.peek() == '\n')
		{
			TileXSize = std::max(TileXSize, Map[TileYSize].size());
			TileYSize++;
		}
	}

	// Copy the map over a 5x5 grid

	const std::size_t MaxYSize = TileYSize * 5;
	const std::size_t MaxXSize = TileXSize * 5;

	Map.resize(MaxYSize);

	for (auto& YMap : Map)
	{
		YMap.resize(MaxXSize);
	}

	for (std::size_t YMapIndex = 0; YMapIndex != 5; ++YMapIndex)
	{
		for (std::size_t XMapIndex = 0; XMapIndex != 5; ++XMapIndex)
		{
			if (YMapIndex == 0 && XMapIndex == 0)
			{
				continue;
			}

			for (std::size_t YIndex = 0; YIndex != TileYSize; ++YIndex)
			{
				const std::size_t NewYIndex = (YMapIndex * TileYSize) + YIndex;
				for (std::size_t XIndex = 0; XIndex != TileXSize; ++XIndex)
				{
					const std::size_t NewXIndex = (XMapIndex * TileXSize) + XIndex;

					TileInfo& NewPoint = Map[NewYIndex][NewXIndex];

					const TileInfo& OriginalPoint = Map[YIndex][XIndex];

					NewPoint.PathRating = -1;

					NewPoint.RiskLevel = (OriginalPoint.RiskLevel + YMapIndex + XMapIndex) % 9;

					if (NewPoint.RiskLevel == 0)
					{
						NewPoint.RiskLevel = 9;
					}
				}
			}
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