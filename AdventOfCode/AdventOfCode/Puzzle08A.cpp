#include "pch.h"

#include "../Utilities/Utilities.h"
#include "PuzzleSolvers.h"

using namespace Utilities;

namespace Puzzle08A
{
	struct Node
	{
		std::string left;
		std::string right;
	};

	std::pair<std::string, std::unordered_map<std::string, Node>> ReadInput(const std::filesystem::path& inputFile)
	{
		auto input = ReadAllLinesInFile(inputFile);

		// First line is directions
		std::string directions = input[0];

		// Lines 2 and onward are node maps
		std::unordered_map<std::string, Node> nodes;
		for (const std::string& line : input | std::views::drop(2))
		{
			std::string source = line.substr(0, 3);
			VerifyElseCrash(source.size() == 3);

			Node node;
			node.left = line.substr(7, 3);
			node.right = line.substr(12, 3);
			VerifyElseCrash(node.left.size() == 3);
			VerifyElseCrash(node.right.size() == 3);

			nodes.insert({ source, node });
		}

		return std::make_pair(directions, nodes);
	}

	void PrintSolution(const std::filesystem::path& inputFile, bool shouldRender)
	{
		auto [directions, nodes] = ReadInput(inputFile);

		int steps = 0;
		std::string current = "AAA";
		for (char next : directions | ranges::views::cycle)
		{
			++steps;

			// Follow the direction left or right using the map
			current = next == 'L' ? nodes[current].left : nodes[current].right;
			
			// Check if we've found the end
			if (current == "ZZZ")
			{
				break;
			}
		}

		std::cout << steps;
	}
} // namespace Puzzle08A