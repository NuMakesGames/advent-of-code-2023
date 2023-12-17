#include "pch.h"

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/breadth_first_search.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/property_map/property_map.hpp>

#include "../Utilities/Utilities.h"
#include "PuzzleSolvers.h"

using namespace Utilities;
using namespace std::literals::chrono_literals;

using Graph =
	boost::adjacency_list<boost::listS, boost::vecS, boost::directedS, boost::no_property, boost::property<boost::edge_weight_t, int>>;
using Vertex = boost::graph_traits<Graph>::vertex_descriptor;

namespace Puzzle17B
{
	constexpr int nodesPerPosition = 40;
	constexpr int minBlocks = 4;
	constexpr int maxBlocks = 10;

	constexpr Vector2d<int> up = { 0, -1 };
	constexpr Vector2d<int> down = { 0, 1 };
	constexpr Vector2d<int> left = { -1, 0 };
	constexpr Vector2d<int> right = { 1, 0 };
	std::array<Vector2d<int>, 4> directions = { up, down, left, right };

	struct Node
	{
		Vector2d<int> position;
		Vector2d<int> direction = up;
		int remainingBlocks = maxBlocks - 1;
	};

	// Given a node, compute its vertex index
	int GetIndexForNode(const Grid2d<int>& grid, const Node& node)
	{
		if (!grid.IsInBounds(node.position))
		{
			return -1;
		}

		int iPosition = grid.GetIndexFromCoordinates(node.position.x, node.position.y);
		int iDirection = static_cast<int>(std::ranges::find(directions, node.direction) - directions.begin());
		return iPosition * nodesPerPosition + iDirection * maxBlocks + node.remainingBlocks;
	}

	// Given a vertex index, compute the node's state
	Node GetNodeForIndex(const Grid2d<int>& grid, int i)
	{
		int iPosition = i / nodesPerPosition;
		int iDirection = (i - nodesPerPosition * iPosition) / maxBlocks;
		int remainingBlocks = i - nodesPerPosition * iPosition - iDirection * maxBlocks;
		return { grid.GetCoordinatesFromIndex(iPosition), directions[iDirection], remainingBlocks };
	}

	// Generate connected nodes for a given node
	std::vector<Node> GetConnectedNodes(const Grid2d<int>& grid, const Node& currentNode)
	{
		std::vector<Node> connectedNodes;

		// Connect straight ahead if we can still go straight
		if (currentNode.remainingBlocks != 0)
		{
			connectedNodes.emplace_back(
				currentNode.position + currentNode.direction, currentNode.direction, currentNode.remainingBlocks - 1);
		}

		// Ultra crucible can only turn if it's traveled at least min blocks
		if (currentNode.remainingBlocks <= maxBlocks - minBlocks)
		{
			// Connect each available turn based on direction
			if (currentNode.direction == up || currentNode.direction == down)
			{
				connectedNodes.emplace_back(currentNode.position + left, left, maxBlocks - 1);
				connectedNodes.emplace_back(currentNode.position + right, right, maxBlocks - 1);
			}
			else
			{
				connectedNodes.emplace_back(currentNode.position + up, up, maxBlocks - 1);
				connectedNodes.emplace_back(currentNode.position + down, down, maxBlocks - 1);
			}
		}

		return connectedNodes;
	}

	void RenderPath(const Grid2d<int>& grid, const std::vector<Vertex>& predecessors, Vertex source, Vertex destination)
	{
		ConsoleRenderer renderer{ grid.Width(), grid.Height(), 1 };
		renderer.SetTimeToWaitAfterPresent(1ms);
		renderer.SetRestoreCursorAfterPresent(false);
		for (int y = 0; y < grid.Height(); ++y)
		{
			for (int x = 0; x < grid.Width(); ++x)
			{
				renderer.Draw(x, y, grid.at(x, y) + '0', ConsoleForegroundColor::White);
			}
		}
		renderer.Present();

		std::vector<Vertex> path;
		for (Vertex v = predecessors[destination]; v != source; v = predecessors[v])
		{
			path.push_back(v);
		}

		for (Vertex v : path | std::views::reverse)
		{
			if (v == source || v == destination)
			{
				continue;
			}

			Node node = GetNodeForIndex(grid, static_cast<int>(v));
			if (node.position.x == 0 && node.position.y == 0)
			{
				continue;
			}

			char directionCharacter = '>';
			if (node.direction == up)
			{
				directionCharacter = '^';
			}
			else if (node.direction == down)
			{
				directionCharacter = 'V';
			}
			else if (node.direction == left)
			{
				directionCharacter = '<';
			}
			renderer.Draw(node.position, directionCharacter, ConsoleForegroundColor::IntenseRed);
			renderer.Present();
		}

		renderer.MoveCursorBelowRenderer();
	}

	void PrintSolution(const std::filesystem::path& inputFile, bool shouldRender)
	{
		Grid2d<int> grid = ReadAllLinesInFileAsGridOfInts(inputFile);

		// Create nodesPerPosition vertices per position in grid
		Graph graph;
		for (int i = 0; i < grid.size(); ++i)
		{
			for (int j = 0; j < nodesPerPosition; ++j)
			{
				boost::add_vertex(graph);
			}
		}

		// Create edges for each node
		for (int i = 0; i < grid.size(); ++i)
		{
			for (int j = 0; j < nodesPerPosition; ++j)
			{
				int iCurrentNode = i * nodesPerPosition + j;
				Node currentNode = GetNodeForIndex(grid, iCurrentNode);
				for (const Node& connectedNode : GetConnectedNodes(grid, currentNode))
				{
					int iConnectedNode = GetIndexForNode(grid, connectedNode);
					if (iConnectedNode < 0)
					{
						continue;
					}
					boost::add_edge(iCurrentNode, iConnectedNode, grid.at(connectedNode.position), graph);
				}
			}
		}

		// Add target node connected to all bottom right position nodes that have traveled at least minBlocks since last turn
		Vertex destination = boost::add_vertex(graph);
		for (int i = 0; i < nodesPerPosition; ++i)
		{
			Node node = GetNodeForIndex(grid, i);
			if (node.remainingBlocks <= maxBlocks - minBlocks)
			{
				boost::add_edge((grid.size() - 1) * nodesPerPosition + i, destination, 0, graph);
			}
		}

		// Add source node connected to all top left position nodes that will travel at least minBlocks before turning
		Vertex source = boost::add_vertex(graph);
		for (int i = 0; i < nodesPerPosition; ++i)
		{
			Node node = GetNodeForIndex(grid, i);
			if (node.remainingBlocks >= 2 * minBlocks)
			{
				boost::add_edge(source, i, 0, graph);
			}
		}

		// Find the shortest path from source to destination
		std::vector<Vertex> predecessors(boost::num_vertices(graph));
		std::vector<int> distances(boost::num_vertices(graph));
		boost::dijkstra_shortest_paths(graph, source, boost::distance_map(distances.data()).predecessor_map(predecessors.data()));

		// Render path visualization, if requested
		if (shouldRender)
		{
			RenderPath(grid, predecessors, source, destination);
		}

		// Report the shortest path distance
		std::cout << distances[destination];
	}
} // namespace Puzzle17B