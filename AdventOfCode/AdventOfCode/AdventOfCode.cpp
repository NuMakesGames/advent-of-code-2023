// AdventOfCode.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"

#include "../Utilities/Utilities.h"
#include "PuzzleSolvers.h"

int main()
{
	constexpr auto partASolver = &Puzzle01A::PrintSolution;
	constexpr auto partBSolver = &Puzzle01B::PrintSolution;

	for (auto i = 0; i < 2; ++i)
	{
		const auto solver = i == 0 ? partASolver : partBSolver;

		auto start = std::chrono::high_resolution_clock::now();
		solver();
		auto stop = std::chrono::high_resolution_clock::now();
		auto durationMs = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
		std::cout << "\nSolver executed in " << durationMs.count() << " ms.\n\n";
	}
}