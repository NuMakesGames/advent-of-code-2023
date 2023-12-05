#include "pch.h"

#include "../Utilities/Utilities.h"
#include "PuzzleSolvers.h"

using namespace Utilities;

namespace Puzzle05A
{
	struct MapEntry
	{
		int64_t destination = 0;
		int64_t source = 0;
		int64_t length = 0;
	};

	struct Map
	{
		std::vector<MapEntry> entries;
	};

	std::pair<std::vector<int64_t>, std::vector<Map>> ReadInput(const std::filesystem::path& inputFile)
	{
		std::vector<std::string> input = ReadAllLinesInFile(inputFile);

		// Read seeds from the first line.
		std::vector<int64_t> seeds = SplitSpaceSeparatedInt64s(input[0].substr(input[0].find(':') + 2));

		// Seven maps in the input
		std::vector<Map> maps;
		maps.resize(7);
		int iMap = 0;

		// Read remaining lines into maps
		for (int i = 2; i < input.size(); ++i)
		{
			const std::string& line = input[i];

			// New lines mean we're starting the next map.
			if (line.empty())
			{
				++iMap;
				continue;
			}

			// Ignore labels.
			if (!std::isdigit(line[0]))
			{
				continue;
			}

			// Read all the ints on the line.
			std::vector<int64_t> tokens = SplitSpaceSeparatedInt64s(line);
			maps[iMap].entries.emplace_back(tokens[0], tokens[1], tokens[2]);
		}

		return std::make_pair(seeds, maps);
	}

	void PrintSolution(const std::filesystem::path& inputFile, bool /*shouldRender*/)
	{
		auto [seeds, maps] = ReadInput(inputFile);
		int64_t minLocation = std::numeric_limits<int64_t>::max();
		for (int64_t location : seeds)
		{
			// Proceed through each map, remapping the value to the new frame of reference
			for (const Map& map : maps)
			{
				for (const MapEntry& entry : map.entries)
				{
					// If no entry matches, the value is unchanged.
					if (entry.source <= location && location < entry.source + entry.length)
					{
						location = entry.destination + (location - entry.source);
						break;
					}
				}
			}
			
			minLocation = std::min(minLocation, location);
		}

		std::cout << minLocation;
	}
} // namespace Puzzle05A