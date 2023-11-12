#pragma once

namespace CodeGenerator
{
	// Generates PuzzleSolvers.h file.
	void GeneratePuzzleSolversHeader();

	// Generates empty Puzzle01A.cpp - Puzzle25B.cpp files.
	void GeneratePuzzleSolvers();

	// Generates empty Puzzle01.input - Puzzle25.input files.
	void GeneratePuzzleInputFiles();
}