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

	// Finds all indices of the provided delimiter in the string
	std::vector<size_t> FindAllInString(std::string_view input, std::string_view delimiter);

	// Parses the input string into tokens separated by the provided delimiter.
	std::vector<std::string> SplitString(std::string_view input, std::string_view delimiter);

	// Parses comma-separated 32-bit integers.
	std::vector<int> SplitCommaSeparatedInt32s(std::string_view input);

	// Parses comma-separated 64-bit integers.
	std::vector<int64_t> SplitCommaSeparatedInt64s(std::string_view input);

	// Parses the input string into tokens separated by the provided delimiter and applies the supplied
	// transform to each token.
	template<typename T>
	std::vector<T> SplitStringAndTransform(
		std::string_view input, std::string_view delimiter, std::function<T(std::string_view)> transform)
	{
		return std::views::split(input, delimiter)
			| std::views::transform([&transform](const auto& subrange){ return transform(std::string_view{ subrange.begin(), subrange.end() }); })
			| std::ranges::to<std::vector>();
	}
}