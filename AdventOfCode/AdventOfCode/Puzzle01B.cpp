#include "pch.h"

#include "../Utilities/Utilities.h"
#include "PuzzleSolvers.h"

using namespace Utilities;

namespace Puzzle01B
{
	void PrintSolution()
	{
		std::vector<int> calibrationValues;
		for (const std::string& line : ReadAllLinesInFile("Puzzle01.input"))
		{
			// Search for first and last digits
			int first = 0;
			int last = 0;
			size_t iFirst = line.size();
			size_t iLast = 0;

			// Find the numeric form of the first digit
			for (int i = 0; i < line.size(); ++i)
			{
				char c = line[i];
				if (c >= '1' && c <= '9')
				{
					first = c - '0';
					iFirst = i;
					break;
				}
			}

			// Find the numeric form of the last digit
			for (int i = static_cast<int>(line.size()) - 1; i >= 0; --i)
			{
				char c = line[i];
				if (c >= '1' && c <= '9')
				{
					last = c - '0';
					iLast = i;
					break;
				}
			}

			// Looks for the provided string and updates the first and last digits
			auto searchAndUpdate = [&](std::string_view sv, int value)
			{
				// Search from front of the current line
				size_t iPosFirst = line.find(sv);
				if (iPosFirst != std::string::npos && iPosFirst < iFirst)
				{
					first = value;
					iFirst = static_cast<int>(iPosFirst);
				}

				// Search from back of the current line
				size_t iPosLast = line.rfind(sv);
				if (iPosLast != std::string::npos && iPosLast > iLast)
				{
					last = value;
					iLast = static_cast<int>(iPosLast);
				}
			};

			// Search for each textual digit
			searchAndUpdate("one", 1);
			searchAndUpdate("two", 2);
			searchAndUpdate("three", 3);
			searchAndUpdate("four", 4);
			searchAndUpdate("five", 5);
			searchAndUpdate("six", 6);
			searchAndUpdate("seven", 7);
			searchAndUpdate("eight", 8);
			searchAndUpdate("nine", 9);

			// Capture the two digit calibration number
			calibrationValues.push_back(first * 10 + last);
		}

		// Add all the calibrations values together
		std::cout << ranges::accumulate(calibrationValues, 0);
	}
} // namespace Puzzle01B