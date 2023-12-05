#include "pch.h"

#include "../Utilities/Utilities.h"
#include "PuzzleSolvers.h"

using namespace std::literals::chrono_literals;
using namespace Utilities;

namespace Puzzle05B
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

	struct SeedRange
	{
		int64_t start = 0;
		int64_t length = 0;
	};

	std::pair<std::vector<SeedRange>, std::vector<Map>> ReadInput(const std::filesystem::path& inputFile)
	{
		std::vector<std::string> input = ReadAllLinesInFile(inputFile);

		// Read seeds from the first line.
		std::vector<int64_t> seeds = SplitSpaceSeparatedInt64s(input[0].substr(input[0].find(':') + 2));
		std::vector<SeedRange> seedRanges;
		for (int i = 0; i < seeds.size(); i += 2)
		{
			seedRanges.emplace_back(seeds[i], seeds[i + 1]);
		}

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

		return std::make_pair(seedRanges, maps);
	}

	Map MergeMaps(std::vector<Map> maps)
	{
		// Rewrite each map to cover the entire source number space
		for (Map& map : maps)
		{
			std::ranges::sort(map.entries, {}, &MapEntry::source);
			std::vector<MapEntry> newEntries = {};

			int64_t i = 0;
			for (const MapEntry& entry : map.entries)
			{
				// Add entry for numbers between current position and the next entry
				if (i < entry.source)
				{
					newEntries.emplace_back(i, i, entry.source - i);
					i += entry.source - i;
				}

				// Copy the current entry
				newEntries.emplace_back(entry);
				i += entry.length;
			}

			// Cover any remaining numbers
			newEntries.emplace_back(i, i, std::numeric_limits<int64_t>::max() - i);
			map.entries.swap(newEntries);
		}

		// Work backwards through the maps merging them together such that the left map's sources map to the right map's destinations
		while (maps.size() > 1)
		{
			Map right = maps.back();
			maps.pop_back();

			Map left = maps.back();
			maps.pop_back();

			Map merged;
			std::stack<MapEntry> entriesToProcess(left.entries.rbegin(), left.entries.rend());
			while (!entriesToProcess.empty())
			{
				const MapEntry& entry = entriesToProcess.top();
				entriesToProcess.pop();

				// Find first entry in the right map whose source range intersects with the current entry's destination range
				int iIntersection = 0;
				while (entry.destination < right.entries[iIntersection].source
				       || right.entries[iIntersection].source + right.entries[iIntersection].length <= entry.destination)
				{
					++iIntersection;
				}

				const MapEntry& intersection = right.entries[iIntersection];
				int64_t offset = entry.destination - intersection.source;

				// If the entry is fully contained within the intersecting range...
				if (entry.destination + entry.length <= intersection.source + intersection.length)
				{
					// ... then remap to the new destination
					merged.entries.emplace_back(intersection.destination + offset, entry.source, entry.length);
					continue;
				}

				// If the entry spills over the intersecting range, split for the intersection...
				int64_t choppedLength = intersection.length - offset;
				merged.entries.emplace_back(intersection.destination + offset, entry.source, choppedLength);

				// ... and process the remaining section later
				entriesToProcess.emplace(entry.destination + choppedLength, entry.source + choppedLength, entry.length - choppedLength);
			}

			maps.push_back(merged);
		}

		return maps[0];
	}

	int64_t FindMinLocation(const SeedRange& range, const Map& map)
	{
		// Find the best location for each entry
		int64_t minLocation = std::numeric_limits<int64_t>::max();
		for (const MapEntry& entry : map.entries)
		{
			int64_t entryStart = entry.source;
			int64_t entryEnd = entry.source + entry.length;
			int64_t rangeStart = range.start;
			int64_t rangeEnd = range.start + range.length;

			// First overlapping position is the lowest final location
			int64_t firstOverlappingSeed = -1;
			if (entryStart <= rangeStart && rangeStart < entryEnd)
			{
				firstOverlappingSeed = rangeStart;
			}
			else if (rangeStart <= entryStart && entryStart < rangeEnd)
			{
				firstOverlappingSeed = entryStart;
			}
			else
			{
				continue;
			}

			// Update minimum location if the current entry has a better location
			int64_t minLocationForEntry = entry.destination + (firstOverlappingSeed - entryStart);
			minLocation = std::min(minLocation, minLocationForEntry);
		}

		return minLocation;
	}

	void PrintSolution(const std::filesystem::path& inputFile, bool /*shouldRender*/)
	{
		auto [seedRanges, maps] = ReadInput(inputFile);
		Map mergedMap = MergeMaps(maps);

		int64_t minLocation = std::numeric_limits<int64_t>::max();
		for (const SeedRange& range : seedRanges)
		{
			minLocation = std::min(minLocation, FindMinLocation(range, mergedMap));
		}

		std::cout << minLocation;
	}
} // namespace Puzzle05B