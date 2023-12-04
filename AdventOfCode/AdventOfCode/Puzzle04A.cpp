#include "pch.h"

#include "../Utilities/Utilities.h"
#include "PuzzleSolvers.h"

using namespace Utilities;

namespace Puzzle04A
{
	struct Card
	{
		std::vector<int> winningNumbers;
		std::vector<int> numbers;

		std::vector<int> GetWinners() const
		{
			// Winners are the intersection of the card's numbers and the winning numbers
			std::vector<int> intersection;
			std::ranges::set_intersection(winningNumbers, numbers, std::back_inserter(intersection));
			return intersection;
		}
	};

	std::vector<Card> ReadInput(const std::filesystem::path& inputFile)
	{
		std::vector<Card> cards;
		std::vector<std::string> input = ReadAllLinesInFile(inputFile);
		for (const std::string& line : input)
		{
			// Parse winning numbers
			size_t iWinningNumbersBegin = line.find(':') + 2;
			size_t iWinningNumbersEnd = line.find('|');
			std::istringstream issWinningNumbers(line.substr(iWinningNumbersBegin, iWinningNumbersEnd - iWinningNumbersBegin));
			std::vector<int> winningNumbers{ std::istream_iterator<int>{ issWinningNumbers }, std::istream_iterator<int>() };
			std::ranges::sort(winningNumbers);

			// Parse card's numbers
			size_t iNumbersBegin = iWinningNumbersEnd + 2;
			std::istringstream issNumbers(line.substr(iWinningNumbersEnd + 2));
			std::vector<int> numbers{ std::istream_iterator<int>{ issNumbers }, std::istream_iterator<int>() };
			std::ranges::sort(numbers);
			
			cards.emplace_back(std::move(winningNumbers), std::move(numbers));
		}

		return cards;
	}

	void RenderCards(const std::vector<Card>& cards)
	{
		ScopedConsoleTextColor textColor(ConsoleForegroundColor::IntenseWhite);
		for (int i = 0; i < cards.size(); ++i)
		{
			const Card& card = cards[i];
			std::unordered_set<int> winners;
			winners.insert_range(card.GetWinners());
			int score = !winners.empty() ? static_cast<int>(std::pow(2, winners.size() - 1)) : 0;

			SetConsoleTextColor(ConsoleForegroundColor::IntenseWhite);
			std::cout << "Card " << std::setw(3) << i + 1;

			SetConsoleTextColor(ConsoleForegroundColor::White);
			std::cout << " is worth ";

			SetConsoleTextColor(ConsoleForegroundColor::IntenseYellow);
			std::cout << std::setw(3) << score << " points";
			
			SetConsoleTextColor(ConsoleForegroundColor::White);
			std::cout << ":";

			for (int number : card.winningNumbers)
			{
				SetConsoleTextColor(winners.contains(number) ? ConsoleForegroundColor::IntenseGreen : ConsoleForegroundColor::White);
				std::cout << " " << std::setw(2) << number;
			}

			SetConsoleTextColor(ConsoleForegroundColor::White);
			std::cout << " |";

			for (int number : card.numbers)
			{
				SetConsoleTextColor(winners.contains(number) ? ConsoleForegroundColor::IntenseGreen : ConsoleForegroundColor::White);
				std::cout << " " << std::setw(2) << number;
			}

			SetConsoleTextColor(ConsoleForegroundColor::White);
			std::cout << "\n";
		}
	}

	void PrintSolution(const std::filesystem::path& inputFile, bool shouldRender)
	{
		std::vector<Card> cards = ReadInput(inputFile);

		if (shouldRender)
		{
			RenderCards(cards);
		}

		std::cout << ranges::accumulate(
			cards,
			0,
			[](int acc, const Card& card)
			{
				// First winning number is worth 1 point; each subsequent winning number doubles the card's value
				std::vector<int> winners = card.GetWinners();
				if (winners.empty())
				{
					return acc;
				}
				return acc + static_cast<int>(std::pow(2, winners.size() - 1));
			});
	}
} // namespace Puzzle04A