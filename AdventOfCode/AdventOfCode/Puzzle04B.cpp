#include "pch.h"

#include "../Utilities/Utilities.h"
#include "PuzzleSolvers.h"

using namespace Utilities;

namespace Puzzle04B
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

	void RenderCard(const Card& card, const std::vector<int>& cardCounts, int cardId, size_t wins)
	{
		ScopedConsoleTextColor textColor(ConsoleForegroundColor::IntenseWhite);

		SetConsoleTextColor(ConsoleForegroundColor::IntenseWhite);
		std::cout << "Card " << std::setw(3) << cardId;

		SetConsoleTextColor(ConsoleForegroundColor::White);
		std::cout << " has ";

		SetConsoleTextColor(ConsoleForegroundColor::IntenseGreen);
		std::cout << std::setw(2) << wins << " winning numbers";

		SetConsoleTextColor(ConsoleForegroundColor::White);
		std::cout << ". ";

		SetConsoleTextColor(ConsoleForegroundColor::IntenseYellow);
		std::cout << std::setw(6) << cardCounts[cardId - 1];
		
		SetConsoleTextColor(ConsoleForegroundColor::White);
		std::cout << std::setw(6) << (cardCounts[cardId - 1] > 1 ? " copies " : " copy   ") << " of ";

		SetConsoleTextColor(ConsoleForegroundColor::IntenseWhite);
		std::cout << " card " << std::setw(3) << cardId;

		if (wins > 0)
		{
			SetConsoleTextColor(ConsoleForegroundColor::White);
			std::cout << " generated: ";

			for (int i = cardId + 1; i <= cardId + wins; ++i)
			{
				if (i > cardId + 1)
				{
					SetConsoleTextColor(ConsoleForegroundColor::White);
					std::cout << ", ";
				}

				SetConsoleTextColor(ConsoleForegroundColor::IntenseYellow);
				std::cout << std::setw(6) << cardCounts[cardId - 1];

				SetConsoleTextColor(ConsoleForegroundColor::IntenseWhite);
				std::cout << " card " << std::setw(3) << i;
			}
		}
		else
		{
			SetConsoleTextColor(ConsoleForegroundColor::White);
			std::cout << " generated no additional cards";
		}


		SetConsoleTextColor(ConsoleForegroundColor::White);
		std::cout << "\n";
	}

	void PrintSolution(const std::filesystem::path& inputFile, bool shouldRender)
	{
		std::vector<Card> cards = ReadInput(inputFile);
		std::vector<int> cardCounts(cards.size(), 1);

		for (int i = 0; i < cards.size(); ++i)
		{
			const Card& card = cards[i];
			std::vector<int> wins = card.GetWinners();

			// For each instance of current card, generate a bonus card for each of n subsequent cards, where n is the total number of wins
			for (int j = i + 1; j <= i + wins.size(); ++j)
			{
				cardCounts[j] += cardCounts[i];
			}

			if (shouldRender)
			{
				RenderCard(card, cardCounts, i + 1, wins.size());
			}
		}

		// Count all cards
		std::cout << ranges::accumulate(cardCounts, 0);
	}
} // namespace Puzzle04B