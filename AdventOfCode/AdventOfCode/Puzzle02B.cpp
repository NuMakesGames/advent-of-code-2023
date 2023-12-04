#include "pch.h"

#include "../Utilities/Utilities.h"
#include "PuzzleSolvers.h"

using namespace Utilities;

namespace Puzzle02B
{
	struct Cubes
	{
		int red = 0;
		int green = 0;
		int blue = 0;
	};

	std::vector<std::vector<Cubes>> ReadInput(const std::filesystem::path& inputFile)
	{
		std::vector<std::vector<Cubes>> games;
		for (const std::string& line : ReadAllLinesInFile(inputFile))
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

	void RenderGame(int gameId, const Cubes& required)
	{
		ScopedConsoleTextColor textColor(ConsoleForegroundColor::White);
		std::cout << "Game " << std::setw(3) << gameId << ": ";

		SetConsoleTextColor(ConsoleForegroundColor::IntenseWhite);
		std::cout << std::setw(4) << required.red * required.green * required.blue;

		SetConsoleTextColor(ConsoleForegroundColor::White);
		std::cout << " (";

		SetConsoleTextColor(ConsoleForegroundColor::IntenseRed);
		std::cout << std::setw(2) << required.red << " red";

		SetConsoleTextColor(ConsoleForegroundColor::White);
		std::cout << ", ";

		SetConsoleTextColor(ConsoleForegroundColor::IntenseGreen);
		std::cout << std::setw(2) << required.green << " green";

		SetConsoleTextColor(ConsoleForegroundColor::White);
		std::cout << ", ";

		SetConsoleTextColor(ConsoleForegroundColor::IntenseBlue);
		std::cout << std::setw(2) << required.blue << " blue";

		SetConsoleTextColor(ConsoleForegroundColor::White);
		std::cout << ")\n";

		std::cout << std::setw(1);
	}

	void PrintSolution(const std::filesystem::path& inputFile, bool shouldRender)
	{
		auto games = ReadInput(inputFile);
		int acc = 0;
		for (int i = 0; i < games.size(); ++i)
		{
			Cubes required;
			for (const Cubes& draw : games[i])
			{
				required.red = std::max(required.red, draw.red);
				required.green = std::max(required.green, draw.green);
				required.blue = std::max(required.blue, draw.blue);
			}

			acc += required.red * required.green * required.blue;

			if (shouldRender)
			{
				RenderGame(i + 1, required);
			}
		}

		std::cout << acc;
	}
} // namespace Puzzle02B