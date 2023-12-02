#include "pch.h"

#include "../Utilities/Utilities.h"
#include "PuzzleSolvers.h"

using namespace Utilities;

namespace Puzzle02B
{
	struct Cubes
	{
		int blue = 0;
		int red = 0;
		int green = 0;
	};

	std::vector<std::vector<Cubes>> ReadInput()
	{
		std::vector<std::vector<Cubes>> games;
		for (const std::string& line : ReadAllLinesInFile("Puzzle02.input"))
		{
			// Find where draws start for this game
			size_t iDrawStart = line.find(':') + 2;

			// Split draws for the game
			std::vector<Cubes> drawsForGame;
			auto drawTokens = SplitString(line.substr(iDrawStart), "; ");

			// Parse each draw
			for (const std::string& drawToken : drawTokens)
			{
				Cubes draw;
				auto individualCubeDrawTokens = SplitString(drawToken, ", ");
				for (const std::string& individualCubeDrawToken : individualCubeDrawTokens)
				{
					// Parse count and color of the individual draw
					auto [_, count, color] = scn::scan_tuple<int, std::string>(individualCubeDrawToken, "{} {}");
					if (color == "blue")
					{
						draw.blue = count;
					}
					else if (color == "red")
					{
						draw.red = count;
					}
					else if (color == "green")
					{
						draw.green = count;
					}
				}

				// Capture this draw
				drawsForGame.emplace_back(draw);
			}

			// Capture all draws for the game
			games.emplace_back(drawsForGame);
		}

		return games;
	}

	void PrintSolution()
	{
		int acc = 0;
		for (const std::vector<Cubes>& game : ReadInput())
		{
			Cubes required;
			for (const Cubes& draw : game)
			{
				required.red = std::max(required.red, draw.red);
				required.green = std::max(required.green, draw.green);
				required.blue = std::max(required.blue, draw.blue);
			}

			acc += required.red * required.green * required.blue;
		}

		std::cout << acc;
	}
} // namespace Puzzle02B