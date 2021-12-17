#include <fstream>
#include <iostream>

int main()
{
	std::ifstream Input("input.txt");

	if (!Input)
	{
		abort();
	}

	std::intmax_t IgnoreBuffer;

	std::intmax_t BeginY;
	std::intmax_t EndY;

	Input.ignore(15);
	Input >> IgnoreBuffer;
	Input.ignore(2);
	Input >> IgnoreBuffer;
	Input.ignore(4);
	Input >> BeginY;
	Input.ignore(2);
	Input >> EndY;

	const std::intmax_t FurthestY = std::abs(BeginY) > std::abs(EndY) ? BeginY : EndY;

	std::cout << FurthestY * (FurthestY + 1) / 2 << std::endl;
}