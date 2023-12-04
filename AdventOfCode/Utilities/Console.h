#pragma once

#include "Grid2d.h"
#include "Vector2d.h"

namespace Utilities
{
	// Returns the current position of the console cursor
	Vector2d<int> GetConsoleCursorPosition();

	// Sets the current position of the console cursor
	void SetConsoleCursorPosition(const Vector2d<int>& position);

	// Control whether the cursor should be visible
	void SetConsoleCursorVisible(bool isCursorVisible);

	// Colors composed from FOREGROUND_BLUE, FOREGROUND_GREEN, FOREGROUND_RED, and FOREGROUND_INTENSITY
	enum class ConsoleForegroundColor
	{
		Black = 0,
		Blue = 1,
		Green = 2,
		Cyan = 3,
		Red = 4,
		Magenta = 5,
		Yellow = 6,
		White = 7,
		IntenseBlack = 8,
		IntenseBlue = 9,
		IntenseGreen = 10,
		IntenseCyan = 11,
		IntenseRed = 12,
		IntenseMagenta = 13,
		IntenseYellow = 14,
		IntenseWhite = 15
	};
	ConsoleForegroundColor& operator++(ConsoleForegroundColor& c);

	// Colors composed from BACKGROUND_BLUE, BACKGROUND_GREEN, BACKGROUND_RED, and BACKGROUND_INTENSITY
	enum class ConsoleBackgroundColor
	{
		Black = 0,
		Blue = 0x10,
		Green = 0x20,
		Cyan = Blue | Green,
		Red = 0x40,
		Magenta = Red | Blue,
		Yellow = Red | Green,
		White = Red | Green | Blue,
		IntenseBlack = 0x80,
		IntenseBlue = IntenseBlack | Blue,
		IntenseGreen = IntenseBlack | Green,
		IntenseCyan = IntenseBlack | Cyan,
		IntenseRed = IntenseBlack | Red,
		IntenseMagenta = IntenseBlack | Magenta,
		IntenseYellow = IntenseBlack | Yellow,
		IntenseWhite = IntenseBlack | White
	};

	// Changes the color of text written to the console
	void SetConsoleTextColor(ConsoleForegroundColor foreground, ConsoleBackgroundColor background = ConsoleBackgroundColor::Black);

	// RAII wrapper to disable the console cursor
	class ScopedConsoleCursorDisable
	{
	public:
		ScopedConsoleCursorDisable();
		~ScopedConsoleCursorDisable();

		ScopedConsoleCursorDisable(const ScopedConsoleCursorDisable&) = delete;
		ScopedConsoleCursorDisable& operator=(const ScopedConsoleCursorDisable&) = delete;
		ScopedConsoleCursorDisable(ScopedConsoleCursorDisable&&) = delete;
		ScopedConsoleCursorDisable& operator=(ScopedConsoleCursorDisable&&) = delete;

	private:
		bool m_visible = true;
	};

	// RAII wrapper to change the console's color
	class ScopedConsoleTextColor
	{
	public:
		ScopedConsoleTextColor(ConsoleForegroundColor foreground, ConsoleBackgroundColor background = ConsoleBackgroundColor::Black);
		~ScopedConsoleTextColor();

		ScopedConsoleTextColor(const ScopedConsoleTextColor&) = delete;
		ScopedConsoleTextColor& operator=(const ScopedConsoleTextColor&) = delete;
		ScopedConsoleTextColor(ScopedConsoleTextColor&&) = delete;
		ScopedConsoleTextColor& operator=(ScopedConsoleTextColor&&) = delete;

	private:
		int m_attributes = 0;
	};

	// Holds a character and color to be rendered by ConsoleRenderer
	struct ConsoleSprite
	{
		char character = ' ';
		ConsoleForegroundColor foregroundColor = ConsoleForegroundColor::White;
		ConsoleBackgroundColor backgroundColor = ConsoleBackgroundColor::Black;

		auto operator<=>(const ConsoleSprite&) const = default;
	};

	// Simple rendering interface for updating the console in-place
	class ConsoleRenderer
	{
	public:
		ConsoleRenderer();
		ConsoleRenderer(int width, int height, int spriteWidth = 2);

		// Configures the renderer to wait the provided number of ms after a call to Present
		void SetTimeToWaitAfterPresent(std::chrono::milliseconds waitAfterPresent);

		// Clears the back buffer
		void Clear();

