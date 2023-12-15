#include "pch.h"

#include "../Utilities/Utilities.h"
#include "PuzzleSolvers.h"

using namespace Utilities;

namespace Puzzle15B
{
	struct Segment
	{
		std::string label;
		char operation = 0;
		int focalLength = 0;
	};

	std::vector<Segment> ReadInput(const std::filesystem::path& inputFile)
	{
		std::vector<std::string> input = ReadAllLinesInFile(inputFile);
		return SplitStringAndTransform<Segment>(
			input[0],
			",",
			[](std::string_view str)
			{
				Segment segment;

				size_t iDelimiter = str.find_first_of("-=");
				segment.label = str.substr(0, iDelimiter);
				segment.operation = str[iDelimiter];
				if (segment.operation == '=')
				{
					segment.focalLength = std::stoi(std::string(str.substr(iDelimiter + 1)));
				}

				return segment;
			});
	}

	int Hash(const std::string& str)
	{
		int value = 0;
		for (char c : str)
		{
			value += static_cast<int>(c);
			value *= 17;
			value %= 256;
		}
		return value;
	}

	void PrintSolution(const std::filesystem::path& inputFile, bool shouldRender)
	{
		std::array<std::vector<Segment>, 256> boxes;
		for (const Segment& segment : ReadInput(inputFile))
		{
			int iBox = Hash(segment.label);
			std::vector<Segment>& box = boxes[iBox];
			auto it = std::ranges::find_if(box, [&segment](const Segment& s) { return s.label == segment.label; });
			switch (segment.operation)
			{
				case '-':
					if (it != box.end())
					{
						box.erase(it);
					}
					break;

				case '=':
					if (it != box.end())
					{
						it->focalLength = segment.focalLength;
					}
					else
					{
						box.push_back(segment);
					}
					break;
			}
		}

		int focusingPower = 0;
		for (int i = 0; i < boxes.size(); ++i)
		{
			const std::vector<Segment>& box = boxes[i];
			for (int j = 0; j < box.size(); ++j)
			{
				const Segment& segment = box[j];
				focusingPower += (1 + i) * (1 + j) * segment.focalLength;
			}
		}

		std::cout << focusingPower;
	}
} // namespace Puzzle15B