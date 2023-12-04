#include "pch.h"

#include "../Utilities/Utilities.h"
#include "PuzzleSolvers.h"

using namespace Utilities;

namespace Puzzle01B
{
	void RenderCalibrationNumber(std::string_view line, int first, int last, size_t iFirst, size_t sizeFirst, size_t iLast, size_t sizeLast)
	{
		SetConsoleTextColor(ConsoleForegroundColor::IntenseCyan);
		std::cout << first;
		SetConsoleTextColor(ConsoleForegroundColor::IntenseMagenta);
		std::cout << last;

		SetConsoleTextColor(ConsoleForegroundColor::White);
		std::cout << ": ";

		for (int i = 0; i < line.size(); ++i)
		{
			if (i >= iFirst && i < iFirst + sizeFirst)
			{
				SetConsoleTextColor(ConsoleForegroundColor::IntenseCyan);
			}
			else if (i >= iLast && i < iLast + sizeLast)
			{
				SetConsoleTextColor(ConsoleForegroundColor::IntenseMagenta);
			}

			std::cout << line[i];
			SetConsoleTextColor(ConsoleForegroundColor::White);
		}

		std::cout << '\n';
	}

	void PrintSolution(const std::filesystem::path& inputFile, bool shouldRender)
	{
		std::vector<int> calibrationValues;
		for (const std::string& line : ReadAllLinesInFile(inputFile))
		{
			// Find the numeric form of the first digit
			int first = 0;
			size_t iFirst = line.size();
			size_t sizeFirst = 0;
			for (int i = 0; i < line.size(); ++i)
			{
				char c = line[i];
				if (c >= '1' && c <= '9')
				{
					first = c - '0';
					iFirst = i;
					sizeFirst = 1;
					break;
				}
			}

			// Find the numeric form of the last digit
			int last = 0;
			size_t iLast = 0;
			size_t sizeLast = 0;
			for (int i = static_cast<int>(line.size()) - 1; i >= 0; --i)
			{
				char c = line[i];
				if (c >= '1' && c <= '9')
				{
					last = c - '0';
					iLast = i;
					sizeLast = 1;
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
					iFirst = iPosFirst;
					sizeFirst = sv.size();
				}

				// Search from back of the current line
				size_t iPosLast = line.rfind(sv);
				if (iPosLast != std::string::npos && iPosLast > iLast)
				{
					last = value;
					iLast = iPosLast;
					sizeLast = sv.size();
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

			// Render the line with the first and last digits highlighted, if enabled
			if (shouldRender)
			{
				RenderCalibrationNumber(line, first, last, iFirst, sizeFirst, iLast, sizeLast);
			}

			// Capture the two digit calibration number
			calibrationValues.push_back(first * 10 + last);
		}

		// Add all the calibrations values together
		std::cout << ranges::accumulate(calibrationValues, 0);
	}
} // namespace Puzzle01B