#include "pch.h"

#include "../Utilities/Utilities.h"
#include "PuzzleSolvers.h"

using namespace Utilities;

namespace Puzzle09A
{
	std::vector<std::vector<int64_t>> ReadInput(const std::filesystem::path& inputFile)
	{
		return ReadAllLinesInFile(inputFile) | std::views::transform(SplitSpaceSeparatedInt64s) | std::ranges::to<std::vector>();
	}

	void PrintSolution(const std::filesystem::path& inputFile, bool shouldRender)
	{
		std::cout << ranges::accumulate(
			ReadInput(inputFile),
			0ll,
			[](int64_t acc, const std::vector<int64_t>& environmentalReport)
			{
				// Build up layers by tracking differences between pairs of numbers in the previous layer
				std::vector<std::vector<int64_t>> layers;
				layers.push_back(environmentalReport);

				// Continue building layers until a layer contains only 0s
				while (std::ranges::any_of(layers.back(), [](int64_t val) { return val != 0; }))
				{
					std::vector<int64_t> nextLayer;
					for (const auto& valuesSlideView : layers.back() | std::views::slide(2))
					{
						nextLayer.emplace_back(valuesSlideView.back() - valuesSlideView.front());
					}
					layers.emplace_back(std::move(nextLayer));
				}

				// Predict the next value
				std::vector<int64_t> predictions = { 0 };
				for (const std::vector<int64_t>& layer : layers | std::views::reverse | std::views::drop(1))
				{
					predictions.emplace_back(layer.back() + predictions.back());
				}

				// Add the final predicted value to the accumulator
				return acc + predictions.back();
			});
	}
} // namespace Puzzle09A