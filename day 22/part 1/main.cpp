#include <fstream>
#include <iostream>
#include <vector>

int main()
{
	std::ifstream Input("input.txt");

	constexpr std::intmax_t MinimumPosition = -50;
	constexpr std::intmax_t MaximumPosition = 50;
	constexpr std::size_t PositionRange = MaximumPosition - MinimumPosition + 1;

	std::vector<std::vector<std::vector<bool>>> ZYXGrid;

	// Z
	ZYXGrid.resize(PositionRange);
	for (auto& YXGrid : ZYXGrid)
	{
		// Y
		YXGrid.resize(PositionRange);
		for (auto& XGrid : YXGrid)
		{
			// X
			XGrid.resize(PositionRange);
		}
	}

	while (Input.peek() != EOF)
	{
		std::intmax_t BeginX, EndX, BeginY, EndY, BeginZ, EndZ;

		Input.ignore(1);
		bool bTurnCubesOn = Input.get() == 'n';
		Input.ignore(3 + !bTurnCubesOn);
		Input >> BeginX;
		Input.ignore(2);
		Input >> EndX;
		Input.ignore(3);
		Input >> BeginY;
		Input.ignore(2);
		Input >> EndY;
		Input.ignore(3);
		Input >> BeginZ;
		Input.ignore(2);
		Input >> EndZ;
		Input.ignore(1);

		if (EndX < BeginX)
		{
			std::swap(BeginX, EndX);
		}

		if (EndY < BeginY)
		{
			std::swap(BeginY, EndY);
		}

		if (EndZ < BeginZ)
		{
			std::swap(BeginZ, EndZ);
		}

		for (std::intmax_t ZIndex = std::max(BeginZ, MinimumPosition); ZIndex <= std::min(EndZ, MaximumPosition); ++ZIndex)
		{
			for (std::intmax_t YIndex = std::max(BeginY, MinimumPosition); YIndex <= std::min(EndY, MaximumPosition); ++YIndex)
			{
				for (std::intmax_t XIndex = std::max(BeginX, MinimumPosition); XIndex <= std::min(EndX, MaximumPosition); ++XIndex)
				{
					ZYXGrid[ZIndex - MinimumPosition][YIndex - MinimumPosition][XIndex - MinimumPosition] = bTurnCubesOn;
				}
			}
		}
	}


	std::size_t EnabledCubeCount = 0;
	for (const auto& YXGrid : ZYXGrid)
	{
		for (const auto& XGrid : YXGrid)
		{
			for (const auto& XGrid : XGrid)
			{
				EnabledCubeCount += XGrid;
			}
		}
	}
	std::cout << EnabledCubeCount << std::endl;
}