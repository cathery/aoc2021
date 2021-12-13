#include <fstream>
#include <iostream>
#include <vector>
#include <limits>
#include <cmath>

int main()
{
	std::ifstream Input("input.txt");

	std::size_t NumberBuffer;

	std::vector<std::size_t> YPositions;
	std::vector<std::size_t> XPositions;

	std::size_t MaxYIndex = 0;
	std::size_t MaxXIndex = 0;

	while (Input)
	{
		Input >> NumberBuffer;
		Input.ignore(1);

		MaxXIndex = std::max(MaxXIndex, NumberBuffer);
		XPositions.push_back(NumberBuffer);

		Input >> NumberBuffer;
		Input.ignore(1);

		MaxYIndex = std::max(MaxYIndex, NumberBuffer);
		YPositions.push_back(NumberBuffer);

		if (Input.peek() == '\n')
		{
			Input.ignore(1);
			break;
		}
	}

	std::size_t NumYFolds = 0;
	std::size_t NumXFolds = 0;

	while (Input.peek() != EOF)
	{
		Input.ignore(11);
		const bool IsYAxis = Input.get() == 'y';
		Input.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

		IsYAxis ? ++NumYFolds : ++NumXFolds;
	}

	const std::size_t YSize = (MaxYIndex + 1) / std::pow(2, NumYFolds);
	const std::size_t XSize = (MaxXIndex + 1) / std::pow(2, NumXFolds);

	std::vector<bool> DotMap;
	DotMap.resize(YSize * XSize);

	for (std::size_t Index = 0; Index != YPositions.size(); ++Index)
	{
		std::size_t YIndex = YPositions[Index];
		std::size_t XIndex = XPositions[Index];

		if ((YIndex / (YSize+1)) % 2 == 0)
		{
			// Y position shouldn't be mirrored
			YIndex = YIndex % (YSize+1);
		}
		else
		{
			// Y position should be mirrored
			YIndex = YSize - (YIndex % (YSize + 1)) - 1;
		}

		if ((XIndex / (XSize+1)) % 2 == 0)
		{
			// X position shouldn't be mirrored
			XIndex = XIndex % (XSize+1);
		}
		else
		{
			// X position should be mirrored
			XIndex = XSize - (XIndex % (XSize + 1)) - 1;
		}

		DotMap[YIndex * XSize + XIndex] = true;
	}

	for (std::size_t YIndex = 0; YIndex != YSize; ++YIndex)
	{
		for (std::size_t XIndex = 0; XIndex != XSize; ++XIndex)
		{
			std::cout << (DotMap[YIndex * XSize + XIndex] ? '#' : '.');
		}
		std::cout << std::endl;
	}
}