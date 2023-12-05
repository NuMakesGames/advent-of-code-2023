#include "pch.h"

#include "../Utilities/Utilities.h"
#include "PuzzleSolvers.h"

using namespace std::literals::chrono_literals;
using namespace Utilities;

namespace Puzzle05B
{
	struct MapEntry
	{
		int id = -1;
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
			maps[iMap].entries.emplace_back(i, tokens[0], tokens[1], tokens[2]);
		}

		return std::make_pair(seedRanges, maps);
	}

	std::mutex coutMutex;

	int64_t FindMinLocation(const SeedRange& range, const std::vector<Map>& maps, int threadId)
	{
		int64_t minLocation = std::numeric_limits<int64_t>::max();

		{
			std::lock_guard<std::mutex> lock(coutMutex);
			std::cout << "Thread " << std::setw(2) << threadId << " evaluating " << std::setw(9) << range.length << " seeds\n";
		}

		auto start = std::chrono::high_resolution_clock::now();

		for (int64_t seed = range.start; seed < range.start + range.length; ++seed)
		{
			// Report progress around every 5 seconds
			int64_t seedIndex = seed - range.start;
			if (seedIndex % 50000000 == 0)
			{
				auto timeSinceLastLog =
					std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - start);
				if (timeSinceLastLog > 5000ms)
				{
					std::lock_guard<std::mutex> lock(coutMutex);
					std::cout << "Thread " << threadId << ": " << static_cast<double>(seedIndex) / range.length * 100.0 << "%\n";
					start = std::chrono::high_resolution_clock::now();
				}
			}

			// Evaluate the seed
			int64_t value = seed;
			for (const Map& map : maps)
			{
				for (const MapEntry& entry : map.entries)
				{
					if (entry.source <= value && value < entry.source + entry.length)
					{
						value = entry.destination + (value - entry.source);
						break;
					}
				}
			}
			minLocation = std::min(minLocation, value);
		}

		{
			std::lock_guard<std::mutex> lock(coutMutex);
			std::cout << "Thread " << std::setw(2) << threadId << " finished\n";
		}

		return minLocation;
	}

	void PrintSolution(const std::filesystem::path& inputFile, bool shouldRender)
	{
		auto [seedRanges, maps] = ReadInput(inputFile);

		int64_t totalLength = ranges::accumulate(seedRanges, 0ull, [](int64_t acc, const SeedRange& range) { return acc + range.length; });
		std::cout << "Total seeds to evaluate: " << totalLength << '\n';

		// Divide the largest range in half until we have a range for each CPU core
		int numThreads = std::thread::hardware_concurrency();
		while (seedRanges.size() < numThreads)
		{
			std::ranges::sort(seedRanges, {}, &SeedRange::length);

			SeedRange range = seedRanges.back();
			seedRanges.pop_back();

			int64_t halfLength = range.length / 2;
			seedRanges.emplace_back(range.start, halfLength);
			seedRanges.emplace_back(range.start + halfLength, range.length - halfLength);
		}

		// Run each range in its own thread
		std::vector<std::thread> threads;
		std::vector<int64_t> results(seedRanges.size(), std::numeric_limits<int64_t>::max());
		for (int i = 0; i < seedRanges.size(); ++i)
		{
			const SeedRange& range = seedRanges[i];
			threads.emplace_back(
				[&results, &maps, i, range]()
				{
					results[i] = FindMinLocation(range, maps, i);
				});
		}
		std::for_each(threads.begin(), threads.end(), [](std::thread& thread) { thread.join(); });

		// Find the smallest location from the results
		std::cout << std::ranges::min(results);
	}
} // namespace Puzzle05B