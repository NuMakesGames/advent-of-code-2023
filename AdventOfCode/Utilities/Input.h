#pragma once

#include "Grid2d.h"

namespace Utilities
{
	// Reads the provided text file and returns all lines as strings.
	std::vector<std::string> ReadAllLinesInFile(const std::filesystem::path& path);

	// Reads the provided text file and returns a 2d grid of characters
	Grid2d<char> ReadAllLinesInFileAsGrid(const std::filesystem::path& path);

	// Writes the provided lines to the specified text file, overwriting if it exists.
	bool WriteAllLinesToFile(const std::filesystem::path& path, const std::vector<std::string>& lines);

	// Parses the input string into tokens separated by the provided delimiter.
	std::vector<std::string> SplitString(const std::string& input, const std::string& delimiter);

	// Parses comma-separated 32-bit integers.
	std::vector<int> SplitCommaSeparatedInt32s(const std::string& input);

	// Parses comma-separated 64-bit integers.
	std::vector<int64_t> SplitCommaSeparatedInt64s(const std::string& input);

	// Parses the input string into tokens separated by the provided delimiter and applies the supplied
	// transform to each token.
	template<typename T>
	std::vector<T> SplitStringAndTransform(
		const std::string& input,
		const std::string& delimiter,
		std::function<T(const std::string&)> transform)
	{
		return std::views::split(input, delimiter)
			| std::views::transform([](const auto& subrange){ return std::string{ subrange.begin(), subrange.end() }; })
			| std::views::transform(transform)
			| std::ranges::to<std::vector>();
	}
}