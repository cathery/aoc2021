#include <fstream>
#include <iostream>
#include <array>

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

	constexpr std::size_t ScoreToWin = 21;
	constexpr std::size_t LowestPosition = 1;
	constexpr std::size_t HighestPosition = 10;
	constexpr std::size_t LowestDieOutcome = 1;
	constexpr std::size_t HighestDieOutcome = 3;

	std::size_t Player1UniversesWon = 0;
	std::size_t Player2UniversesWon = 0;

	// GameStates[Player1Position][Player1Score][Player2Position][Player2Score] = Number Of Universes in this game state
	std::array<std::array<std::array<std::array<std::size_t, ScoreToWin+1>, HighestPosition+1>, ScoreToWin+1>, HighestPosition+1> GameStates{};
	std::array<std::array<std::array<std::array<std::size_t, ScoreToWin+1>, HighestPosition+1>, ScoreToWin+1>, HighestPosition+1> NextGameStates{};

	GameStates[Player1StartingPosition][0][Player2StartingPosition][0] = 1;
	
	bool IsPlayer1Turn = true;
	while (true)
	{
		for (std::size_t Player1Position = LowestPosition; Player1Position != HighestPosition + 1; ++Player1Position)
		{
			for (std::size_t Player1Score = 0; Player1Score != ScoreToWin + 1; ++Player1Score)
			{
				for (std::size_t Player2Position = LowestPosition; Player2Position != HighestPosition + 1; ++Player2Position)
				{
					for (std::size_t Player2Score = 0; Player2Score != ScoreToWin + 1; ++Player2Score)
					{
						const std::size_t CurrentUniverseCount = GameStates[Player1Position][Player1Score][Player2Position][Player2Score];

						if (CurrentUniverseCount == 0)
						{
							continue;
						}

						for (std::uint8_t DiceRoll1 = LowestDieOutcome; DiceRoll1 != HighestDieOutcome + 1; ++DiceRoll1)
						{
							for (std::uint8_t DiceRoll2 = LowestDieOutcome; DiceRoll2 != HighestDieOutcome + 1; ++DiceRoll2)
							{
								for (std::uint8_t DiceRoll3 = LowestDieOutcome; DiceRoll3 != HighestDieOutcome + 1; ++DiceRoll3)
								{
									std::size_t MoveSpaces = DiceRoll1 + DiceRoll2 + DiceRoll3;
									if (IsPlayer1Turn)
									{
										const std::size_t NewPlayer1Position = ((Player1Position + MoveSpaces - 1) % 10) + 1;
										const std::size_t NewPlayer1Score = Player1Score + NewPlayer1Position;

										if (NewPlayer1Score >= ScoreToWin)
										{
											Player1UniversesWon += CurrentUniverseCount;
											continue;
										}

										NextGameStates[NewPlayer1Position][NewPlayer1Score][Player2Position][Player2Score] += CurrentUniverseCount;
									}
									else
									{
										const std::size_t NewPlayer2Position = ((Player2Position + MoveSpaces - 1) % 10) + 1;
										const std::size_t NewPlayer2Score = Player2Score + NewPlayer2Position;

										if (NewPlayer2Score >= ScoreToWin)
										{
											Player2UniversesWon += CurrentUniverseCount;
											continue;
										}

										NextGameStates[Player1Position][Player1Score][NewPlayer2Position][NewPlayer2Score] += CurrentUniverseCount;
									}
								}
							}
						}
					}
				}
			}
		}

		bool bAreAllGamesFinished = true;
		for (const auto& Player1Position : NextGameStates)
		{
			for (const auto& Player1Score : Player1Position)
			{
				for (const auto& Player2Position : Player1Score)
				{
					for (const auto& Player2Score : Player2Position)
					{
						if (Player2Score > 0)
						{
							bAreAllGamesFinished = false;
							goto BREAKLOOP;
						}
					}
				}
			}
		}

BREAKLOOP:
		IsPlayer1Turn = !IsPlayer1Turn;
		GameStates = NextGameStates;
		NextGameStates = {};

		if (bAreAllGamesFinished)
		{
			break;
		}
	}

	std::cout << (Player1UniversesWon > Player2UniversesWon ? Player1UniversesWon : Player2UniversesWon) << std::endl;
}