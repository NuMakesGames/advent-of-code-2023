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

	std::vector<std::string> SplitString(const std::string& input, const std::string& delimiter)
	{
		return Utilities::SplitStringAndTransform<std::string>(input, delimiter, [](const std::string& token) { return token; });
	}

	std::vector<int> SplitCommaSeparatedInt32s(const std::string& input)
	{
		return Utilities::SplitStringAndTransform<int>(input, ",", [](const std::string& token) { return std::stoi(token); });
	}

	std::vector<int64_t> SplitCommaSeparatedInt64s(const std::string& input)
	{
		return Utilities::SplitStringAndTransform<int64_t>(input, ",", [](const std::string& token) { return std::stoll(token); });
	}

} // namespace Utilities
