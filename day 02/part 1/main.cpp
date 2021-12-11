#include <fstream>
#include <iostream>
#include <map>

class Submarine
{
public:
	Submarine()
		: HorizontalPosition(0)
		, Depth(0)
	{
	}

	std::int64_t HorizontalPosition;
	std::int64_t Depth;
};

static void CommandForward(Submarine& Sub, std::ifstream& Input)
{
	std::int64_t HorizontalPosIncrease;
	Input >> HorizontalPosIncrease;
	Sub.HorizontalPosition += HorizontalPosIncrease;
}

static void CommandDown(Submarine& Sub, std::ifstream& Input)
{
	std::int64_t DepthIncrease;
	Input >> DepthIncrease;
	Sub.Depth += DepthIncrease;
}

static void CommandUp(Submarine& Sub, std::ifstream& Input)
{
	std::int64_t DepthDecrease;
	Input >> DepthDecrease;
	Sub.Depth -= DepthDecrease;
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