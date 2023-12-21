#include "pch.h"

#include "../Utilities/Utilities.h"
#include "PuzzleSolvers.h"

using namespace Utilities;

namespace Puzzle20B
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

	struct PeriodKey
	{
		// Module that received the pulse
		std::string label;

		// High or low (1 or 0)
		int pulse = 0;

		auto operator<=>(const PeriodKey&) const = default;
	};
} // namespace Puzzle20B

IMPLEMENT_STD_HASH(Puzzle20B::PeriodKey, value.label, value.pulse);

namespace Puzzle20B
{
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

	std::vector<PulseRecord> PushButton(std::unordered_map<std::string, Module>& modules)
	{
		std::vector<PulseRecord> pulses;

		// Push the button...
		std::queue<PulseRecord> queue;
		queue.push({ "button", "broadcaster", 0 });

		// ... and process all downstream pulses
		while (!queue.empty())
		{
			auto [source, destination, pulse] = queue.front();
			queue.pop();

			// Record the pulse
			pulses.emplace_back(source, destination, pulse);

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

		return pulses;
	}

	void PrintSolution(const std::filesystem::path& inputFile, bool shouldRender)
	{
		std::unordered_map<std::string, Module> modules = ReadInput(inputFile);

		std::string targetModuleLabel = "rx";
		constexpr int targetPulse = 1;
		const Module& targetModule = modules.at(targetModuleLabel);

		// rx is fed by a single conjunction module
		VerifyElseCrash(targetModule.sourceModules.size() == 1);
		std::string sourceModuleLabel = targetModule.sourceModules[0];
		const Module& sourceModule = modules[sourceModuleLabel];
		VerifyElseCrash(sourceModule.type == ModuleType::Conjunction);

		// The conjunction module feeding rx is fed a series of other conjunction module
		std::vector<std::string> inputModuleLabels = sourceModule.sourceModules;
		for (const std::string& inputModuleLabel : inputModuleLabels)
		{
			VerifyElseCrash(modules[inputModuleLabel].type == ModuleType::Conjunction);
		}

		// Find the periods of those other conjunction modules
		std::unordered_map<std::string, std::vector<int64_t>> inputModulePulses;
		std::unordered_map<std::string, int64_t> inputModulePeriods;
		for (const std::string& inputModuleLabel : inputModuleLabels)
		{
			inputModulePeriods[inputModuleLabel] = 0;
		}

		// Repeatedly press the button until we've found all necessary periods
		int i = 0;
		while (std::ranges::any_of(inputModulePeriods | std::views::values, [](int64_t period) { return period == 0; }))
		{
			++i;
			std::vector<PulseRecord> pulses = PushButton(modules);
			for (const PulseRecord& pulseRecord : pulses)
			{
				if (pulseRecord.pulse == 1 && pulseRecord.destination == sourceModuleLabel && inputModulePeriods[pulseRecord.source] == 0)
				{
					std::vector<int64_t>& modulePulses = inputModulePulses[pulseRecord.source];
					if (modulePulses.empty() || modulePulses.back() != i)
					{
						modulePulses.push_back(i);
					}

					if (modulePulses.size() == 3)
					{
						inputModulePeriods[pulseRecord.source] = modulePulses[2] - modulePulses[1];
					}
				}
			}
		}

		// Compute the lcm where the conjunction module feeding rx will fire a high pulse
		int64_t lcm = 1;
		for (int64_t period : inputModulePeriods | std::views::values)
		{
			lcm = std::lcm(lcm, period);
		}

		std::cout << lcm;
	}
} // namespace Puzzle20B