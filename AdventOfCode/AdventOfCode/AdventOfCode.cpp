// AdventOfCode.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"

#include "../Utilities/Utilities.h"
#include "PuzzleSolvers.h"

using namespace Utilities;

const std::unordered_set<std::string> validArgs = {
	"--sampleInput", "--fullInput", "--render",   "--partA",    "--partB",    "--puzzle01", "--puzzle02", "--puzzle03",
	"--puzzle04",    "--puzzle05",  "--puzzle06", "--puzzle07", "--puzzle08", "--puzzle09", "--puzzle10", "--puzzle11",
	"--puzzle12",    "--puzzle13",  "--puzzle14", "--puzzle15", "--puzzle16", "--puzzle17", "--puzzle18", "--puzzle19",
	"--puzzle20",    "--puzzle21",  "--puzzle22", "--puzzle23", "--puzzle24", "--puzzle25"
};

const std::vector<std::function<void(const std::filesystem::path&, bool)>> partASolvers = {
	Puzzle01A::PrintSolution, Puzzle02A::PrintSolution, Puzzle03A::PrintSolution, Puzzle04A::PrintSolution, Puzzle05A::PrintSolution,
	Puzzle06A::PrintSolution, Puzzle07A::PrintSolution, Puzzle08A::PrintSolution, Puzzle09A::PrintSolution, Puzzle10A::PrintSolution,
	Puzzle11A::PrintSolution, Puzzle12A::PrintSolution, Puzzle13A::PrintSolution, Puzzle14A::PrintSolution, Puzzle15A::PrintSolution,
	Puzzle16A::PrintSolution, Puzzle17A::PrintSolution, Puzzle18A::PrintSolution, Puzzle19A::PrintSolution, Puzzle20A::PrintSolution,
	Puzzle21A::PrintSolution, Puzzle22A::PrintSolution, Puzzle23A::PrintSolution, Puzzle24A::PrintSolution, Puzzle25A::PrintSolution
};

const std::vector<std::function<void(const std::filesystem::path&, bool)>> partBSolvers = {
	Puzzle01B::PrintSolution, Puzzle02B::PrintSolution, Puzzle03B::PrintSolution, Puzzle04B::PrintSolution, Puzzle05B::PrintSolution,
	Puzzle06B::PrintSolution, Puzzle07B::PrintSolution, Puzzle08B::PrintSolution, Puzzle09B::PrintSolution, Puzzle10B::PrintSolution,
	Puzzle11B::PrintSolution, Puzzle12B::PrintSolution, Puzzle13B::PrintSolution, Puzzle14B::PrintSolution, Puzzle15B::PrintSolution,
	Puzzle16B::PrintSolution, Puzzle17B::PrintSolution, Puzzle18B::PrintSolution, Puzzle19B::PrintSolution, Puzzle20B::PrintSolution,
	Puzzle21B::PrintSolution, Puzzle22B::PrintSolution, Puzzle23B::PrintSolution, Puzzle24B::PrintSolution, Puzzle25B::PrintSolution
};

const std::vector<std::vector<std::filesystem::path>> puzzleInputPaths = {
	{ "Puzzle01.input" }, { "Puzzle02.input" }, { "Puzzle03.input" }, { "Puzzle04.input" }, { "Puzzle05.input" },
	{ "Puzzle06.input" }, { "Puzzle07.input" }, { "Puzzle08.input" }, { "Puzzle09.input" }, { "Puzzle10.input" },
	{ "Puzzle11.input" }, { "Puzzle12.input" }, { "Puzzle13.input" }, { "Puzzle14.input" }, { "Puzzle15.input" },
	{ "Puzzle16.input" }, { "Puzzle17.input" }, { "Puzzle18.input" }, { "Puzzle19.input" }, { "Puzzle20.input" },
	{ "Puzzle21.input" }, { "Puzzle22.input" }, { "Puzzle23.input" }, { "Puzzle24.input" }, { "Puzzle25.input" },
};

