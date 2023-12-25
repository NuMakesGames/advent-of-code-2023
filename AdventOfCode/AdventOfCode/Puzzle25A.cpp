#include "pch.h"

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/stoer_wagner_min_cut.hpp>

#include "../Utilities/Utilities.h"
#include "PuzzleSolvers.h"

using namespace Utilities;

namespace Puzzle25A
{
	typedef boost::
		adjacency_list<boost::vecS, boost::vecS, boost::undirectedS, boost::no_property, boost::property<boost::edge_weight_t, int>>
			Graph;
	typedef boost::property_map<Graph, boost::edge_weight_t>::type WeightMapType;
	typedef boost::property_traits<WeightMapType>::value_type WeightType;

	auto ReadInput(const std::filesystem::path& inputFile)
	{
		std::unordered_set<std::string> vertices;
		std::vector<std::pair<std::string, std::string>> edges;
		for (const std::string& line : ReadAllLinesInFile(inputFile))
		{
			std::vector<std::string> tokens = SplitString(line, ": ");
			std::string source = tokens[0];
			std::vector<std::string> destinations = SplitString(tokens[1], " ");

			vertices.insert(source);
			for (const std::string& destination : destinations)
			{
				vertices.insert(destination);
				edges.push_back(std::make_pair(source, destination));
			}
		}

		return std::make_pair(vertices | std::ranges::to<std::vector>(), edges);
	}

	void PrintSolution(const std::filesystem::path& inputFile, bool shouldRender)
	{
		auto [vertices, edges] = ReadInput(inputFile);

		// Add all vertices to the graph
		Graph g;
		for (const std::string& vertex : vertices)
		{
			boost::add_vertex(g);
		}

		// Add all edges to the graph with weight 1
		for (const auto& [source, destination] : edges)
		{
			size_t iSource = std::distance(vertices.begin(), std::ranges::find(vertices, source));
			size_t iDestination = std::distance(vertices.begin(), std::ranges::find(vertices, destination));
			boost::add_edge(iSource, iDestination, 1, g);
		}

		// define a property map, `parities`, that will store a boolean value for each vertex. Vertices that have the same parity
		// after stoer_wagner_min_cut runs are on the same side of the min-cut.
		BOOST_AUTO(parities, boost::make_one_bit_color_map(num_vertices(g), get(boost::vertex_index, g)));

		// run the Stoer-Wagner algorithm to obtain the min-cut weight and parities is also filled in.
		int w = boost::stoer_wagner_min_cut(g, boost::get(boost::edge_weight, g), boost::parity_map(parities));

		// Count vertices on one side of the min-cut
		int verticesCount = 0;
		for (size_t i = 0; i < boost::num_vertices(g); ++i)
		{
			if (boost::get(parities, i))
			{
				++verticesCount;
			}
		}

		// Multiply the vertices on each side of the min cut
		std::cout << verticesCount * (vertices.size() - verticesCount);
	}
} // namespace Puzzle25A