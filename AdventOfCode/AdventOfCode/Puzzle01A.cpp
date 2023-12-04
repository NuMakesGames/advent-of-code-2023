#include "pch.h"

#include "../Utilities/Utilities.h"
#include "PuzzleSolvers.h"

using namespace Utilities;

namespace Puzzle01A
{
	void RenderCalibrationNumber(std::string_view line, int first, int last, int iFirst, int iLast)
	{
		SetConsoleTextColor(ConsoleForegroundColor::IntenseCyan);
		std::cout << first;
		SetConsoleTextColor(ConsoleForegroundColor::IntenseMagenta);
		std::cout << last;

		SetConsoleTextColor(ConsoleForegroundColor::White);
		std::cout << ": ";

		for (int i = 0; i < line.size(); ++i)
		{
			if (i == iFirst)
			{
				SetConsoleTextColor(ConsoleForegroundColor::IntenseCyan);
			}
			else if (i == iLast)
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
			// Find first digit between 1 and 9
			int first = 0;
			int iFirst = -1;
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

			// Find last digit between 1 and 9
			int last = 0;
			int iLast = -1;
			for (int i = static_cast<int>(line.size()) - 1; i >=0; --i)
			{
				char c = line[i];
				if (c >= '1' && c <= '9')
				{
					last = c - '0';
					iLast = i;
					break;
				}
			}

			// Render the line with the first and last digits highlighted, if enabled
			if (shouldRender)
			{
				RenderCalibrationNumber(line, first, last, iFirst, iLast);
			}

			// Capture the two digit calibration number
			calibrationValues.push_back(first * 10 + last);
		}

		// Add all calibration values together
		std::cout << ranges::accumulate(calibrationValues, 0);
	}
} // namespace Puzzle01A