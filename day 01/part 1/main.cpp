#include <fstream>
#include <iostream>

int main()
{
	std::ifstream Input("input.txt");

	std::int64_t CurrentHeight;
	std::int64_t LastHeight;

	std::uint64_t NumIncrements = 0;

	Input >> LastHeight;

	while (Input >> CurrentHeight)
	{
		if (CurrentHeight > LastHeight)
		{
			NumIncrements++;
		}
		LastHeight = CurrentHeight;
	}

	std::cout << NumIncrements << std::endl;
}