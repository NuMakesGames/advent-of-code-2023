#include "pch.h"

#include "../Utilities/Utilities.h"
#include "PuzzleSolvers.h"

using namespace Utilities;

namespace Puzzle12A
{
	// Scraped from input
	constexpr int conditionRecordMaxSize = 32;

	struct Arrangement
	{
		// 1 for damaged, 0 for operational
		std::bitset<conditionRecordMaxSize> springs;

		// Unknown springs >= iEnd are set to operational
		int iEnd = 0;

		// Springs left to be selected
		int remainingUnknownSprings = 0;

		// Insertion operator overload.
		friend std::ostream& operator<<(std::ostream& stream, const Arrangement& arrangement)
		{
			for (int i = 0; i < arrangement.iEnd; ++i)
			{
				stream << (arrangement.springs.test(i) ? '#' : '.');
			}
			return stream;
		}
	};

	struct IsValidArrangementResult
	{
		bool isValid = true;
		bool isComplete = true;
	};

	std::pair<std::vector<std::string>, std::vector<std::vector<int>>> ReadInput(const std::filesystem::path& inputFile)
	{
		std::vector<std::string> conditionRecords;
		std::vector<std::vector<int>> contiguousGroups;
		for (const std::string& line : ReadAllLinesInFile(inputFile))
		{
			std::vector<std::string> tokens = SplitString(line, " ");
			conditionRecords.emplace_back(std::move(tokens[0]));
			contiguousGroups.emplace_back(SplitCommaSeparatedInt32s(tokens[1]));
		}

		return std::make_pair(conditionRecords, contiguousGroups);
	}

	IsValidArrangementResult IsValidArrangement(
		const std::string& conditionRecord,
		const std::vector<int>& contiguousGroups,
		int expectedDamagedSprings,
		const Arrangement& arrangement)
	{
		IsValidArrangementResult result;

		// Complete when we've assigned all damaged springs
		result.isComplete = arrangement.iEnd == conditionRecord.size();

		if (!result.isComplete)
		{
			// Still valid only if enough potential damaged springs remain
			int damagedSprings = static_cast<int>(arrangement.springs.count());
			result.isValid = expectedDamagedSprings <= damagedSprings + arrangement.remainingUnknownSprings;
			if (!result.isValid)
			{
				return result;
			}
		}

		bool isParsingGroup = false;
		int iGroupStart = -1;
		int iContiguousGroup = 0;
		for (int i = 0; i < arrangement.iEnd; ++i)
		{
			// Start counting a group of damaged springs
			if (arrangement.springs.test(i) && !isParsingGroup)
			{
				isParsingGroup = true;
				iGroupStart = i;
				continue;
			}

			// Stop counting the current group of damage springs; check its size matches
			if (!arrangement.springs.test(i) && isParsingGroup)
			{
				isParsingGroup = false;
				int size = i - iGroupStart;
				if (iContiguousGroup >= contiguousGroups.size())
				{
					result.isValid = false;
					return result;
				}

				// Size should be correct, even for incomplete groups as we're only checking through the end of the arrangement
				int expectedSize = contiguousGroups[iContiguousGroup++];
				if (size != expectedSize)
				{
					result.isValid = false;
					return result;
				}
			}
		}

		// Close out the current group, if one is outstanding
		if (isParsingGroup)
		{
			int size = arrangement.iEnd - iGroupStart;
			if (iContiguousGroup >= contiguousGroups.size())
			{
				result.isValid = false;
				return result;
			}

			// Size may not be correct here, if the result is incomplete
			int expectedSize = contiguousGroups[iContiguousGroup++];
			if (size > expectedSize || result.isComplete && size != expectedSize)
			{
				result.isValid = false;
				return result;
			}
		}

		// Arrangement should either be incomplete or should match the expected number of groups
		result.isValid = !result.isComplete || iContiguousGroup == contiguousGroups.size();
		return result;
	}

	int64_t ComputeValidArrangements(
		const std::string& conditionRecord,
		const std::vector<int>& contiguousGroups,
		bool shouldRender,
		int expectedDamagedSprings,
		Arrangement arrangement)
	{
		// Initialize arrangement
		if (arrangement.iEnd == 0)
		{
			arrangement.iEnd = static_cast<int>(conditionRecord.find_first_of('?'));
			for (int i = 0; i < conditionRecord.size(); ++i)
			{
				if (conditionRecord[i] == '#')
				{
					arrangement.springs.set(i);
				}
				else if (conditionRecord[i] == '?')
				{
					++arrangement.remainingUnknownSprings;
				}
			}
		}

		// If the arrangement is already invalid, no need to continue
		IsValidArrangementResult validArrangement =
			IsValidArrangement(conditionRecord, contiguousGroups, expectedDamagedSprings, arrangement);
		if (!validArrangement.isValid)
		{
			// Render additional details, if requested
			if (shouldRender)
			{
				ScopedConsoleTextColor textColor(ConsoleForegroundColor::IntenseRed);
				std::cout << "Invalid: " << arrangement << '\n';
			}
			return 0;
		}

		// If the arrangement is valid and complete, we're done
		if (validArrangement.isComplete)
		{
			// Render additional details, if requested
			if (shouldRender)
			{
				ScopedConsoleTextColor textColor(ConsoleForegroundColor::IntenseGreen);
				std::cout << "Valid:   " << arrangement << '\n';
			}
			return 1;
		}

		// Recursively try all possible arrangements
		int iSpring = arrangement.iEnd;
		size_t iSpringNext = conditionRecord.find_first_of('?', iSpring + 1);
		arrangement.iEnd = static_cast<int>(iSpringNext == std::string::npos ? conditionRecord.size() : iSpringNext);

		// Set the next spring
		--arrangement.remainingUnknownSprings;

		// Assume the spring is operational
		int64_t operationalArrangements =
			ComputeValidArrangements(conditionRecord, contiguousGroups, shouldRender, expectedDamagedSprings, arrangement);

		// Assume the spring is damaged
		arrangement.springs.set(iSpring);
		int64_t damagedArrangements =
			ComputeValidArrangements(conditionRecord, contiguousGroups, shouldRender, expectedDamagedSprings, arrangement);

		return damagedArrangements + operationalArrangements;
	}

	int64_t ComputeValidArrangements(const std::string& conditionRecord, const std::vector<int>& contiguousGroups, bool shouldRender)
	{
		return ComputeValidArrangements(
			conditionRecord, contiguousGroups, shouldRender, ranges::accumulate(contiguousGroups, 0), Arrangement{});
	}

	void PrintSolution(const std::filesystem::path& inputFile, bool shouldRender)
	{
		auto [conditionRecords, contiguousGroups] = ReadInput(inputFile);

		std::vector<int64_t> arrangementCounts;
		for (const auto& [record, groups] : std::views::zip(conditionRecords, contiguousGroups))
		{
			arrangementCounts.emplace_back(ComputeValidArrangements(record, groups, shouldRender));

			// Render additional details, if requested
			if (shouldRender)
			{
				ScopedConsoleTextColor textColor(ConsoleForegroundColor::IntenseYellow);
				std::cout << arrangementCounts.back() << ": " << record << " \t";
				for (int group : groups)
				{
					std::cout << group << " ";
				}
				std::cout << "\n\n";
			}
		}

		std::cout << ranges::accumulate(arrangementCounts, 0ll);
	}
} // namespace Puzzle12A