#include "pch.h"

#include "../Utilities/Utilities.h"
#include "PuzzleSolvers.h"

using namespace Utilities;

namespace Puzzle18A
{
	constexpr Vector2d<int> up = { 0, -1 };
	constexpr Vector2d<int> down = { 0, 1 };
	constexpr Vector2d<int> left = { -1, 0 };
	constexpr Vector2d<int> right = { 1, 0 };

	struct DigInstruction
	{
		Vector2d<int> direction = up;
		int distance = 0;
	};

	std::vector<DigInstruction> ReadInput(const std::filesystem::path& inputFile)
	{
		std::vector<DigInstruction> digInstructions;
		for (const std::string& line : ReadAllLinesInFile(inputFile))
		{
			std::vector<std::string> tokens = SplitString(line, " ");
			DigInstruction digInstruction;
			switch (tokens[0][0])
			{
				case 'R':
					digInstruction.direction = right;
					break;
				case 'L':
					digInstruction.direction = left;
					break;
				case 'U':
					digInstruction.direction = up;
					break;
				case 'D':
					digInstruction.direction = down;
					break;
			}
			digInstruction.distance = std::stoi(tokens[1]);
			digInstructions.push_back(digInstruction);
		}

		return digInstructions;
	}

	void PrintSolution(const std::filesystem::path& inputFile, bool shouldRender)
	{
		std::vector<DigInstruction> digInstructions = ReadInput(inputFile);

		// Pick's thoerem solves for area of a polygon with integer vertex coordinates:
		//   area = interior points + boundary points / 2 - 1
		//
		// Boundary points can be computed by counting the points on the dig path and
		// area can be computed with the shoelace formula. Then:
		//   interior points = area - boundary points / 2 + 1
		//
		// The total number of dug points is the sum of the boundary points and interior points.

		// Find the vertices and the number of boundary points
		int boundaryPoints = 0;
		std::vector<Vector2d<int>> vertices;
		Vector2d<int> currentPosition = { 0, 0 };
		for (const DigInstruction& digInstruction : digInstructions)
		{
			currentPosition += digInstruction.direction * digInstruction.distance;
			boundaryPoints += digInstruction.distance;
			vertices.push_back(currentPosition);
		}

		// Compute the polygon's area using the shoelace formula
		int area = 0;
		for (int i = 0; i < vertices.size(); ++i)
		{
			int j = (i + 1) % vertices.size();
			area += vertices[i].x * vertices[j].y - vertices[j].x * vertices[i].y;
		}
		area /= 2;

		// Compute interior points using the derivation above
		int interiorPoints = area - boundaryPoints / 2 + 1;

		std::cout << boundaryPoints + interiorPoints;
	}
} // namespace Puzzle18A