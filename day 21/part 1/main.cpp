#include <fstream>
#include <iostream>

int main()
{
	std::ifstream Input("input.txt");

	if (!Input)
	{
		abort();
	}

	std::size_t Player1StartingPosition;
	std::size_t Player2StartingPosition;

	Input.ignore(28);
	Input >> Player1StartingPosition;
	Input.ignore(29);
	Input >> Player2StartingPosition;

	std::size_t DieCounter = 0;
	std::size_t NumberOfDieRolls = 0;

	auto RollDie = [&]() -> std::size_t
	{
		DieCounter = (DieCounter % 100) + 1;
		NumberOfDieRolls++;

		return DieCounter;
	};

	std::size_t Player1Position = Player1StartingPosition;
	std::size_t Player2Position = Player2StartingPosition;

	std::size_t Player1Score = 0;
	std::size_t Player2Score = 0;

	bool IsPlayer1Turn = true;

	while ((Player1Score < 1000) && (Player2Score < 1000))
	{
		std::size_t MoveSpaces = RollDie() + RollDie() + RollDie();
		if (IsPlayer1Turn)
		{
			Player1Position = ((Player1Position + MoveSpaces - 1) % 10) + 1;
			Player1Score += Player1Position;
		}
		else
		{
			Player2Position = ((Player2Position + MoveSpaces - 1) % 10) + 1;
			Player2Score += Player2Position;
		}
		IsPlayer1Turn = !IsPlayer1Turn;
	}

	std::cout << (IsPlayer1Turn ? Player1Score : Player2Score) * NumberOfDieRolls << std::endl;
}