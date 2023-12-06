#include "pch.h"

#include "Input.h"

namespace Utilities
{
	std::vector<std::string> ReadAllLinesInFile(const std::filesystem::path& path)
	{
		auto lines = std::vector<std::string>{};
		if (!std::filesystem::exists(path))
		{
			return lines;
		}

		auto fileStream = std::ifstream{ path };
		auto currentLine = std::string{};
		while (std::getline(fileStream, currentLine))
		{
			lines.emplace_back(currentLine);
		}

		return lines;
	}

	Grid2d<char> ReadAllLinesInFileAsGrid(const std::filesystem::path& path)
	{
		auto lines = ReadAllLinesInFile(path);
		VerifyElseCrash(lines.size() >= 1);

		int width = static_cast<int>(lines[0].size());
		int height = static_cast<int>(lines.size());
		auto grid = Grid2d<char>{ width, height };

		for (int y = 0; y < grid.Height(); ++y)
		{
			const auto& line = lines[y];
			VerifyElseCrash(line.size() == width);
			for (int x = 0; x < grid.Width(); ++x)
			{
				grid.at(x, y) = line[x];
			}
		}

		return grid;
	}

	bool WriteAllLinesToFile(const std::filesystem::path& path, const std::vector<std::string>& lines)
	{
		auto fileStream = std::ofstream{ path };
		if (!fileStream.good())
		{
			return false;
		}

		for (auto i = 0; i < lines.size(); ++i)
		{
			if (i != 0)
			{
				fileStream << '\n';
			}

			fileStream << lines[i];
		}

		return fileStream.good();
	}

	std::vector<size_t> FindAllInString(std::string_view input, std::string_view delimiter)
	{
		std::vector<size_t> results;
		if (delimiter.empty())
		{
			return results;
		}

		for (size_t i = input.find(delimiter); i != std::string::npos; i = input.find(delimiter, i + 1))
		{
			results.emplace_back(i);
		}

		return results;
	}

	std::vector<std::string> SplitString(std::string_view input, std::string_view delimiter)
	{
		return Utilities::SplitStringAndTransform<std::string>(
			input, delimiter, [](std::string_view token) { return std::string{ token }; });
	}

	std::vector<int> SplitCommaSeparatedInt32s(std::string_view input)
	{
		return Utilities::SplitStringAndTransform<int>(input, ",", [](std::string_view token) { return std::stoi(std::string{ token }); });
	}

	std::vector<int64_t> SplitCommaSeparatedInt64s(std::string_view input)
	{
		return Utilities::SplitStringAndTransform<int64_t>(
			input, ",", [](std::string_view token) { return std::stoll(std::string{ token }); });
	}

	std::vector<int> SplitSpaceSeparatedInt32s(std::string_view input)
	{
		return Utilities::SplitStringAndTransform<int>(input, " ", [](std::string_view token) { return std::stoi(std::string{ token }); });
	}

	std::vector<int64_t> SplitSpaceSeparatedInt64s(std::string_view input)
	{
		return Utilities::SplitStringAndTransform<int64_t>(
			input, " ", [](std::string_view token) { return std::stoll(std::string{ token }); });
	}

	std::vector<int> ExtractInt32s(std::string_view input)
	{
		return ExtractInts<int32_t>(input);
	}

	std::vector<int64_t> ExtractInt64s(std::string_view input)
	{
		return ExtractInts<int64_t>(input);
	}
} // namespace Utilities
