#include <fstream>
#include <iostream>
#include <vector>

enum class EPacketTypeID : std::uint8_t
{
	LiteralValue = 4,
};

enum class ELengthTypeID : std::uint8_t
{
	BitsLength = 0,
	SubpacketsLength = 1,
};

class Packet
{
public:
	std::vector<bool> Data;
	std::size_t Index = 0;
	std::size_t VersionNumbersSum = 0;

	std::size_t ExtractBits(std::size_t BitsCount)
	{
		std::size_t Result = 0;
		for (std::size_t CurrentIndex = 0; CurrentIndex != BitsCount; ++CurrentIndex)
		{
			Result |= (Data[Index + CurrentIndex] == 1) << (BitsCount - CurrentIndex - 1);
		}
		Index += BitsCount;
		return Result;
	}

	void ParsePacket()
	{
		const std::size_t VersionNumber = ExtractBits(3);
		const EPacketTypeID TypeID = static_cast<EPacketTypeID>(ExtractBits(3));

		VersionNumbersSum += VersionNumber;

		switch (TypeID)
		{
		case EPacketTypeID::LiteralValue:
			{
				while (true)
				{
					bool IsFinalBit = ExtractBits(1) != 1;
					ExtractBits(4);
					if (IsFinalBit)
					{
						break;
					}
				}
			}
			break;
		default:
			{
				ELengthTypeID LengthType = static_cast<ELengthTypeID>(ExtractBits(1));
				switch (LengthType)
				{
				case ELengthTypeID::BitsLength:
					{
						const std::size_t BitsLength = ExtractBits(15);
						const std::size_t BitsEnd = Index + BitsLength;

						while (Index < BitsEnd)
						{
							ParsePacket();
						}
					}
					break;
				case ELengthTypeID::SubpacketsLength:
					{
						std::size_t SubpacketsLength = ExtractBits(11);

						while (SubpacketsLength --> 0)
						{
							ParsePacket();
						}
					}
					break;
				default:
					abort();
				}
			}
			break;
		}
	}
};

int main()
{
	std::ifstream Input("input.txt");

	char CharacterBuffer;

	Packet NewPacket;

	while (Input >> CharacterBuffer)
	{
		std::uint8_t Value = 0;
		if (CharacterBuffer >= '0' && CharacterBuffer <= '9')
		{
			Value = CharacterBuffer - '0';
		}
		else if (CharacterBuffer >= 'A' && CharacterBuffer <= 'F')
		{
			Value = (CharacterBuffer - 'A') + 10;
		}

		for (std::size_t Index = 0; Index != 4; ++Index)
		{
			NewPacket.Data.push_back( Value & (1 << (4 - Index - 1) ) );
		}
	}

	NewPacket.ParsePacket();

	std::cout << NewPacket.VersionNumbersSum << std::endl;
}