const std::vector<std::vector<std::vector<std::filesystem::path>>> puzzleSamplePaths = {
	{
		{ "Puzzle01SampleA.input", "Puzzle01SampleB.input" },
		{ "Puzzle01SampleA.input", "Puzzle01SampleB.input" },
	},
	{
		{ "Puzzle02SampleA.input" },
		{ "Puzzle02SampleA.input" },
	},
	{
		{ "Puzzle03SampleA.input" },
		{ "Puzzle03SampleA.input" },
	},
	{
		{ "Puzzle04SampleA.input" },
		{ "Puzzle04SampleA.input" },
	},
	{
		{ "Puzzle05SampleA.input" },
		{ "Puzzle05SampleA.input" },
	},
	{
		{ "Puzzle06SampleA.input" },
		{ "Puzzle06SampleA.input" },
	},
	{
		{ "Puzzle07SampleA.input" },
		{ "Puzzle07SampleA.input" },
	},
	{
		{ "Puzzle08SampleA.input", "Puzzle08SampleB.input" },
		{ "Puzzle08SampleC.input" },
	},
	{
		{ "Puzzle09SampleA.input" },
		{ "Puzzle09SampleA.input" },
	},
	{
		{ "Puzzle10SampleA.input", "Puzzle10SampleB.input" },
		{ "Puzzle10SampleC.input", "Puzzle10SampleD.input", "Puzzle10SampleE.input" },
	},
	{
		{ "Puzzle11SampleA.input" },
		{ "Puzzle11SampleA.input" },
	},
	{
		{ "Puzzle12SampleA.input" },
		{ "Puzzle12SampleA.input" },
	},
	{
		{ "Puzzle13SampleA.input" },
		{ "Puzzle13SampleA.input" },
	},
	{
		{ "Puzzle14SampleA.input" },
		{ "Puzzle14SampleA.input" },
	},
	{
		{ "Puzzle15SampleA.input" },
		{ "Puzzle15SampleA.input" },
	},
	{
		{ "Puzzle16SampleA.input" },
		{ "Puzzle16SampleA.input" },
	},
	{
		{ "Puzzle17SampleA.input" },
		{ "Puzzle17SampleA.input" },
	},
	{
		{ "Puzzle18SampleA.input" },
		{ "Puzzle18SampleA.input" },
	},
	{
		{ "Puzzle19SampleA.input" },
		{ "Puzzle19SampleA.input" },
	},
	{
		{ "Puzzle20SampleA.input" },
		{ "Puzzle20SampleA.input" },
	},
	{
		{ "Puzzle21SampleA.input" },
		{ "Puzzle21SampleA.input" },
	},
	{
		{ "Puzzle22SampleA.input" },
		{ "Puzzle22SampleA.input" },
	},
	{
		{ "Puzzle23SampleA.input" },
		{ "Puzzle23SampleA.input" },
	},
	{
		{ "Puzzle24SampleA.input" },
		{ "Puzzle24SampleA.input" },
	},
	{
		{ "Puzzle25SampleA.input" },
		{ "Puzzle25SampleA.input" },
	}
};

struct Args
{
	bool useSampleInput = false;
	bool useFullInput = false;
	bool shouldRender = false;
	bool runPartA = false;
	bool runPartB = false;
	std::vector<int> puzzlesToRun;
};

Args ReadArgs(int argc, char* argv[])
{
	ScopedConsoleTextColor textColor{ ConsoleForegroundColor::IntenseGreen };
	{
		std::string header = "Advent of Code";
		for (int i = 0; i < header.size(); ++i)
		{
			SetConsoleTextColor(i % 2 == 0 ? ConsoleForegroundColor::IntenseGreen : ConsoleForegroundColor::IntenseRed);
			std::cout << header[i];
		}
		SetConsoleTextColor(ConsoleForegroundColor::IntenseYellow);
		std::cout << " 2023\n";
	}

	Args result;
	if (argc > 1)
	{
		SetConsoleTextColor(ConsoleForegroundColor::IntenseWhite);
		std::cout << "Command line arguments:\n";
		for (int i = 1; i < argc; ++i)
		{
			std::string arg = argv[i];
			if (validArgs.contains(arg))
			{
				SetConsoleTextColor(ConsoleForegroundColor::IntenseGreen);

				if (arg == "--sampleInput")
				{
					result.useSampleInput = true;
				}
				else if (arg == "--fullInput")
				{
					result.useFullInput = true;
				}
				else if (arg == "--render")
				{
					result.shouldRender = true;
				}
				else if (arg == "--partA")
				{
					result.runPartA = true;
				}
				else if (arg == "--partB")
				{
					result.runPartB = true;
				}
				else
				{
					result.puzzlesToRun.push_back(std::stoi(arg.substr(arg.size() - 2)));
				}
			}
			else
			{
				SetConsoleTextColor(ConsoleForegroundColor::IntenseRed);
			}

			std::cout << "    " << argv[i] << "\n";
		}
	}

	if (!result.runPartA && !result.runPartB)
	{
		result.runPartA = true;
		result.runPartB = true;
	}

	if (!result.useSampleInput && !result.useFullInput)
	{
		result.useSampleInput = true;
		result.useFullInput = true;
	}

	std::ranges::sort(result.puzzlesToRun);
	return result;
}

