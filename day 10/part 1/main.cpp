#include <fstream>
#include <iostream>
#include <stack>
#include <unordered_map>
#include <limits>
#include <array>

enum Bracket : std::uint8_t
{
	None,
	RoundBracket,
	SquareBracket,
	CurlyBracket,
	AngleBracket,
};

constexpr static std::array BracketScores{
	0,
	3,
	57,
	1197,
	25137,
};

const static std::unordered_map<char, Bracket> BracketMap{
	{'\n', Bracket::None},
	{'(', Bracket::RoundBracket},
	{'[', Bracket::SquareBracket},
	{'{', Bracket::CurlyBracket},
	{'<', Bracket::AngleBracket},
	{')', Bracket::RoundBracket},
	{']', Bracket::SquareBracket},
	{'}', Bracket::CurlyBracket},
	{'>', Bracket::AngleBracket},
};


int main()
{
	std::ifstream Input("input.txt");
	Input >> std::noskipws;

	char CharacterBuffer;
	std::stack<Bracket> BracketStack;
	std::uintmax_t SyntaxErrorScore = 0;

	while (Input >> CharacterBuffer)
	{
		const Bracket CurrentBracket = BracketMap.at(CharacterBuffer);
		switch (CharacterBuffer)
		{
		case '(':
		case '[':
		case '{':
		case '<':
			BracketStack.push(CurrentBracket);
			break;
		case ')':
		case ']':
		case '}':
		case '>':
			if (BracketStack.size() && BracketStack.top() == CurrentBracket)
			{
				BracketStack.pop();
				break;
			}
			else
			{
				SyntaxErrorScore += BracketScores.at(CurrentBracket);
				Input.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			}
		case '\n':
			BracketStack = {};
		default:
			break;
		}
	}

	std::cout << SyntaxErrorScore << std::endl;
}