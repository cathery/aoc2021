#include <fstream>
#include <iostream>
#include <vector>

enum class EPacketTypeID : std::uint8_t
{
	Sum = 0,
	Product = 1,
	Minimum = 2,
	Maximum = 3,
	LiteralValue = 4,
	GreaterThan = 5,
	LessThan = 6,
	EqualTo = 7,
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

	std::vector<std::size_t> ProcessSubPackets()
	{
		std::vector<std::size_t> SubpacketValues;
		ELengthTypeID LengthType = static_cast<ELengthTypeID>(ExtractBits(1));
		switch (LengthType)
		{
		case ELengthTypeID::BitsLength:
			{
				const std::size_t BitsLength = ExtractBits(15);
				const std::size_t BitsEnd = Index + BitsLength;

				while (Index < BitsEnd)
				{
					SubpacketValues.push_back(ParsePacket());
				}
			}
			break;

		case ELengthTypeID::SubpacketsLength:
			{
				std::size_t SubpacketsLength = ExtractBits(11);
				SubpacketValues.reserve(SubpacketsLength);

				while (SubpacketsLength --> 0)
				{
					SubpacketValues.push_back(ParsePacket());
				}
			}
			break;
		default:
			abort();
		}

		return SubpacketValues;
	}

	std::size_t ParsePacket()
	{
		const std::size_t VersionNumber = ExtractBits(3);
		const EPacketTypeID TypeID = static_cast<EPacketTypeID>(ExtractBits(3));

		switch (TypeID)
		{
		case EPacketTypeID::Sum:
			{
				std::size_t Sum = 0;
				for (const auto& Value : ProcessSubPackets())
				{
					Sum += Value;
				}
				return Sum;
			}
		case EPacketTypeID::Product:
			{
				std::size_t Product = 1;
				for (const auto& Value : ProcessSubPackets())
				{
					Product *= Value;
				}
				return Product;
			}
		case EPacketTypeID::Minimum:
			{
				std::size_t Minimum = -1;
				for (const auto& Value : ProcessSubPackets())
				{
					Minimum = std::min(Minimum, Value);
				}
				return Minimum;
			}
		case EPacketTypeID::LiteralValue:
			{
				std::size_t ResultValue = 0;
				while (true)
				{
					bool IsFinalBit = ExtractBits(1) != 1;
					ResultValue = (ResultValue << 4) + ExtractBits(4);
					if (IsFinalBit)
					{
						break;
					}
				}
				return ResultValue;
			}
		case EPacketTypeID::Maximum:
			{
				std::size_t Maximum = 0;
				for (const auto& Value : ProcessSubPackets())
				{
					Maximum = std::max(Maximum, Value);
				}
				return Maximum;
			}
		case EPacketTypeID::GreaterThan:
			{
				const auto& Subpackets = ProcessSubPackets();
				return Subpackets[0] > Subpackets[1];
			}
		case EPacketTypeID::LessThan:
			{
				const auto& Subpackets = ProcessSubPackets();
				return Subpackets[0] < Subpackets[1];
			}
		case EPacketTypeID::EqualTo:
			{
				const auto& Subpackets = ProcessSubPackets();
				return Subpackets[0] == Subpackets[1];
			}
		default:
			abort();
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

	std::cout << NewPacket.ParsePacket() << std::endl;
}