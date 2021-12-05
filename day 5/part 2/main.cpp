#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

// Read input until we succeed or reach eof
template<class T>
static bool SafeReadInput(std::ifstream& Input, T& Value)
{
	while (true)
	{
		Input >> Value;

		if (Input.eof() || Input.bad())
		{
			return false;
		}
		else if (Input.fail())
		{
			// Skip one character every time we fail
			Input.clear();
			Input.ignore(1);
		}
		else
		{
			return true;
		}
	}
}

struct VentLine
{
	std::uint64_t X1;
	std::uint64_t Y1;

	std::uint64_t X2;
	std::uint64_t Y2;

	friend std::ifstream& operator>>(std::ifstream& Input, VentLine& Value)
	{
		SafeReadInput(Input, Value.X1);
		SafeReadInput(Input, Value.Y1);
		SafeReadInput(Input, Value.X2);
		SafeReadInput(Input, Value.Y2);
		return Input;
	}

	constexpr bool IsHorizontal() const
	{
		return Y1 == Y2;
	}

	constexpr bool IsVertical() const
	{
		return X1 == X2;
	}

	constexpr bool IsCardinal() const
	{
		return IsHorizontal() || IsVertical();
	}

	constexpr bool IsOrdinal() const
	{
		const auto DeltaX = (X1 > X2) ? (X1 - X2) : (X2 - X1);
		const auto DeltaY = (Y1 > Y2) ? (Y1 - Y2) : (Y2 - Y1);

		return DeltaX == DeltaY;
	}
};

struct VentDiagram
{
	std::vector<std::vector<std::uint64_t>> Map;

	void MarkLine(const VentLine& Line)
	{
		if (!Line.IsCardinal() && !Line.IsOrdinal())
		{
			return;
		}

		// Extend the map to fit the coordinates
		const auto MaxY = std::max(std::max(Line.Y1, Line.Y2) + 1, Map.size());
		Map.resize(MaxY);

		const auto MaxX = std::max(std::max(Line.X1, Line.X2) + 1, Map[0].size());
		for (auto& MapX : Map)
		{
			MapX.resize(MaxX);
		}

		std::int8_t SignX = (Line.X1 < Line.X2) - (Line.X2 < Line.X1);
		std::int8_t SignY = (Line.Y1 < Line.Y2) - (Line.Y2 < Line.Y1);

		std::uint64_t Index = 0;
		while (true)
		{
			std::uint64_t LineY = Line.Y1 + (Index * SignY);
			std::uint64_t LineX = Line.X1 + (Index * SignX);

			Map[LineY][LineX]++;

			if (LineY == Line.Y2 && LineX == Line.X2)
			{
				break;
			}

			Index++;
		}
	}

	std::uint64_t CountOverlappingPoints() const
	{
		std::uint64_t SumTotal = 0;
		for (const auto& MapY : Map)
		{
			for (const auto& MapX : MapY)
			{
				if (MapX > 1)
				{
					SumTotal++;
				}
			}
		}
		return SumTotal;
	}

	friend std::ofstream& operator<<(std::ofstream& Output, VentDiagram& Value)
	{
		for (const auto& MapY : Value.Map)
		{
			for (const auto& MapX : MapY)
			{
				if (MapX >= 10)
				{
					Output << '*';
				}
				else if (MapX > 0)
				{
					Output << MapX;
				}
				else
				{
					Output << '.';
				}
			}
			Output << std::endl;
		}
		return Output;
	}
};

int main()
{
	std::ifstream Input("input.txt");
	std::string StringBuffer;

	VentLine CurrentLine;
	VentDiagram Diagram;

	// {%},{%} -> {%},{%}
	while (Input >> CurrentLine)
	{
		Diagram.MarkLine(CurrentLine);
	}

	std::cout << "Total overlapping points: " << Diagram.CountOverlappingPoints() << std::endl;

	std::ofstream Output("output.txt");
	Output << Diagram;
}