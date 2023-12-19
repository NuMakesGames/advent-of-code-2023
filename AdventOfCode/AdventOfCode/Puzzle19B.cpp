#include "pch.h"

#include "../Utilities/Utilities.h"
#include "PuzzleSolvers.h"

using namespace Utilities;

namespace Puzzle19B
{
	struct Rule
	{
		char variable = 0;
		char op = 0;
		int64_t operand = 0;
		std::string jumpToLabel;
	};

	struct Workflow
	{
		std::string label;
		std::vector<Rule> rules;
	};

	// Inclusive ranges of valid variables
	struct PartRanges
	{
		Vector2d<int64_t> x{ 1, 4000 };
		Vector2d<int64_t> m{ 1, 4000 };
		Vector2d<int64_t> a{ 1, 4000 };
		Vector2d<int64_t> s{ 1, 4000 };
	};

	auto ReadInput(const std::filesystem::path& inputFile)
	{
		std::vector<std::string> lines = ReadAllLinesInFile(inputFile);

		// Read workflows
		int i = 0;
		std::unordered_map<std::string, Workflow> workflows;
		while (i < lines.size() && !lines[i].empty())
		{
			const std::string& line = lines[i];

			// Extract workflow label
			Workflow workflow;
			workflow.label = line.substr(0, line.find('{'));

			// Extract rules
			int rulesStart = static_cast<int>(workflow.label.size()) + 1;
			int rulesEnd = static_cast<int>(line.size()) - 1;
			std::vector<std::string> ruleTokens = SplitString(line.substr(rulesStart, rulesEnd - rulesStart), ",");
			for (int j = 0; j < ruleTokens.size() - 1; ++j)
			{
				const std::string& ruleToken = ruleTokens[j];
				auto [_, variable, op, operand, jumpToLabel] = scn::scan_tuple<char, char, int, std::string>(ruleToken, "{}{}{}:{}");
				workflow.rules.emplace_back(variable, op, operand, jumpToLabel);
			}

			// Final rule is always a jump to the next workflow
			workflow.rules.emplace_back(0, 0, 0, ruleTokens.back());

			workflows[workflow.label] = workflow;
			++i;
		}
		return workflows;
	}

	int64_t ComputeAcceptedParts(const std::unordered_map<std::string, Workflow>& workflows, const std::string& label, PartRanges part)
	{
		// Rejected parts go in the garbage
		if (label == "R")
		{
			return 0;
		}

		// Compute the total number of parts from the range
		if (label == "A")
		{
			int64_t totalX = part.x.y - part.x.x + 1;
			int64_t totalM = part.m.y - part.m.x + 1;
			int64_t totalA = part.a.y - part.a.x + 1;
			int64_t totalS = part.s.y - part.s.x + 1;
			return totalX * totalM * totalA * totalS;
		}

		const Workflow& workflow = workflows.at(label);
		int64_t acceptedParts = 0;
		for (const Rule& rule : workflow.rules)
		{
			// Split range into two subranges if rule is a < or > operation
			PartRanges newPart = part;
			switch (rule.op)
			{
				case '0':
					break;
				case '<':
					switch (rule.variable)
					{
						case 'x':
							newPart.x.y = std::min(newPart.x.y, rule.operand - 1);
							part.x.x = std::max(part.x.x, rule.operand);
							break;
						case 'm':
							newPart.m.y = std::min(newPart.m.y, rule.operand - 1);
							part.m.x = std::max(part.m.x, rule.operand);
							break;
						case 'a':
							newPart.a.y = std::min(newPart.a.y, rule.operand - 1);
							part.a.x = std::max(part.a.x, rule.operand);
							break;
						case 's':
							newPart.s.y = std::min(newPart.s.y, rule.operand - 1);
							part.s.x = std::max(part.s.x, rule.operand);
							break;
					}
					break;
				case '>':
					switch (rule.variable)
					{
						case 'x':
							newPart.x.x = std::max(newPart.x.x, rule.operand + 1);
							part.x.y = std::min(part.x.y, rule.operand);
							break;
						case 'm':
							newPart.m.x = std::max(newPart.m.x, rule.operand + 1);
							part.m.y = std::min(part.m.y, rule.operand);
							break;
						case 'a':
							newPart.a.x = std::max(newPart.a.x, rule.operand + 1);
							part.a.y = std::min(part.a.y, rule.operand);
							break;
						case 's':
							newPart.s.x = std::max(newPart.s.x, rule.operand + 1);
							part.s.y = std::min(part.s.y, rule.operand);
							break;
					}
					break;
			}

			// Send new subrange to the next workflow; continue reducing original range if additional rules remain
			acceptedParts += ComputeAcceptedParts(workflows, rule.jumpToLabel, newPart);
		}

		return acceptedParts;
	}

	void PrintSolution(const std::filesystem::path& inputFile, bool shouldRender)
	{
		std::unordered_map<std::string, Workflow> workflows = ReadInput(inputFile);
		std::cout << ComputeAcceptedParts(workflows, "in", PartRanges{});
	}
} // namespace Puzzle19B