		// Draws text starting at the specified position
		void DrawString(
			const Vector2d<int>& position,
			const std::string& text,
			ConsoleForegroundColor foreground = ConsoleForegroundColor::White,
			ConsoleBackgroundColor background = ConsoleBackgroundColor::Black)
		{
			DrawString(position.x, position.y, text, foreground, background);
		}

		// Draws text starting at the specified position
		void DrawString(
			int x,
			int y,
			const std::string& text,
			ConsoleForegroundColor foreground = ConsoleForegroundColor::White,
			ConsoleBackgroundColor background = ConsoleBackgroundColor::Black);

		// Draws grid starting at (0, 0)
		void DrawGrid(
			const Grid2d<char>& grid,
			ConsoleForegroundColor foreground = ConsoleForegroundColor::White,
			ConsoleBackgroundColor background = ConsoleBackgroundColor::Black)
		{
			DrawGrid(0, 0, grid, foreground, background);
		}

		// Draws grid starting at the specified position
		void DrawGrid(
			const Vector2d<int>& position,
			const Grid2d<char>& grid,
			ConsoleForegroundColor foreground = ConsoleForegroundColor::White,
			ConsoleBackgroundColor background = ConsoleBackgroundColor::Black)
		{
			DrawGrid(position.x, position.y, grid, foreground, background);
		}

		// Draws grid starting at the specified position
		void DrawGrid(
			int x,
			int y,
			const Grid2d<char>& grid,
			ConsoleForegroundColor foreground = ConsoleForegroundColor::White,
			ConsoleBackgroundColor background = ConsoleBackgroundColor::Black);

		// Draws the sprite to the specified location
		void Draw(const Vector2d<int>& position, const ConsoleSprite& sprite);

		// Draws the sprite to the specified location
		void Draw(
			const Vector2d<int>& position,
			char c,
			ConsoleForegroundColor foreground = ConsoleForegroundColor::White,
			ConsoleBackgroundColor background = ConsoleBackgroundColor::Black)
		{
			Draw(position, ConsoleSprite{ c, foreground, background });
		}

		// Draws the sprite to the specified location
		void Draw(int x, int y, const ConsoleSprite& sprite);

		// Draws the sprite to the specified location
		void Draw(
			int x,
			int y,
			char c,
			ConsoleForegroundColor foreground = ConsoleForegroundColor::White,
			ConsoleBackgroundColor background = ConsoleBackgroundColor::Black)
		{
			Draw(x, y, ConsoleSprite{ c, foreground, background });
		}

		// Draws the sprite to all specified locations in the range
		template<std::ranges::input_range R>
		void Draw(R&& range, const ConsoleSprite& sprite)
		{
			for (const auto& vec : range)
			{
				Draw(vec, sprite);
			}
		}

		// Draws the sprite to all specified locations in the range
		template<std::ranges::input_range R>
		void Draw(
			R&& range,
			char c,
			ConsoleForegroundColor foreground = ConsoleForegroundColor::White,
			ConsoleBackgroundColor background = ConsoleBackgroundColor::Black)
		{
			for (const auto& vec : range)
			{
				Draw(vec, ConsoleSprite{ c, foreground, background });
			}
		}

		// Copies the back buffer to the front buffer and renders its contents
		void Present();

		// Manually sets the starting cursor position for the renderer
		void SetCursorPosition(const Vector2d<int>& cursorPosition);

		// Sets whether the cursor should be moved under the render rect after present.
		// Set to true if program writes to console while renderer is running.
		// Set to false to support larger views without the console scrolling.
		// If disabled, call MoveCursorBelowRenderer manually to place the cursor below the render rect.
		void SetRestoreCursorAfterPresent(bool shouldRestoreCursorAfterPresent);

		// Manually moves the cursor beneath the render rect. Intended for when SetRestoreCursorAfterPresent is used to
		// disable automatically restoring the cursor position.
		void MoveCursorBelowRenderer();

	private:
		// Attempts to resize/scroll the console as necessary
		void MakeViewportVisible();

		ScopedConsoleCursorDisable m_disableCursor;
		Vector2d<int> m_cursorPosition;
		int m_width = 0;
		int m_height = 0;
		int m_spriteWidth = 2;
		bool m_shouldRestoreCursorAfterPresent = true;
		std::chrono::milliseconds m_waitAfterPresent = std::chrono::milliseconds(0);

		Grid2d<ConsoleSprite> m_frontBuffer;
		Grid2d<ConsoleSprite> m_backBuffer;
	};
} // namespace Utilities