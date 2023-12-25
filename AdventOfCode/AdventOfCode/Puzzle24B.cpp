#include "pch.h"

#include "../Utilities/Utilities.h"
#include "PuzzleSolvers.h"

using namespace Utilities;

namespace Puzzle24B
{
	struct Hailstone
	{
		Vector3d<double> position;
		Vector3d<double> velocity;
	};

	std::vector<Hailstone> ReadInput(const std::filesystem::path& inputFile)
	{
		std::vector<Hailstone> hailstones;
		for (const std::string& line : ReadAllLinesInFile(inputFile))
		{
			std::vector<std::string> tokens = SplitString(line, " @ ");

			auto transform = [](std::string_view sv) { return std::stoll(std::string(sv)); };
			std::vector<int64_t> positionTokens = SplitStringAndTransform<int64_t>(tokens[0], ", ", transform);
			std::vector<int64_t> velocityTokens = SplitStringAndTransform<int64_t>(tokens[1], ", ", transform);
			hailstones.emplace_back(
				Vector3d<double>{ static_cast<double>(positionTokens[0]),
			                      static_cast<double>(positionTokens[1]),
			                      static_cast<double>(positionTokens[2]) },
				Vector3d<double>{ static_cast<double>(velocityTokens[0]),
			                      static_cast<double>(velocityTokens[1]),
			                      static_cast<double>(velocityTokens[2]) });
		}

		return hailstones;
	}

	void PrintSolution(const std::filesystem::path& inputFile, bool shouldRender)
	{
		std::vector<Hailstone> hailstones = ReadInput(inputFile);

		// Need to solve for a rock position and velocity that intersects with all hailstones
		// We'll just build output for WolframAlpha to consume
		constexpr bool printEquations = true;
		if (printEquations)
		{
			for (const Hailstone& hail : hailstones)
			{
				std::string str = std::format(
					"(x-{})/(a-{})=(y-{})/(b-{})=(z-{})/(c-{})",
					hail.position.x,
					hail.velocity.x,
					hail.position.y,
					hail.velocity.y,
					hail.position.z,
					hail.velocity.z);

				size_t pos = str.find("--");
				while (pos != std::string::npos)
				{
					str.replace(pos, 2, "+");
					pos = str.find("--", pos + 1);
				}

				std::cout << str << "\n";
			}
		}

		// Solved on WolframAlpha with "system of four equations"
		// x = 150345122760494 and y = 343916889344399 and z = 183394034557877 and c = 182 and b = -109 and a = 196
		// TOO HIGH: 14344154377373234138
		std::cout << 150345122760494ull + 343916889344399ull + 183394034557877ull;
	}
} // namespace Puzzle24B