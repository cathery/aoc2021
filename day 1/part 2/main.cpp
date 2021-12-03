#include <fstream>
#include <iostream>

int main()
{
	std::ifstream Input("input.txt");

	std::int64_t FirstMeasurement;
	std::int64_t SecondMeasurement;
	std::int64_t ThirdMeasurement;

	std::int64_t CurrentHeight;

	std::uint64_t NumIncrements = 0;

	Input >> FirstMeasurement;
	Input >> SecondMeasurement;
	Input >> ThirdMeasurement;

	while (Input >> CurrentHeight)
	{
		const std::int64_t OldMeasurementSum = FirstMeasurement + SecondMeasurement + ThirdMeasurement;
		FirstMeasurement = SecondMeasurement;
		SecondMeasurement = ThirdMeasurement;
		ThirdMeasurement = CurrentHeight;

		const std::int64_t NewMeasurementSum = FirstMeasurement + SecondMeasurement + ThirdMeasurement;
		if (NewMeasurementSum > OldMeasurementSum)
		{
			NumIncrements++;
		}
	}

	std::cout << NumIncrements << std::endl;
}