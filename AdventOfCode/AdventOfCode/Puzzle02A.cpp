#include "pch.h"

#include "../Utilities/Utilities.h"
#include "PuzzleSolvers.h"

using namespace std::chrono_literals;
using namespace Utilities;

namespace Puzzle02A
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

	void RenderGame(int gameId, bool isGameValid, const Cubes& maxCubes, const Cubes& firstInvalidDraw)
	{
		ScopedConsoleTextColor textColor(ConsoleForegroundColor::White);
		std::cout << "Game " << std::setw(3) << gameId << ": ";

		if (isGameValid)
		{
			SetConsoleTextColor(ConsoleForegroundColor::IntenseGreen);
			std::cout << "Valid\n";
		}
		else
		{
			SetConsoleTextColor(ConsoleForegroundColor::IntenseRed);
			std::cout << "Invalid";

			SetConsoleTextColor(ConsoleForegroundColor::White);
			std::cout << " (";

			bool isFirstInvalid = true;
			if (firstInvalidDraw.red > maxCubes.red)
			{
				SetConsoleTextColor(ConsoleForegroundColor::IntenseRed);
				std::cout << std::setw(2) << firstInvalidDraw.red << " red";
				isFirstInvalid = false;
			}

			if (firstInvalidDraw.green > maxCubes.green)
			{
				if (!isFirstInvalid)
				{
					SetConsoleTextColor(ConsoleForegroundColor::White);
					std::cout << ", ";
				}

				SetConsoleTextColor(ConsoleForegroundColor::IntenseGreen);
				std::cout << std::setw(2) << firstInvalidDraw.green << " green";
				isFirstInvalid = false;
			}

			if (firstInvalidDraw.blue > maxCubes.blue)
			{
				if (!isFirstInvalid)
				{
					SetConsoleTextColor(ConsoleForegroundColor::White);
					std::cout << ", ";
				}

				SetConsoleTextColor(ConsoleForegroundColor::IntenseBlue);
				std::cout << std::setw(2) << firstInvalidDraw.blue << " blue";
			}

			SetConsoleTextColor(ConsoleForegroundColor::White);
			std::cout << ")\n";
		}

		std::cout << std::setw(1);
	}

	void PrintSolution(const std::filesystem::path& inputFile, bool shouldRender)
	{
		auto games = ReadInput(inputFile);

		constexpr Cubes maxCubes{ 12, 13, 14 };
		int acc = 0;
		for (int i = 0; i < games.size(); ++i)
		{
			// Game is valid only if all draws in the game don't require more cubes than are in the bag
			bool isGameValid = true;
			Cubes firstInvalidDraw;
			for (const Cubes& draw : games[i])
			{
				if (draw.red > maxCubes.red || draw.green > maxCubes.green || draw.blue > maxCubes.blue)
				{
					isGameValid = false;
					firstInvalidDraw = draw;
					break;
				}
			}

			if (isGameValid)
			{
				// If the game is valid, add the game number
				acc += i + 1;
			}

			// Render result, if requested
			if (shouldRender)
			{
				RenderGame(i + 1, isGameValid, maxCubes, firstInvalidDraw);
			}
		}

		std::cout << acc;
	}
} // namespace Puzzle02A