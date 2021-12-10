#include <fstream>
#include <iostream>
#include <stack>
#include <unordered_map>
#include <limits>
#include <array>
#include <vector>
#include <algorithm>

enum Bracket : std::uint8_t
{
	None,
	RoundBracket,
	SquareBracket,
	CurlyBracket,
	AngleBracket,
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
	std::vector<std::uintmax_t> SyntaxErrorScores;

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
			}
			else
			{
				// line is corrupt, ignore
				Input.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
				BracketStack = {};
			}
			break;
		case '\n':
			{
				// line is incomplete, find the syntax error score
				const std::size_t StackSize = BracketStack.size();
				std::uintmax_t SyntaxErrorScore = 0;
				for (std::size_t Index = 0; Index != StackSize; ++Index)
				{
					SyntaxErrorScore *= 5;
					SyntaxErrorScore += BracketStack.top();
					BracketStack.pop();
				}
				SyntaxErrorScores.push_back(SyntaxErrorScore);
			}
			BracketStack = {};
		default:
			break;
		}
	}

	std::sort(SyntaxErrorScores.begin(), SyntaxErrorScores.end());

	std::cout << SyntaxErrorScores[(SyntaxErrorScores.size() - 1) / 2] << std::endl;
}