#include <fstream>
#include <iostream>
#include <array>
#include <limits>

union Digit
{
	std::uint8_t Data;
	struct
	{
		std::uint8_t A : 1;
		std::uint8_t B : 1;
		std::uint8_t C : 1;
		std::uint8_t D : 1;
		std::uint8_t E : 1;
		std::uint8_t F : 1;
		std::uint8_t G : 1;
		std::uint8_t _ : 1;	// padding
	};

	constexpr std::uint8_t SegmentCount() const
	{
		std::uint8_t Value = Data;
		std::uint8_t Count = 0;
		for ( ; Value; Value >>=1)
		{
			Count += Value & 1;
		}
		return Count;
	}

	constexpr static std::size_t NumSegments = 7;
	constexpr static std::uint8_t Bitmask = (1 << NumSegments) - 1;
};
static_assert(sizeof(Digit) == 1);

constexpr static std::array<Digit, 10> Digits{
	0b1110111,	// 0
	0b0100100,	// 1
	0b1011101,	// 2
	0b1101101,	// 3
	0b0101110,	// 4
	0b1101011,	// 5
	0b1111011,	// 6
	0b0100101,	// 7
	0b1111111,	// 8
	0b1101111,	// 9
};


static constexpr Digit MakeDigit(const std::string_view StringData)
{
	Digit Out{};
	const std::size_t SegmentCount = StringData.size();
	for (std::size_t Index = 0; Index != SegmentCount; ++Index)
	{
		const std::uintmax_t Segment = StringData[Index] - 97;
		Out.Data |= (1 << Segment);
	}
	return Out;
}

static constexpr std::uint8_t GetNumberFromDigit(const Digit InDigit)
{
	//lazy
	for (std::size_t Index = 0; Index != Digits.size(); ++Index)
	{
		if (InDigit.Data == Digits[Index].Data)
		{
			return Index;
		}
	}
	return 0;
}

int main()
{
	std::ifstream Input("input.txt");

	std::string StringBuffer;

	std::uintmax_t OutputNumbersSum = 0;

	while (Input.good())
	{
		constexpr std::size_t NumSignalPatterns = 10;
		constexpr std::size_t NumOutputDigits = 4;

		std::array<Digit, NumSignalPatterns> SignalPatterns;
		std::array<Digit, NumOutputDigits> OutputDigits;

		for (std::size_t Index = 0; Index != NumSignalPatterns; ++Index)
		{
			Input >> StringBuffer;
			SignalPatterns[Index] = MakeDigit(StringBuffer);
		}

		Input.ignore(std::numeric_limits<std::streamsize>::max(), '|');

		for (std::size_t Index = 0; Index != NumOutputDigits; ++Index)
		{
			Input >> StringBuffer;
			OutputDigits[Index] = MakeDigit(StringBuffer);
		}

		// For some reason Input reads one line past the end
		if (!Input)
		{
			break;
		}

		// Decode the cipher

		std::array<Digit, 10> OrderedPatterns;

		// we can easily locate digits 1, 4, 7, and 8 because they have a unique segment count

		for (const auto& Pattern : SignalPatterns)
		{
			const std::size_t SegmentCount = Pattern.SegmentCount();
			if (SegmentCount == 2)
			{
				OrderedPatterns[1] = Pattern;
			}
			else if (SegmentCount == 3)
			{
				OrderedPatterns[7] = Pattern;
			}
			else if (SegmentCount == 4)
			{
				OrderedPatterns[4] = Pattern;
			}
			else if (SegmentCount == 7)
			{
				OrderedPatterns[8] = Pattern;
			}
		}

		// derive some signals from the digits we located

		const std::uint8_t TopMostSignal = OrderedPatterns[7].Data & ~(OrderedPatterns[1].Data);
		const std::uint8_t TopRightAndBottomRightSignals = OrderedPatterns[1].Data;
		const std::uint8_t TopLeftAndMiddleSignals = OrderedPatterns[4].Data & ( ~(OrderedPatterns[7].Data) & Digit::Bitmask );
		const std::uint8_t BottomLeftAndBottomMostSignals = ~(OrderedPatterns[1].Data | OrderedPatterns[7].Data | OrderedPatterns[4].Data) & Digit::Bitmask;

		// locate 0, 6, and 9

		for (const auto& Pattern : SignalPatterns)
		{
			if (Pattern.SegmentCount() == 6)
			{
				// could be 0, 6, or 9

				const std::uint8_t AlmostZeroMask = TopMostSignal | TopRightAndBottomRightSignals | BottomLeftAndBottomMostSignals;
				const std::uint8_t AlmostNineMask = TopMostSignal | TopRightAndBottomRightSignals | TopLeftAndMiddleSignals;

				if ((Pattern.Data & AlmostZeroMask) == AlmostZeroMask)
				{
					OrderedPatterns[0] = Pattern;
				}
				else if ((Pattern.Data & AlmostNineMask) == AlmostNineMask)
				{
					OrderedPatterns[9] = Pattern;
				}
				else
				{
					OrderedPatterns[6] = Pattern;
				}
			}
		}

		// derive more exact signals from the digits

		const std::uint8_t BottomRightSignal = TopRightAndBottomRightSignals & OrderedPatterns[6].Data;
		const std::uint8_t TopRightSignal = TopRightAndBottomRightSignals & ~BottomRightSignal;

		// locate 2, 3, and 5

		for (const auto& Pattern : SignalPatterns)
		{
			if (Pattern.SegmentCount() == 5)
			{
				// could be 2, 3, or 5

				const std::uint8_t AlmostTwoMask = TopMostSignal | TopRightSignal | BottomLeftAndBottomMostSignals;
				const std::uint8_t AlmostFiveMask = TopMostSignal | TopLeftAndMiddleSignals | BottomRightSignal;

				if ((Pattern.Data & AlmostTwoMask) == AlmostTwoMask)
				{
					OrderedPatterns[2] = Pattern;
				}
				else if ((Pattern.Data & AlmostFiveMask) == AlmostFiveMask)
				{
					OrderedPatterns[5] = Pattern;
				}
				else
				{
					OrderedPatterns[3] = Pattern;
				}
			}
		}

		// all of the digits have been located by now, we can decode the output

		std::uintmax_t OutputNumber = 0;

		for (std::size_t OutputIndex = 0; OutputIndex != OutputDigits.size(); ++OutputIndex)
		{
			for (std::size_t Index = 0; Index != OrderedPatterns.size(); ++Index)
			{
				if (OutputDigits[OutputIndex].Data == OrderedPatterns[Index].Data)
				{
					OutputNumber = (OutputNumber * 10) + Index;
					break;
				}
			}
		}

		OutputNumbersSum += OutputNumber;
	}

	std::cout << OutputNumbersSum << std::endl;
}