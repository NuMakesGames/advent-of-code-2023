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

	ConsoleForegroundColor& operator++(ConsoleForegroundColor& c)
	{
		int underlying = static_cast<int>(c);
		++underlying;

		if (underlying > static_cast<int>(ConsoleForegroundColor::IntenseWhite))
		{
			underlying = static_cast<int>(ConsoleForegroundColor::Black);
		}

		c = static_cast<ConsoleForegroundColor>(underlying);
		return c;
	}

	void SetConsoleTextColor(ConsoleForegroundColor foreground, ConsoleBackgroundColor background)
	{
		auto info = CONSOLE_SCREEN_BUFFER_INFO{};
		if (::GetConsoleScreenBufferInfo(hOut, &info))
		{
			auto attributesMinusColor = info.wAttributes & 0xFF00;
			::SetConsoleTextAttribute(hOut, attributesMinusColor | static_cast<WORD>(foreground) | static_cast<WORD>(background));
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

	ScopedConsoleTextColor::ScopedConsoleTextColor(ConsoleForegroundColor foreground, ConsoleBackgroundColor background)
	{
		auto info = CONSOLE_SCREEN_BUFFER_INFO{};
		if (::GetConsoleScreenBufferInfo(hOut, &info))
		{
			m_attributes = info.wAttributes;
			SetConsoleTextColor(foreground, background);
		}
	}

	ScopedConsoleTextColor::~ScopedConsoleTextColor()
	{
		::SetConsoleTextAttribute(hOut, m_attributes);
	}

	ConsoleRenderer::ConsoleRenderer() : ConsoleRenderer(50, 50, 2)
	{
	}

	ConsoleRenderer::ConsoleRenderer(int width, int height, int spriteWidth)
		: m_width(width)
		, m_height(height)
		, m_spriteWidth(spriteWidth)
		, m_frontBuffer(m_width, m_height)
		, m_backBuffer(m_width, m_height)
		, m_cursorPosition(GetConsoleCursorPosition())
	{
		MakeViewportVisible();
	}

	void ConsoleRenderer::SetTimeToWaitAfterPresent(std::chrono::milliseconds waitAfterPresent)
	{
		m_waitAfterPresent = waitAfterPresent;
	}

	void ConsoleRenderer::Clear()
	{
		std::ranges::fill(m_backBuffer, ConsoleSprite{});
	}

	void ConsoleRenderer::DrawString(
		int x,
		int y,
		const std::string& text,
		ConsoleForegroundColor foreground,
		ConsoleBackgroundColor background)
	{
		for (int i = 0; i < text.size(); ++i)
		{
			m_backBuffer.at(x + i, y) = ConsoleSprite{ text[i], foreground, background };
		}
	}

	void ConsoleRenderer::DrawGrid(
		int x,
		int y,
		const Grid2d<char>& grid,
		ConsoleForegroundColor foreground,
		ConsoleBackgroundColor background)
	{
		for (int j = 0; j < grid.Height(); ++j)
		{
			for (int i = 0; i < grid.Width(); ++i)
			{
				m_backBuffer.at(x + i, y + j) = ConsoleSprite{ grid.at(i, j), foreground, background };
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
		// Restore attributes at the end of rendering
		ScopedConsoleTextColor textColor{ ConsoleForegroundColor::White };

		// Capture original console cursor position
		Vector2d<int> originalCursorPosition = GetConsoleCursorPosition();

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
				SetConsoleCursorPosition(Vector2d<int>{ m_cursorPosition.x + x * m_spriteWidth, m_cursorPosition.y + y });
				SetConsoleTextColor(sprite.foregroundColor, sprite.backgroundColor);
				std::cout << sprite.character;
			}
		}

		// Copy the contents of the front buffer into the back buffer
		m_backBuffer = m_frontBuffer;

		// Move cursor below render rect; prefer starting position if deeper
		if (m_shouldRestoreCursorAfterPresent)
		{
			if (originalCursorPosition.y >= m_cursorPosition.y + m_height)
			{
				SetConsoleCursorPosition(originalCursorPosition);
			}
			else
			{
				SetConsoleCursorPosition(Vector2d<int>{ m_cursorPosition.x, m_cursorPosition.y + m_height });
			}
		}

		// Pause momentarily so updates are visible
		if (m_waitAfterPresent > std::chrono::milliseconds(0))
		{
			std::this_thread::sleep_for(m_waitAfterPresent);
		}
	}

	void ConsoleRenderer::SetCursorPosition(const Vector2d<int>& cursorPosition)
	{
		m_cursorPosition = cursorPosition;
	}

	void ConsoleRenderer::SetRestoreCursorAfterPresent(bool shouldRestoreCursorAfterPresent)
	{
		m_shouldRestoreCursorAfterPresent = shouldRestoreCursorAfterPresent;
	}

	void ConsoleRenderer::MoveCursorBelowRenderer()
	{
		SetConsoleCursorPosition(Vector2d<int>{ m_cursorPosition.x, m_cursorPosition.y + m_height });
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
		if (csbi.dwSize.X < m_width * m_spriteWidth || csbi.dwSize.Y < m_height)
		{
			// Add a little vertical slack when resizing to avoid any unnecessary scrolling
			constexpr int slack = 10;
			csbi.dwSize.X = max(csbi.dwSize.X, static_cast<SHORT>(m_width) * m_spriteWidth);
			csbi.dwSize.Y = max(csbi.dwSize.Y, static_cast<SHORT>(m_height) + slack);
			shouldUpdateScreenBuffer = true;
		}

		// Make sure the window rect is large enough
		if (csbi.srWindow.Right - csbi.srWindow.Left < m_width * m_spriteWidth || csbi.srWindow.Bottom - csbi.srWindow.Top < m_height)
		{
			// Add a little vertical slack when resizing to avoid any unnecessary scrolling
			constexpr int slack = 10;
			csbi.srWindow.Right = max(csbi.srWindow.Right, m_width * m_spriteWidth);
			csbi.srWindow.Bottom = max(csbi.srWindow.Bottom, m_height + slack);
			shouldUpdateScreenBuffer = true;
		}

		// Make sure the viewport is on screen horizontally
		if (csbi.srWindow.Right < m_cursorPosition.x + m_width * m_spriteWidth)
		{
			SHORT horizontal = csbi.srWindow.Right - csbi.srWindow.Left;
			csbi.srWindow.Right = m_cursorPosition.x + m_width * m_spriteWidth;
			csbi.srWindow.Left = csbi.srWindow.Right - horizontal;
			shouldUpdateScreenBuffer = true;
		}

		// Make sure the viewport is on screen vertically
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