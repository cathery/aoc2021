#include <fstream>
#include <iostream>
#include <unordered_map>
#include <algorithm>

union Pair_t
{
	char16_t Data;
	struct
	{
		char FirstCharacter;
		char SecondCharacter;
	};
};

struct PairInfo_t
{
	std::size_t Count;
	std::size_t PendingCount;
	char InsertedCharacter;
};

int main()
{
	std::ifstream Input("input.txt");

	std::unordered_map<char, std::size_t> NumChars;

	std::unordered_map<char16_t, PairInfo_t> Pairs;

	Pair_t CurrentPair{};

	while (Input)
	{
		const char CharacterBuffer = Input.get();
		NumChars[CharacterBuffer]++;

		CurrentPair.FirstCharacter = CurrentPair.SecondCharacter;
		CurrentPair.SecondCharacter = CharacterBuffer;

		Pairs[CurrentPair.Data].Count++;

		if (Input.peek() == '\n')
		{
			Input.ignore(2);
			break;
		}
	}

	while (Input.peek() != EOF)
	{
		Pair_t RuleCondition;
		RuleCondition.FirstCharacter = Input.get();
		RuleCondition.SecondCharacter = Input.get();
		Input.ignore(4);
		const char InsertedCharacter = Input.get();
		Input.ignore(1);
		
		Pairs[RuleCondition.Data].InsertedCharacter = InsertedCharacter;
	}

	for (std::size_t Step = 0; Step != 40; ++Step)
	{
		for (const auto& Pair : Pairs)
		{
			if (Pair.second.InsertedCharacter != 0)
			{
				const Pair_t* TypedPair = reinterpret_cast<const Pair_t*>(&Pair.first);

				Pair_t NewPair;

				NewPair.FirstCharacter = TypedPair->FirstCharacter;
				NewPair.SecondCharacter = Pair.second.InsertedCharacter;
				Pairs[NewPair.Data].PendingCount += Pair.second.Count;

				NewPair.FirstCharacter = NewPair.SecondCharacter;
				NewPair.SecondCharacter = TypedPair->SecondCharacter;
				Pairs[NewPair.Data].PendingCount += Pair.second.Count;

				NumChars[Pair.second.InsertedCharacter] += Pair.second.Count;
				Pairs[Pair.first].Count = 0;
			}
		}
		for (auto& Pair : Pairs)
		{
			Pair.second.Count += Pair.second.PendingCount;
			Pair.second.PendingCount = 0;
		}
	}

	const auto& [SmallestCharNum, BiggestCharNum] = std::minmax_element(NumChars.begin(), NumChars.end(), [](const auto& L, const auto& R) { return L.second < R.second; });

	std::cout << BiggestCharNum->second - SmallestCharNum->second << std::endl;
}