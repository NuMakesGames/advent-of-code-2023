#include "pch.h"

#include "../Utilities/Utilities.h"
#include "PuzzleSolvers.h"

using namespace Utilities;

namespace Puzzle12B
{
	struct MemoizeKey
	{
		int iGroupStart = 0;
		int iContiguousGroup = 0;

		auto operator<=>(const MemoizeKey& that) const = default;
	};

} // namespace Puzzle12B

IMPLEMENT_STD_HASH(Puzzle12B::MemoizeKey, value.iGroupStart, value.iContiguousGroup);

namespace Puzzle12B
{
	std::pair<std::vector<std::string>, std::vector<std::vector<int>>> ReadInput(const std::filesystem::path& inputFile)
	{
		std::vector<std::string> conditionRecords;
		std::vector<std::vector<int>> contiguousGroups;
		for (const std::string& line : ReadAllLinesInFile(inputFile))
		{
			std::vector<std::string> tokens = SplitString(line, " ");

			// Repeat input 5 times, with additional separators in between each copy
			std::string conditionRecord;
			std::string contiguousGroup;
			for (int i = 0; i < 5; ++i)
			{
				if (i != 0)
				{
					conditionRecord += '?';
					contiguousGroup += ',';
				}
				conditionRecord += tokens[0];
				contiguousGroup += tokens[1];
			}

			conditionRecords.emplace_back(std::move(conditionRecord));
			contiguousGroups.emplace_back(SplitCommaSeparatedInt32s(contiguousGroup));
		}

		return std::make_pair(conditionRecords, contiguousGroups);
	}

	int64_t ComputeValidArrangements(
		const std::string& conditionRecord,
		const std::vector<int>& contiguousGroups,
		std::unordered_map<MemoizeKey, int64_t>& memoizeCache,
		MemoizeKey memoizeKey)
	{
		// If we've already memoized, return cached result
		if (memoizeCache.contains(memoizeKey))
		{
			return memoizeCache[memoizeKey];
		}

		// All groups have been satisfied
		if (memoizeKey.iContiguousGroup >= contiguousGroups.size())
		{
			// If all remaining spaces can be operational
			for (int i = memoizeKey.iGroupStart; i < conditionRecord.size(); ++i)
			{
				if (conditionRecord[i] == '#')
				{
					memoizeCache[memoizeKey] = 0;
					return 0;
				}
			}

			memoizeCache[memoizeKey] = 1;
			return 1;
		}

		// Advance to the starting position
		MemoizeKey updatedKey = memoizeKey;
		while (updatedKey.iGroupStart < conditionRecord.size() && conditionRecord[updatedKey.iGroupStart] == '.')
		{
			++updatedKey.iGroupStart;
		}

		// Check for memoized result with updated key
		if (memoizeCache.contains(updatedKey))
		{
			return memoizeCache[updatedKey];
		}

		// Check for sufficient space
		int groupSize = contiguousGroups[memoizeKey.iContiguousGroup];
		if (updatedKey.iGroupStart + groupSize > conditionRecord.size())
		{
			memoizeCache[memoizeKey] = 0;
			memoizeCache[updatedKey] = 0;
			return 0;
		}

		// Group must end at the end of the string or on a '.' or '?'
		int iGroupEnd = updatedKey.iGroupStart + groupSize;
		bool isGroupValid = iGroupEnd == conditionRecord.size() || conditionRecord[iGroupEnd] != '#';
		for (int i = updatedKey.iGroupStart; i < iGroupEnd; ++i)
		{
			if (conditionRecord[i] == '.')
			{
				isGroupValid = false;
				break;
			}
		}

		// If the group is valid, compute the number of ways to arrange the tail
		int64_t arrangements = 0;
		if (isGroupValid)
		{
			// Next event starts 1 past the end of the group, because that position must be an operational spring
			MemoizeKey nextKey{ iGroupEnd + 1, memoizeKey.iContiguousGroup + 1 };
			arrangements = ComputeValidArrangements(conditionRecord, contiguousGroups, memoizeCache, nextKey);
		}

		// If start position's spring state is unknown, also consider other start positions for the group
		if (conditionRecord[updatedKey.iGroupStart] == '?')
		{
			MemoizeKey alternateKey{ updatedKey.iGroupStart + 1, memoizeKey.iContiguousGroup };
			arrangements += ComputeValidArrangements(conditionRecord, contiguousGroups, memoizeCache, alternateKey);
		}

		// Memoize the final result
		memoizeCache[memoizeKey] = arrangements;
		return arrangements;
	}

	int64_t ComputeValidArrangements(const std::string& conditionRecord, const std::vector<int>& contiguousGroups)
	{
		std::unordered_map<MemoizeKey, int64_t> memoizeCache;
		MemoizeKey memoizeKey;
		return ComputeValidArrangements(conditionRecord, contiguousGroups, memoizeCache, memoizeKey);
	}

	void PrintSolution(const std::filesystem::path& inputFile, bool /*shouldRender*/)
	{
		auto [conditionRecords, contiguousGroups] = ReadInput(inputFile);
		std::cout << ranges::accumulate(
			std::views::zip(conditionRecords, contiguousGroups),
			0ll,
			[](int64_t acc, const auto& zipView) { return acc + ComputeValidArrangements(std::get<0>(zipView), std::get<1>(zipView)); });
	}
} // namespace Puzzle12B