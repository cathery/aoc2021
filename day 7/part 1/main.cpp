#include <fstream>
#include <iostream>
#include <vector>
#include <limits>

int main()
{
	std::ifstream Input("input.txt");

	std::vector<std::uintmax_t> CrabPositions;

	std::uintmax_t Number;
	while (Input >> Number)
	{
		CrabPositions.push_back(Number);
		Input.ignore(1);
	}

	std::vector<std::uintmax_t> PathfindingMap;

	for (const auto& CrabPosition : CrabPositions)
	{
		const std::uintmax_t NewSize = std::max(CrabPosition + 1, PathfindingMap.size());
		PathfindingMap.resize(NewSize);

		for (std::uintmax_t MapIndex = 0; MapIndex != NewSize; ++MapIndex)
		{
			PathfindingMap[MapIndex] += MapIndex > CrabPosition ? MapIndex - CrabPosition : CrabPosition - MapIndex;
		}
	}

	std::uintmax_t LowestFuelCount = std::numeric_limits<std::uintmax_t>::max();
	for (const auto& CurrentPositionFuelCount : PathfindingMap)
	{
		if (CurrentPositionFuelCount < LowestFuelCount)
		{
			LowestFuelCount = CurrentPositionFuelCount;
		}
	}


	std::cout << LowestFuelCount << std::endl;
}