int main(int argc, char* argv[])
{
	Args args = ReadArgs(argc, argv);
	if (args.puzzlesToRun.empty())
	{
		ScopedConsoleTextColor textColor{ ConsoleForegroundColor::IntenseRed };
		std::cout << "\nNo puzzles specified for execution. Pass desired puzzles as command-line arguments. Sample usage:\n";

		SetConsoleTextColor(ConsoleForegroundColor::IntenseWhite);
		std::cout << "  AdventOfCode.exe ";
		SetConsoleTextColor(ConsoleForegroundColor::IntenseGreen);
		std::cout << "--puzzle04\n";

		SetConsoleTextColor(ConsoleForegroundColor::IntenseWhite);
		std::cout << "  AdventOfCode.exe ";
		SetConsoleTextColor(ConsoleForegroundColor::IntenseGreen);
		std::cout << "--render --puzzle01 --puzzle02\n";

		SetConsoleTextColor(ConsoleForegroundColor::IntenseWhite);
		std::cout << "  AdventOfCode.exe ";
		SetConsoleTextColor(ConsoleForegroundColor::IntenseGreen);
		std::cout << "--partB --fullInput --puzzle02\n";

		SetConsoleTextColor(ConsoleForegroundColor::IntenseWhite);
		std::cout << "  AdventOfCode.exe ";
		SetConsoleTextColor(ConsoleForegroundColor::IntenseGreen);
		std::cout << "--partA --sampleInput --puzzle03\n";
	}

	for (int puzzleId : args.puzzlesToRun)
	{
		for (auto i = 0; i < 2; ++i)
		{
			if (i == 0 && !args.runPartA)
			{
				continue;
			}

			if (i == 1 && !args.runPartB)
			{
				continue;
			}

			{
				ScopedConsoleTextColor textColor{ ConsoleForegroundColor::IntenseWhite };
				std::cout << "\nRunning puzzle ";
				SetConsoleTextColor(ConsoleForegroundColor::IntenseCyan);
				std::cout << puzzleId;
				SetConsoleTextColor(ConsoleForegroundColor::IntenseWhite);
				std::cout << ", part ";
				SetConsoleTextColor(i == 0 ? ConsoleForegroundColor::IntenseGreen : ConsoleForegroundColor::IntenseRed);
				std::cout << (i == 0 ? 'A' : 'B');
				SetConsoleTextColor(ConsoleForegroundColor::IntenseWhite);
				std::cout << ":\n";
			}

			std::vector<std::filesystem::path> inputPaths;
			if (args.useSampleInput)
			{
				inputPaths.append_range(puzzleSamplePaths[puzzleId - 1][i]);
			}
			if (args.useFullInput)
			{
				inputPaths.append_range(puzzleInputPaths[puzzleId - 1]);
			}

			const auto solver = i == 0 ? partASolvers[puzzleId - 1] : partBSolvers[puzzleId - 1];
			for (const auto& inputPath : inputPaths)
			{
				{
					ScopedConsoleTextColor textColor{ ConsoleForegroundColor::IntenseWhite };
					std::cout << "Using input file ";
					SetConsoleTextColor(ConsoleForegroundColor::IntenseCyan);
					std::cout << inputPath;
					SetConsoleTextColor(ConsoleForegroundColor::IntenseWhite);
					std::cout << ":\n";
				}

				// Fully qualify the path to the input files, which have been copied next to the executable.
				static const std::filesystem::path executablePath = argv[0];
				std::filesystem::path fullInputPath = executablePath.parent_path() / inputPath;

				auto start = std::chrono::high_resolution_clock::now();
				solver(fullInputPath, args.shouldRender);
				auto stop = std::chrono::high_resolution_clock::now();
				auto durationMs = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);

				{
					ScopedConsoleTextColor textColor{ ConsoleForegroundColor::IntenseWhite };
					std::cout << "\nSolver executed in ";
					SetConsoleTextColor(ConsoleForegroundColor::IntenseYellow);
					std::cout << durationMs.count() << " ms";
					SetConsoleTextColor(ConsoleForegroundColor::IntenseWhite);
					std::cout << ".\n";
				}
			}
		}
	}
}