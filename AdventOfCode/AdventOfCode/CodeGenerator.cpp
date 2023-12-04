#include "CodeGenerator.h"

#include "pch.h"

#include "../Utilities/Utilities.h"

namespace CodeGenerator
{
	void GeneratePuzzleSolversHeader()
	{
		auto lines = std::vector<std::string>{ "#pragma once" };

		for (auto i = 1; i <= 25; ++i)
		{
			for (auto j = 0; j < 2; ++j)
			{
				const auto suffix = j == 0 ? 'A' : 'B';

				lines.emplace_back("");

				auto builder = std::stringstream{};
				builder << "namespace Puzzle" << std::setfill('0') << std::setw(2) << i << suffix;
				lines.emplace_back(builder.str());
				lines.emplace_back("{");
				lines.emplace_back("\tvoid PrintSolution(const std::filesystem::path& inputFile, bool shouldRender);");
				lines.emplace_back("}");
			}
		}

		Utilities::WriteAllLinesToFile("PuzzleSolvers.h", lines);
	}

	void GeneratePuzzleSolvers()
	{
		for (auto i = 1; i <= 25; ++i)
		{
			for (auto j = 0; j < 2; ++j)
			{
				auto builder = std::stringstream{};
				builder << "Puzzle" << std::setfill('0') << std::setw(2) << i;
				const auto puzzleName = builder.str();
				builder << (j == 0 ? 'A' : 'B');
				const auto puzzleNameWithSuffix = builder.str();

				auto lines = std::vector<std::string>{
					R"(#include "pch.h")",        "", R"(#include "../Utilities/Utilities.h")", R"(#include "PuzzleSolvers.h")", "",
					"using namespace Utilities;", ""
				};

				builder.str("");
				builder.clear();
				builder << "namespace " << puzzleNameWithSuffix;
				auto namespaceName = builder.str();
				lines.emplace_back(namespaceName);

				lines.emplace_back("{");
				lines.emplace_back("\tauto ReadInput(const std::filesystem::path& inputFile)");
				lines.emplace_back("\t{");

				builder.str("");
				builder.clear();
				builder << "\t\t"
						<< R"(auto input = ReadAllLinesInFile(inputFile);)";
				lines.emplace_back(builder.str());
				lines.emplace_back("");
				lines.emplace_back("\t\treturn input;");
				lines.emplace_back("\t}");
				lines.emplace_back("");

				lines.emplace_back("\tvoid PrintSolution(const std::filesystem::path& inputFile, bool shouldRender)");
				lines.emplace_back("\t{");
				lines.emplace_back("\t\tauto input = ReadInput(inputFile);");

				builder.str("");
				builder.clear();
				builder << "\n\t\t"
						<< R"(std::cout << ")" << puzzleNameWithSuffix << R"( not yet solved!";)";
				lines.emplace_back(builder.str());

				lines.emplace_back("\t}");
				builder.str("");
				builder.clear();
				builder << "} // " << namespaceName;
				lines.emplace_back(builder.str());

				builder.str("");
				builder.clear();
				builder << puzzleNameWithSuffix << ".cpp";
				Utilities::WriteAllLinesToFile(builder.str(), lines);
			}
		}
	}

	void GeneratePuzzleInputFiles()
	{
		auto lines = std::vector<std::string>{ "" };
		for (auto i = 1; i <= 25; ++i)
		{
			{
				auto builder = std::stringstream{};
				builder << "Puzzle" << std::setfill('0') << std::setw(2) << i << ".input";
				Utilities::WriteAllLinesToFile(builder.str(), lines);
			}

			{
				auto builder = std::stringstream{};
				builder << "Puzzle" << std::setfill('0') << std::setw(2) << i << "SampleA.input";
				Utilities::WriteAllLinesToFile(builder.str(), lines);
			}
		}
	}
} // namespace CodeGenerator