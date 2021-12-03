#include <fstream>
#include <iostream>
#include <map>

class Submarine
{
public:
	Submarine()
		: HorizontalPosition(0)
		, Depth(0)
		, Aim(0)
	{
	}

	std::int64_t HorizontalPosition;
	std::int64_t Depth;
	std::int64_t Aim;
};

static void CommandForward(Submarine& Sub, std::ifstream& Input)
{
	std::int64_t HorizontalPosIncrease;
	Input >> HorizontalPosIncrease;
	Sub.HorizontalPosition += HorizontalPosIncrease;
	Sub.Depth += Sub.Aim * HorizontalPosIncrease;
}

static void CommandDown(Submarine& Sub, std::ifstream& Input)
{
	std::int64_t AimIncrease;
	Input >> AimIncrease;
	Sub.Aim += AimIncrease;
}

static void CommandUp(Submarine& Sub, std::ifstream& Input)
{
	std::int64_t AimDecrease;
	Input >> AimDecrease;
	Sub.Aim -= AimDecrease;
}

using CommandFunction = void(*)(Submarine&, std::ifstream&);

static std::map<std::string, CommandFunction> CommandMap{
	{ "forward", CommandForward },
	{ "down", CommandDown },
	{ "up", CommandUp },
};

int main()
{
	std::ifstream Input("input.txt");

	Submarine Sub;

	std::string CommandName;

	while (Input >> CommandName)
	{
		CommandFunction Command = CommandMap.at(CommandName);
		Command(Sub, Input);

	}

	std::cout << Sub.Depth * Sub.HorizontalPosition;
}