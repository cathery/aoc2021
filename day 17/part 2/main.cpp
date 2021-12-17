#include <fstream>
#include <iostream>

int main()
{
	std::ifstream Input("input.txt");

	if (!Input)
	{
		abort();
	}

	std::intmax_t BeginX;
	std::intmax_t EndX;

	std::intmax_t BeginY;
	std::intmax_t EndY;

	Input.ignore(15);
	Input >> BeginX;
	Input.ignore(2);
	Input >> EndX;
	Input.ignore(4);
	Input >> BeginY;
	Input.ignore(2);
	Input >> EndY;

	if (EndX < BeginX)
	{
		std::swap(BeginX, EndX);
	}

	if (EndY < BeginY)
	{
		std::swap(BeginY, EndY);
	}

	std::size_t NumValidVelocities = 0;

	// There are ways to calculate the minimum and maximum starting velocity values, and the maximum possible step,
	// but at the end of the day it's simpler to just pick a large constant

	for (std::intmax_t StartVelocityX = -300; StartVelocityX != 300; ++StartVelocityX)
	{
		for (std::intmax_t StartVelocityY = -300; StartVelocityY != 300; ++StartVelocityY)
		{
			std::intmax_t PositionX = 0;
			std::intmax_t PositionY = 0;

			std::intmax_t VelocityX = StartVelocityX;
			std::intmax_t VelocityY = StartVelocityY;


			for (std::size_t Step = 0; Step != 300; ++Step)
			{
				PositionX += VelocityX;
				PositionY += VelocityY;

				if (VelocityX != 0)
				{
					VelocityX += ((VelocityX > 0) ? -1 : 1);
				}

				VelocityY += -1;

				if (PositionX >= BeginX && PositionX <= EndX && PositionY >= BeginY && PositionY <= EndY)
				{
					NumValidVelocities++;
					break;
				}
			}
		}
	}

	std::cout << NumValidVelocities << std::endl;
}