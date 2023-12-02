#include "pch.h"

#include "../Utilities/Utilities.h"
#include "PuzzleSolvers.h"

using namespace Utilities;

namespace Puzzle02A
{
	struct Cubes
	{
		int red = 0;
		int green = 0;
		int blue = 0;
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
		auto games = ReadInput();

		constexpr Cubes maxCubes{ 12, 13, 14 };
		int acc = 0;
		for (int i = 0; i < games.size(); ++i)
		{
			// Game is valid only if all draws in the game don't require more cubes than are in the bag
			bool isGameValid = true;
			for (const auto& draw : games[i])
			{
				if (draw.red > maxCubes.red || draw.green > maxCubes.green || draw.blue > maxCubes.blue)
				{
					isGameValid = false;
				}
			}

			if (isGameValid)
			{
				// If the game is valid, add the game number
				acc += i + 1;
			}
		}
	
		std::cout << acc;
	}
} // namespace Puzzle02A