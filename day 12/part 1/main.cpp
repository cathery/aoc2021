#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <set>

struct Cave
{
	bool IsSmall;
	std::set<const Cave*> Connections;
};

using VisitsMap = std::set<const Cave*>;

static std::unordered_map<std::string, Cave> CaveMap;
static std::uintmax_t PathsCount = 0;

static void WalkThroughCave(const Cave* CurrentCave, VisitsMap VisitedCaves)
{
	static const Cave* EndCave = &CaveMap.at("end");
	if (CurrentCave == EndCave)
	{
		PathsCount++;
		return;
	}

	if (CurrentCave->IsSmall)
	{
		VisitedCaves.insert(CurrentCave);
	}

	for (const auto& ConnectedCave : CurrentCave->Connections)
	{
		if (!ConnectedCave->IsSmall || !VisitedCaves.count(ConnectedCave))
		{
			WalkThroughCave(ConnectedCave, VisitedCaves);
		}
	}
}

int main()
{
	std::ifstream Input("input.txt");

	std::string CaveName1;
	std::string CaveName2;

	while(Input.peek() != EOF)
	{
		std::getline(Input, CaveName1, '-');
		std::getline(Input, CaveName2, '\n');

		auto& Cave1 = CaveMap[CaveName1];
		auto& Cave2 = CaveMap[CaveName2];

		Cave1.IsSmall = islower(CaveName1[0]);
		Cave2.IsSmall = islower(CaveName2[0]);

		Cave1.Connections.insert(&Cave2);
		Cave2.Connections.insert(&Cave1);
	}

	WalkThroughCave(&CaveMap.at("start"), {});

	std::cout << PathsCount << std::endl;
}