#include "pch.h"

#include "Console.h"
#include "Vector2d.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace Utilities
{
	const auto hOut = GetStdHandle(STD_OUTPUT_HANDLE);

	Vector2d<int> GetConsoleCursorPosition()
	{
		auto info = CONSOLE_SCREEN_BUFFER_INFO{};
		return ::GetConsoleScreenBufferInfo(hOut, &info) ? Vector2d<int>{ info.dwCursorPosition.X, info.dwCursorPosition.Y }
		                                                 : Vector2d<int>{};
	}

	void SetConsoleCursorPosition(const Vector2d<int>& position)
	{
		::SetConsoleCursorPosition(hOut, COORD{ static_cast<SHORT>(position.x), static_cast<SHORT>(position.y) });
	}

	void SetConsoleCursorVisible(bool isCursorVisible)
	{
		auto cursorInfo = CONSOLE_CURSOR_INFO{};
		::GetConsoleCursorInfo(hOut, &cursorInfo);
		if (cursorInfo.bVisible == (isCursorVisible ? 1 : 0))
		{
			return;
		}

		cursorInfo.bVisible = isCursorVisible;
		::SetConsoleCursorInfo(hOut, &cursorInfo);
	}

	ConsoleTextColor& operator++(ConsoleTextColor& c)
	{
		int underlying = static_cast<int>(c);
		++underlying;

		if (underlying > static_cast<int>(ConsoleTextColor::BrightWhite))
		{
			underlying = static_cast<int>(ConsoleTextColor::Blue);
		}

		c = static_cast<ConsoleTextColor>(underlying);
		return c;
	}

	void SetConsoleTextColor(ConsoleTextColor color)
	{
		auto info = CONSOLE_SCREEN_BUFFER_INFO{};
		if (::GetConsoleScreenBufferInfo(hOut, &info))
		{
			auto attributesMinusColor = info.wAttributes & 0xF0;
			::SetConsoleTextAttribute(hOut, attributesMinusColor | static_cast<WORD>(color));
		}
	}

	ScopedConsoleCursorDisable::ScopedConsoleCursorDisable()
	{
		auto cursorInfo = CONSOLE_CURSOR_INFO{};
		::GetConsoleCursorInfo(hOut, &cursorInfo);
		m_visible = cursorInfo.bVisible;
		cursorInfo.bVisible = false;
		::SetConsoleCursorInfo(hOut, &cursorInfo);
	}

	ScopedConsoleCursorDisable::~ScopedConsoleCursorDisable()
	{
		auto cursorInfo = CONSOLE_CURSOR_INFO{};
		::GetConsoleCursorInfo(hOut, &cursorInfo);
		cursorInfo.bVisible = m_visible;
		::SetConsoleCursorInfo(hOut, &cursorInfo);
	}

	ScopedConsoleTextColor::ScopedConsoleTextColor(ConsoleTextColor color)
	{
		auto info = CONSOLE_SCREEN_BUFFER_INFO{};
		if (::GetConsoleScreenBufferInfo(hOut, &info))
		{
			m_attributes = info.wAttributes;
			auto attributesMinusColor = m_attributes & 0xF0;
			::SetConsoleTextAttribute(hOut, attributesMinusColor | static_cast<WORD>(color));
		}
	}

	ScopedConsoleTextColor::~ScopedConsoleTextColor()
	{
		::SetConsoleTextAttribute(hOut, m_attributes);
	}

	ConsoleRenderer::ConsoleRenderer() : ConsoleRenderer(50, 50)
	{
	}

	ConsoleRenderer::ConsoleRenderer(int width, int height)
		: m_width(width)
		, m_height(height)
		, m_frontBuffer(m_width, m_height)
		, m_backBuffer(m_width, m_height)
		, m_cursorPosition(GetConsoleCursorPosition())
	{
		MakeViewportVisible();
	}

	void ConsoleRenderer::Clear()
	{
		std::ranges::fill(m_backBuffer, ConsoleSprite{});
	}

	void ConsoleRenderer::DrawString(int x, int y, const std::string& text, ConsoleTextColor color)
	{
		for (int i = 0; i < text.size(); ++i)
		{
			m_backBuffer.at(x + i, y) = ConsoleSprite{ text[i], color };
		}
	}


	void ConsoleRenderer::DrawGrid(int x, int y, const Grid2d<char>& grid, ConsoleTextColor color)
	{
		for (int j = 0; j < grid.Height(); ++j)
		{
			for (int i = 0; i < grid.Width(); ++i)
			{
				m_backBuffer.at(x + i, y + j) = ConsoleSprite{ grid.at(i, j), color };
			}
		}
	}

	void ConsoleRenderer::Draw(const Vector2d<int>& position, const ConsoleSprite& sprite)
	{
		m_backBuffer.at(position) = sprite;
	}

	void ConsoleRenderer::Draw(int x, int y, const ConsoleSprite& sprite)
	{
		m_backBuffer.at(x, y) = sprite;
	}

	void ConsoleRenderer::Present()
	{
		// Disable the cursor during rendering
		ScopedConsoleCursorDisable disableCursor;

		// Restore attributes at the end of rendering
		ScopedConsoleTextColor textColor{ ConsoleTextColor::Gray };

		// Swap our buffers
		m_frontBuffer.swap(m_backBuffer);

		for (int y = 0; y < m_frontBuffer.Height(); ++y)
		{
			for (int x = 0; x < m_frontBuffer.Width(); ++x)
			{
				// Check if the pixel changed
				const ConsoleSprite& sprite = m_frontBuffer.at(x, y);
				if (sprite == m_backBuffer.at(x, y))
				{
					continue;
				}

				// If different, render it
				SetConsoleCursorPosition(Vector2d<int>{ m_cursorPosition.x + x * 2, m_cursorPosition.y + y });
				SetConsoleTextColor(sprite.color);
				std::cout << sprite.character;
			}
		}

		// Copy the contents of the front buffer into the back buffer
		m_backBuffer = m_frontBuffer;

		// Move cursor below render rect
		SetConsoleCursorPosition(Vector2d<int>{ m_cursorPosition.x, m_cursorPosition.y + m_height });

		// Pause momentarily so updates are visible
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}

	void ConsoleRenderer::MakeViewportVisible()
	{
		CONSOLE_SCREEN_BUFFER_INFOEX csbi;
		csbi.cbSize = sizeof(csbi);
		if (!::GetConsoleScreenBufferInfoEx(hOut, &csbi))
		{
			return;
		}

		bool shouldUpdateScreenBuffer = false;

		// Make sure the buffer is large enough
		if (csbi.dwSize.X < m_width * 2 || csbi.dwSize.Y < m_height)
		{
			// Add a little vertical slack when resizing to avoid any unnecessary scrolling
			constexpr int slack = 10;
			csbi.dwSize.X = max(csbi.dwSize.X, static_cast<SHORT>(m_width) * 2);
			csbi.dwSize.Y = max(csbi.dwSize.Y, static_cast<SHORT>(m_height) + slack);
			shouldUpdateScreenBuffer = true;
		}

		// Make sure the window rect is large enough
		if (csbi.srWindow.Right - csbi.srWindow.Left < m_width * 2 || csbi.srWindow.Bottom - csbi.srWindow.Top < m_height)
		{
			// Add a little vertical slack when resizing to avoid any unnecessary scrolling
			constexpr int slack = 10;
			csbi.srWindow.Right = max(csbi.srWindow.Right, m_width * 2);
			csbi.srWindow.Bottom = max(csbi.srWindow.Bottom, m_height + slack);
			shouldUpdateScreenBuffer = true;
		}

		// Make sure the viewport is on screen horizontally
		if (csbi.srWindow.Right < m_cursorPosition.x + m_width * 2)
		{
			SHORT horizontal = csbi.srWindow.Right - csbi.srWindow.Left;
			csbi.srWindow.Right = m_cursorPosition.x + m_width * 2;
			csbi.srWindow.Left = csbi.srWindow.Right - horizontal;
			shouldUpdateScreenBuffer = true;
		}

		// Make sure the viewbort is on screen vertically
		if (csbi.srWindow.Bottom < m_cursorPosition.y + m_height)
		{
			SHORT vertical = csbi.srWindow.Bottom - csbi.srWindow.Top;
			csbi.srWindow.Bottom = m_cursorPosition.y + m_height;
			csbi.srWindow.Top = csbi.srWindow.Bottom - vertical;
			shouldUpdateScreenBuffer = true;
		}

		if (shouldUpdateScreenBuffer)
		{
			::SetConsoleScreenBufferInfoEx(hOut, &csbi);
		}
	}
} // namespace Utilities