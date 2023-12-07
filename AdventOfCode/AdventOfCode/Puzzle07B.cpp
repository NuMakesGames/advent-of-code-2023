#include "pch.h"

#include "../Utilities/Utilities.h"
#include "PuzzleSolvers.h"

using namespace Utilities;

namespace Puzzle07B
{
	enum class Type
	{
		HighCard,
		OnePair,
		TwoPairs,
		ThreeOfAKind,
		FullHouse,
		FourOfAKind,
		FiveOfAKind,
	};

	struct Hand
	{
		std::string cards;
		int bid = 0;
	};

	std::vector<Hand> ReadInput(const std::filesystem::path& inputFile)
	{
		// Read hands and bids
		std::vector<Hand> hands;
		for (const auto& line : ReadAllLinesInFile(inputFile))
		{
			auto split = SplitString(line, " ");
			hands.emplace_back(split[0], std::stoi(split[1]));
		}

		return hands;
	}

	Type GetHandType(const std::string& hand)
	{
		// Count the number of each card
		std::unordered_map<char, int> cardCounts;
		for (const auto& card : hand)
		{
			cardCounts[card]++;
		}

		// Then count the number of each count (ex. countCounts[2] == 2 means 2 pairs)
		std::unordered_map<int, int> countCounts;
		for (const auto& [card, count] : cardCounts)
		{
			// Don't count jokers in the count
			if (card == 'J')
			{
				continue;
			}

			countCounts[count]++;
		}

		// Determine type based on counts of counts and jokers
		switch (cardCounts['J'])
		{
			// Regular rules apply
			case 0:
				if (countCounts[5] == 1)
				{
					return Type::FiveOfAKind;
				}
				else if (countCounts[4] == 1)
				{
					return Type::FourOfAKind;
				}
				else if (countCounts[3] == 1 && countCounts[2] == 1)
				{
					return Type::FullHouse;
				}
				else if (countCounts[3] == 1)
				{
					return Type::ThreeOfAKind;
				}
				else if (countCounts[2] == 2)
				{
					return Type::TwoPairs;
				}
				else if (countCounts[2] == 1)
				{
					return Type::OnePair;
				}
				else
				{
					return Type::HighCard;
				}
			case 1:
				if (countCounts[4] == 1)
				{
					return Type::FiveOfAKind;
				}
				else if (countCounts[3] == 1)
				{
					return Type::FourOfAKind;
				}
				else if (countCounts[2] == 2)
				{
					return Type::FullHouse;
				}
				else if (countCounts[2] == 1)
				{
					return Type::ThreeOfAKind;
				}
				else
				{
					return Type::OnePair;
				}
			case 2:
				if (countCounts[3] == 1)
				{
					return Type::FiveOfAKind;
				}
				else if (countCounts[2] == 1)
				{
					return Type::FourOfAKind;
				}
				else
				{
					return Type::ThreeOfAKind;
				}
			case 3:
				if (countCounts[2] == 1)
				{
					return Type::FiveOfAKind;
				}
				else
				{
					return Type::FourOfAKind;
				}
			case 4:
				return Type::FiveOfAKind;
			case 5:
				return Type::FiveOfAKind;
		}

		VerifyNotReached();
		return Type::HighCard;
	}

	void PrintSolution(const std::filesystem::path& inputFile, bool shouldRender)
	{
		auto input = ReadInput(inputFile);

		// Sort hands by increasing rank
		std::ranges::sort(
			input,
			[](const auto& lhs, const auto& rhs)
			{
				Type leftType = GetHandType(lhs.cards);
				Type rightType = GetHandType(rhs.cards);

				// If the types are the same, tie break on card strength by position
				if (leftType == rightType)
				{
					static const std::vector<char> strengths = { 'J', '2', '3', '4', '5', '6', '7', '8', '9', 'T', 'Q', 'K', 'A' };
					for (int i = 0; i < lhs.cards.size(); ++i)
					{
						// Continue if cards are the same
						char leftCard = lhs.cards[i];
						char rightCard = rhs.cards[i];
						if (leftCard == rightCard)
						{
							continue;
						}

						// Otherwise, return the hand with the higher card strength
						return std::ranges::find(strengths, leftCard) < std::ranges::find(strengths, rightCard);
					}
				}

				// If types are different, return the hand with the higher type
				return leftType < rightType;
			});

		// Calculate winnings by multiplying bid by rank
		int64_t acc = 0;
		for (int i = 0; i < input.size(); ++i)
		{
			acc += input[i].bid * (i + 1);
		}
		std::cout << acc;
	}
} // namespace Puzzle07B