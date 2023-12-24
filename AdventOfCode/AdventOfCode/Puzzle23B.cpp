#include "pch.h"

#include "../Utilities/Utilities.h"
#include "PuzzleSolvers.h"

using namespace Utilities;

namespace Puzzle23B
{
	constexpr Vector2d<int> up = { 0, -1 };
	constexpr Vector2d<int> down = { 0, 1 };
	constexpr Vector2d<int> left = { -1, 0 };
	constexpr Vector2d<int> right = { 1, 0 };
	constexpr std::array<Vector2d<int>, 4> directions = { up, down, left, right };

	// Search state for DFS through the graph
	struct GraphSearchState
	{
		int iNodeCurrent = 0;
		int totalWeight = 0;
		uint64_t visited = 0;
	};

	// Edges in the graph
	struct Edge
	{
		int iTargetNode = 0;
		int weight = 0;
	};

	// Nodes in the graph
	struct Node
	{
		Vector2d<int> position;
		std::vector<Edge> edges;
	};

	// Simple check for how many empty spaces are around a given position; used to find nodes in the graph
	int CountAdjacentSpaces(const Grid2d<char>& grid, const Vector2d<int>& position)
	{
		int count = 0;
		for (const Vector2d<int>& direction : directions)
		{
			Vector2d<int> next = position + direction;
			if (grid.IsInBounds(next) && grid.at(next) == '.')
			{
				++count;
			}
		}

		return count;
	}

	// Reduces search space by merging runs as weighted graph edges
	std::vector<Node> CompressGridToGraph(const Grid2d<char>& grid, const Vector2d<int>& start, const Vector2d<int>& end)
	{
		std::vector<Node> graph;
		graph.emplace_back(start);
		graph.emplace_back(end);

		// Find all nodes
		for (int i = 0; i < grid.size(); ++i)
		{
			Vector2d<int> position = grid.GetCoordinatesFromIndex(i);
			if (grid.at(position) != '.')
			{
				continue;
			}

			if (position == start || position == end)
			{
				continue;
			}

			// Any position with 3 or more adjacent free spaces is a node in the graph
			if (CountAdjacentSpaces(grid, position) >= 3)
			{
				graph.emplace_back(position);
			}
		}

		// Find all edges
		for (int i = 0; i < graph.size(); ++i)
		{
			Node& node = graph[i];

			// BFS from the current node to discover all adjacent nodes
			Grid2d<int> visited(grid.Width(), grid.Height());
			std::ranges::fill(visited, -1);
			visited.at(node.position) = 0;

			// Setup initial queue
			std::queue<std::pair<Vector2d<int>, Vector2d<int>>> toExplore;
			for (const Vector2d<int>& direction : directions)
			{
				toExplore.push({ node.position, direction });
			}
			
			// Run BFS
			while (!toExplore.empty())
			{
				auto [current, direction] = toExplore.front();
				toExplore.pop();

				Vector2d<int> next = current + direction;
				if (!grid.IsInBounds(next) || visited.at(next) >= 0 || grid.at(next) != '.')
				{
					continue;
				}

				visited.at(next) = visited.at(current) + 1;

				// If we've found a node, create the edge and stop searching on this path
				auto nodeIt = std::ranges::find(graph, next, &Node::position);
				if (nodeIt != graph.end())
				{
					int iTargetNode = static_cast<int>(nodeIt - graph.begin());

					// Add edge if we haven't already discovered it
					if (!std::ranges::any_of(node.edges, [iTargetNode](const Edge& edge) { return edge.iTargetNode == iTargetNode; }))
					{
						node.edges.push_back({ iTargetNode, visited.at(next) });
						graph[iTargetNode].edges.push_back({ i, visited.at(next) });
					}

					continue;
				}

				// Continue searching adjacent positions
				for (const Vector2d<int>& direction : directions)
				{
					toExplore.push({ next, direction });
				}
			}
		}

		return graph;
	}

	void PrintSolution(const std::filesystem::path& inputFile, bool shouldRender)
	{
		Grid2d<char> grid = ReadAllLinesInFileAsGrid(inputFile);

		// Replace all slopes with normal positions
		for (char& c : grid)
		{
			switch (c)
			{
				case '^':
				case 'v':
				case '<':
				case '>':
					c = '.';
					break;
			}
		}

		// Define start and end positions
		Vector2d<int> start{ 1, 0 };
		Vector2d<int> end{ grid.Width() - 2, grid.Height() - 1 };
		VerifyElseCrash(grid.at(start) == '.' && grid.at(end) == '.');

		std::vector<Node> graph = CompressGridToGraph(grid, start, end);

		// Verify that the graph is small enough to cache visited nodes in a 64-bit bitmask
		VerifyElseCrash(graph.size() < sizeof(uint64_t) * 8);

		// Indices for start and end nodes
		int iNodeStart = 0;
		int iNodeEnd = static_cast<int>(std::ranges::find(graph, end, &Node::position) - graph.begin());

		// Find the longest path
		int longestPath = 0;
		uint64_t visited = 0ull;

		// Setup a stack to run DFS
		std::stack<GraphSearchState> toExplore;
		toExplore.push({ iNodeStart, 0, visited });
		while (!toExplore.empty())
		{
			auto [iNode, totalWeight, visited] = toExplore.top();
			toExplore.pop();

			// If we've reached the end, check if this path is longer than the longest path we've found so far
			if (iNode == iNodeEnd)
			{
				longestPath = std::max(totalWeight, longestPath);
				continue;
			}

			// Explore all edges
			for (const Edge& edge : graph[iNode].edges)
			{
				int iNextNode = edge.iTargetNode;
				uint64_t nextVisited = visited;

				// If we've already visited this node, skip it
				if (nextVisited & (1ull << iNextNode))
				{
					continue;
				}

				// Mark the node visited and continue exploring
				nextVisited |= 1ull << iNextNode;
				toExplore.push({ iNextNode, totalWeight + edge.weight, nextVisited });
			}
		}

		std::cout << longestPath;
	}
} // namespace Puzzle23B