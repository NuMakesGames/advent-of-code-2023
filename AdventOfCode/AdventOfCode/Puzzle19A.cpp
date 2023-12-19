#include "pch.h"

#include "../Utilities/Utilities.h"
#include "PuzzleSolvers.h"

using namespace Utilities;

namespace Puzzle19A
{
	struct Rule
	{
		char variable = 0;
		char op = 0;
		int operand = 0;
		std::string jumpToLabel;
	};

	struct Workflow
	{
		std::string label;
		std::vector<Rule> rules;
	};

	struct Part
	{
		int x = 0;
		int m = 0;
		int a = 0;
		int s = 0;
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

		// Skip empty line
		++i;

		// Read parts
		std::vector<Part> parts;
		while (i < lines.size())
		{
			auto [_, x, m, a, s] = scn::scan_tuple<int, int, int, int>(lines[i], "{{x={},m={},a={},s={}}}");
			parts.emplace_back(x, m, a, s);
			++i;
		}

		return std::make_pair(workflows, parts);
	}

	void PrintSolution(const std::filesystem::path& inputFile, bool shouldRender)
	{
		auto [workflows, parts] = ReadInput(inputFile);

		std::vector<Part> accepted;
		for (const Part& part : parts)
		{
			Workflow currentWorkflow = workflows["in"];
			bool isPartProcessed = false;
			while (!isPartProcessed)
			{
				for (const Rule& rule : currentWorkflow.rules)
				{
					bool shouldJump = false;
					switch (rule.op)
					{
						case 0:
							shouldJump = true;
							break;
						default:
							switch (rule.variable)
							{
								case 'x':
									shouldJump = rule.op == '<' ? part.x < rule.operand : part.x > rule.operand;
									break;
								case 'm':
									shouldJump = rule.op == '<' ? part.m < rule.operand : part.m > rule.operand;
									break;
								case 'a':
									shouldJump = rule.op == '<' ? part.a < rule.operand : part.a > rule.operand;
									break;
								case 's':
									shouldJump = rule.op == '<' ? part.s < rule.operand : part.s > rule.operand;
									break;
							}
							break;
					}

					if (shouldJump)
					{
						if (rule.jumpToLabel == "R")
						{
							isPartProcessed = true;
						}
						else if (rule.jumpToLabel == "A")
						{
							accepted.emplace_back(part);
							isPartProcessed = true;
						}
						else
						{
							currentWorkflow = workflows[rule.jumpToLabel];
						}
						break;
					}
				}
			}
		}

		std::cout << ranges::accumulate(
			accepted, 0ll, [](int64_t acc, const Part& part) { return acc + part.x + part.m + part.a + part.s; });
	}
} // namespace Puzzle19A