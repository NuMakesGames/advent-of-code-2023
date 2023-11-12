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

	// Valid colors for console text
	enum class ConsoleTextColor
	{
		Blue = 1,
		Green = 2,
		Cyan = 3,
		Red = 4,
		Magenta = 5,
		Yellow = 6,
		White = 7,
		Gray = 8,
		BrightBlue = 9,
		BrightGreen = 10,
		BrightCyan = 11,
		BrightRed = 12,
		BrightMagenta = 13,
		BrightYellow = 14,
		BrightWhite = 15
	};
	ConsoleTextColor& operator++(ConsoleTextColor& c);

	void SetConsoleTextColor(ConsoleTextColor color);

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
		ScopedConsoleTextColor(ConsoleTextColor color);
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
		ConsoleTextColor color = ConsoleTextColor::Gray;

		auto operator<=>(const ConsoleSprite&) const = default;
	};

	// Simple rendering interface for updating the console in-place
	class ConsoleRenderer
	{
	public:
		ConsoleRenderer();
		ConsoleRenderer(int width, int height);

		// Clears the back buffer
		void Clear();

		// Draws text starting at the specified position
		void DrawString(const Vector2d<int>& position, const std::string& text, ConsoleTextColor color)
		{
			DrawString(position.x, position.y, text, color);
		}

		// Draws text starting at the specified position
		void DrawString(int x, int y, const std::string& text, ConsoleTextColor color);

		// Draws grid starting at (0, 0)
		void DrawGrid(const Grid2d<char>& grid, ConsoleTextColor color)
		{
			DrawGrid(0, 0, grid, color);
		}

		// Draws grid starting at the specified position
		void DrawGrid(const Vector2d<int>& position, const Grid2d<char>& grid, ConsoleTextColor color)
		{
			DrawGrid(position.x, position.y, grid, color);
		}

		// Draws grid starting at the specified position
		void DrawGrid(int x, int y, const Grid2d<char>& grid, ConsoleTextColor color);

		// Draws the sprite to the specified location
		void Draw(const Vector2d<int>& position, const ConsoleSprite& sprite);

		// Draws the sprite to the specified location
		void Draw(const Vector2d<int>& position, char c, ConsoleTextColor color)
		{
			Draw(position, ConsoleSprite{ c, color });
		}

		// Draws the sprite to the specified location
		void Draw(int x, int y, const ConsoleSprite& sprite);

		// Draws the sprite to the specified location
		void Draw(int x, int y, char c, ConsoleTextColor color)
		{
			Draw(x, y, ConsoleSprite{ c, color });
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
		void Draw(R&& range, char c, ConsoleTextColor color)
		{
			for (const auto& vec : range)
			{
				Draw(vec, ConsoleSprite{ c, color });
			}
		}

		// Copies the back buffer to the front buffer and renders its contents
		void Present();

	private:
		// Attempts to resize/scroll the console as necessary
		void MakeViewportVisible();

		Vector2d<int> m_cursorPosition;
		int m_width = 0;
		int m_height = 0;

		Grid2d<ConsoleSprite> m_frontBuffer;
		Grid2d<ConsoleSprite> m_backBuffer;
	};
} // namespace Utilities