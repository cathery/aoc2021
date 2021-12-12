#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <set>
#include <vector>

struct Cave
{
	bool IsSmall;
	std::set<const Cave*> Connections;
};

using CavePath = std::vector<const Cave*>;
using VisitsMap = std::unordered_map<const Cave*, std::uint8_t>;

static std::unordered_map<std::string, Cave> CaveMap;
static const Cave* StartCave;
static const Cave* EndCave;

static std::set<CavePath> UniquePaths;
static std::uintmax_t PathsCount = 0;

static void WalkThroughCave(const Cave* CurrentCave, VisitsMap VisitsLeft, CavePath PathTaken)
{
	if (CurrentCave == EndCave)
	{
		if (UniquePaths.count(PathTaken))
		{
			return;
		}
		UniquePaths.insert(std::move(PathTaken));
		PathsCount++;
		return;
	}

	if (CurrentCave->IsSmall)
	{
		VisitsLeft[CurrentCave] -= VisitsLeft[CurrentCave] > 0;
	}

	PathTaken.push_back(CurrentCave);

	for (const auto& ConnectedCave : CurrentCave->Connections)
	{
		if (!ConnectedCave->IsSmall || VisitsLeft[ConnectedCave] > 0)
		{
			WalkThroughCave(ConnectedCave, VisitsLeft, PathTaken);
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

	StartCave = &CaveMap.at("start");
	EndCave = &CaveMap.at("end");

	VisitsMap DefaultVisitsLeft;
	for (auto& CurrentCave : CaveMap)
	{
		if (CurrentCave.second.IsSmall)
		{
			DefaultVisitsLeft[&CurrentCave.second] = 1;
		}
	}

	for (auto& CurrentCave : CaveMap)
	{
		const Cave* CavePtr = &CurrentCave.second;
		if (CavePtr != StartCave && CavePtr != EndCave)
		{
			if (CavePtr->IsSmall)
			{
				VisitsMap VisitsLeft{DefaultVisitsLeft};
				VisitsLeft[CavePtr]++;

				WalkThroughCave(StartCave, VisitsLeft, {});
			}
		}
	}

	std::cout << PathsCount << std::endl;
}