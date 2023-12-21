#include "pch.h"

#include "../Utilities/Utilities.h"
#include "PuzzleSolvers.h"

using namespace Utilities;

namespace Puzzle20A
{
	enum class ModuleType
	{
		Broadcaster,
		FlipFlop,
		Conjunction,
		Terminus
	};

	struct Module
	{
		// Name of the module
		std::string label;

		// Type of the module
		ModuleType type;

		// Modules that send pulses to this module
		std::vector<std::string> sourceModules;

		// Modules that this module sends pulses to
		std::vector<std::string> destinationModules;

		// Only used by flip-flops
		bool isOn = false;

		// Only used by conjunctions
		std::unordered_map<std::string, int> lastPulse;
	};

	struct PulseRecord
	{
		// Module that sent the pulse
		std::string source;

		// Module that received the pulse
		std::string destination;

		// High or low (1 or 0)
		int pulse = 0;
	};

	std::unordered_map<std::string, Module> ReadInput(const std::filesystem::path& inputFile)
	{
		// Read all modules from the input file
		std::unordered_map<std::string, Module> modules;
		for (const auto& line : ReadAllLinesInFile(inputFile))
		{
			Module module;

			auto tokens = SplitString(line, " -> ");
			module.destinationModules = SplitString(tokens[1], ", ");

			if (tokens[0].starts_with("broadcaster"))
			{
				module.label = "broadcaster";
				module.type = ModuleType::Broadcaster;
			}
			else if (tokens[0][0] == '%')
			{
				module.label = tokens[0].substr(1);
				module.type = ModuleType::FlipFlop;
			}
			else
			{
				module.label = tokens[0].substr(1);
				module.type = ModuleType::Conjunction;
			}

			modules[module.label] = module;
		}

		// Create a terminus module for any module that doesn't have any destination modules
		for (const auto& [label, module] : modules)
		{
			for (const std::string& destination : module.destinationModules)
			{
				if (!modules.contains(destination))
				{
					modules[destination] = Module{ destination, ModuleType::Terminus };
				}
			}
		}

		// Keep track of source modules for each module
		for (auto& [destinationLabel, destination] : modules)
		{
			for (const auto& [sourceLabel, source] : modules)
			{
				if (std::ranges::find(source.destinationModules, destination.label) != source.destinationModules.end())
				{
					destination.sourceModules.push_back(sourceLabel);

					// If we're a conjunction, initialize a memory slot for a low pulse for each source module
					if (destination.type == ModuleType::Conjunction)
					{
						destination.lastPulse[sourceLabel] = 0;
					}
				}
			}
		}

		return modules;
	}

	void PrintSolution(const std::filesystem::path& inputFile, bool shouldRender)
	{
		std::unordered_map<std::string, Module> modules = ReadInput(inputFile);
		std::vector<PulseRecord> allPulses;

		constexpr int buttonPushes = 1000;
		for (int i = 0; i < buttonPushes; ++i)
		{
			// Push the button...
			std::queue<PulseRecord> queue;
			queue.push({ "button", "broadcaster", 0 });

			// ... and process all downstream pulses
			while (!queue.empty())
			{
				auto [source, destination, pulse] = queue.front();
				queue.pop();

				// Record the pulse
				allPulses.emplace_back(source, destination, pulse);

				// Generate downstream pulses based on the module type
				Module& current = modules[destination];
				switch (current.type)
				{
					case ModuleType::Broadcaster:
						for (const std::string& next : current.destinationModules)
						{
							queue.push({ current.label, next, pulse });
						}
						break;
					case ModuleType::FlipFlop:
						if (pulse == 0)
						{
							current.isOn = !current.isOn;
							for (const std::string& next : current.destinationModules)
							{
								queue.push({ current.label, next, current.isOn ? 1 : 0 });
							}
						}
						break;
					case ModuleType::Conjunction:
						current.lastPulse[source] = pulse;
						bool allHigh = std::ranges::all_of(current.lastPulse | std::views::values, [](int last) { return last == 1; });
						for (const auto& next : current.destinationModules)
						{
							queue.push({ current.label, next, allHigh ? 0 : 1 });
						}
						break;
				}
			}
		}

		int64_t highPulses = std::ranges::count_if(allPulses, [](const PulseRecord& pulse) { return pulse.pulse == 1; });
		int64_t lowPulses = allPulses.size() - highPulses;
		std::cout << lowPulses * highPulses;
	}
} // namespace Puzzle20A