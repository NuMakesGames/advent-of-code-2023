#include "pch.h"

#include "../Utilities/Utilities.h"
#include "PuzzleSolvers.h"

using namespace Utilities;

namespace Puzzle08B
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

		// Set starting positions to every node that ends with 'A'
		std::vector<std::string> currentNodes = nodes | std::views::keys
		                                      | std::views::filter([](const std::string& str) { return str.back() == 'A'; })
		                                      | std::ranges::to<std::vector>();

		// Track how many steps it takes to get to Z for each node
		std::vector<int64_t> stepsToEnd(currentNodes.size(), 0);

		int64_t steps = 0;
		for (char next : directions | ranges::views::cycle)
		{
			++steps;

			// Evaluate every node
			for (int i = 0; i < currentNodes.size(); ++i)
			{
				// Skip nodes that have already reached Z
				if (stepsToEnd[i] > 0)
				{
					continue;
				}

				// Follow the direction left or right using the map
				std::string& current = currentNodes[i];
				current = next == 'L' ? nodes[current].left : nodes[current].right;

				// Update steps to end if we've found the end for this starting position
				if (current.back() == 'Z')
				{
					stepsToEnd[i] = steps;
				}
			}

			// Check if we've found the end for all starting positions
			if (std::ranges::all_of(stepsToEnd, [](int64_t val) { return val > 0; }))
			{
				break;
			}
		}

		// First time all nodes reach Z will be the lowest common multiple of all steps to their respective end
		std::cout << ranges::accumulate(
			stepsToEnd | std::views::drop(1), stepsToEnd[0], [](int64_t acc, int64_t val) { return std::lcm(acc, val); });
	}
} // namespace Puzzle08B