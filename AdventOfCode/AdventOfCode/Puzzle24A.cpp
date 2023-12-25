#include "pch.h"

#include "../Utilities/Utilities.h"
#include "PuzzleSolvers.h"

using namespace Utilities;

namespace Puzzle24A
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

	std::pair<bool, Vector2d<double>> FindIntersection(const Hailstone& left, const Hailstone& right)
	{
		// Hailstone line can be represented as lines ax + by = c
		double a1 = left.velocity.y;
		double b1 = -1.0 * left.velocity.x;
		double c1 = left.velocity.y * left.position.x - left.velocity.x * left.position.y;

		double a2 = right.velocity.y;
		double b2 = -1.0 * right.velocity.x;
		double c2 = right.velocity.y * right.position.x - right.velocity.x * right.position.y;

		// Check if lines are parallel
		if (a1 * b2 - a2 * b1 == 0.0)
		{
			return std::make_pair(false, Vector2d<double>{ 0.0, 0.0 });
		}

		// Solve for intersection
		return std::make_pair(
			true, Vector2d<double>{ (c1 * b2 - c2 * b1) / (a1 * b2 - a2 * b1), (c2 * a1 - c1 * a2) / (a1 * b2 - a2 * b1) });
	}

	void PrintSolution(const std::filesystem::path& inputFile, bool shouldRender)
	{
		std::vector<Hailstone> hailstones = ReadInput(inputFile);

		// Sample spread
		//constexpr Vector2d<double> testSpread = { 7.0, 27.0 };
		
		// Full input spread
		constexpr Vector2d<double> testSpread = { 200000000000000.0, 400000000000000.0 };

		int acc = 0;
		for (int i = 0; i < hailstones.size(); ++i)
		{
			for (int j = i + 1; j < hailstones.size(); ++j)
			{
				const Hailstone& left = hailstones[i];
				const Hailstone& right = hailstones[j];
				auto [intersects, at] = FindIntersection(left, right);
				if (intersects && at.x >= testSpread.x && at.y >= testSpread.x && at.x <= testSpread.y && at.y <= testSpread.y)
				{
					// Check if intersection is in the past for the left hailstone
					Vector2d<double> leftOffset = at - Vector2d<double>{ left.position.x, left.position.y };
					if (leftOffset.x < 0 && left.velocity.x > 0 || leftOffset.x > 0 && left.velocity.x < 0
					    || leftOffset.y < 0 && left.velocity.y > 0 || leftOffset.y > 0 && left.velocity.y < 0)
					{
						continue;
					}

					// Check if the intersection is in the past for the right hailstone
					Vector2d<double> rightOffset = at - Vector2d<double>{ right.position.x, right.position.y };
					if (rightOffset.x < 0 && right.velocity.x > 0 || rightOffset.x > 0 && right.velocity.x < 0
					    || rightOffset.y < 0 && right.velocity.y > 0 || rightOffset.y > 0 && right.velocity.y < 0)
					{
						continue;
					}

					++acc;
				}
			}
		}

		std::cout << acc;
	}
} // namespace Puzzle24A