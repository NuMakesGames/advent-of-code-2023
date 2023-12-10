#pragma once

#include "Assertions.h"
#include "Vector2d.h"

namespace Utilities
{
	// Wraps a std::vector and for easier two-dimensional addressing.
	// Grid dimensions cannot be changed after construction.
	template<typename T>
	class Grid2d
	{
	public:
		// Initializes grid to specified width and height. Default initializes.
		Grid2d(int width, int height) : m_width(width), m_height(height), m_data(m_width * m_height)
		{
		}

		Grid2d() : Grid2d(0, 0)
		{
		}

		virtual ~Grid2d()
		{
		}

		// Returns a mutable row-by-row view of the grid.
		auto GetRows()
		{
			return m_data | ranges::views::chunk(m_width);
		}

		// Returns a constant row-by-row view of the grid.
		auto GetRows() const
		{
			return m_data | ranges::views::chunk(m_width);
		}

		// Width of the grid.
		int Width() const
		{
			return m_width;
		}

		// Height of the grid.
		int Height() const
		{
			return m_height;
		}

		// Resizes the grid and attempts to maintain existing elements.
		void resize(int width, int height)
		{
			auto newGrid = Grid2d<T>(width, height);
			for (int y = 0; y < std::min(height, m_height); ++y)
			{
				for (int x = 0; x < std::min(width, m_width); ++x)
				{
					newGrid.at(x, y) = this->at(x, y);
				}
			}
			*this = std::move(newGrid);
		}

		// Returns the size of the underlying vector.
		size_t size() const
		{
			return m_data.size();
		}

		// Converts an index for the underlying vector into (x, y) coordinates.
		virtual Vector2d<int> GetCoordinatesFromIndex(int i) const
		{
			VerifyElseCrash(i < m_data.size());
			return Vector2d<int>(i % m_width, i / m_width);
		}

		// Converts (x, y) coordinates into an index for the underlying vector.
		virtual int GetIndexFromCoordinates(int x, int y) const
		{
			VerifyElseCrash(x < m_width);
			VerifyElseCrash(y < m_height);
			return y * m_width + x;
		}

		// Checks if the provided coordinates are within the bounds of the grid.
		virtual bool IsInBounds(int x, int y) const
		{
			return x >= 0 && x < m_width && y >= 0 && y < m_height;
		}

		// Checks if the provided vector is within the bounds of the grid.
		virtual bool IsInBounds(const Vector2d<int>& position) const
		{
			return IsInBounds(position.x, position.y);
		}

		// Retrieves reference to the value stored at the provided coordinates.
		virtual T& at(int x, int y)
		{
			VerifyElseCrash(x < m_width);
			VerifyElseCrash(y < m_height);
			return m_data.at(y * m_width + x);
		}

		// Retrieves constant reference to the value stored at the provided coordinates.
		virtual const T& at(int x, int y) const
		{
			VerifyElseCrash(x < m_width);
			VerifyElseCrash(y < m_height);
			return m_data.at(y * m_width + x);
		}

		// Retrieves reference to the value stored at the provided vector.
		virtual T& at(const Vector2d<int>& position)
		{
			VerifyElseCrash(position.x < m_width);
			VerifyElseCrash(position.y < m_height);
			return m_data.at(position.y * m_width + position.x);
		}

		// Retrieves constant reference to the value stored at the provided vector.
		virtual const T& at(const Vector2d<int>& position) const
		{
			VerifyElseCrash(position.x < m_width);
			VerifyElseCrash(position.y < m_height);
			return m_data.at(position.y * m_width + position.x);
		}

		// Retrieves reference to the value stored at the provided index.
		virtual T& at(int i)
		{
			return m_data.at(i);
		}

		// Retrieves constant reference to the value stored at the provided index.
		virtual const T& at(int i) const
		{
			return m_data.at(i);
		}

		// Swaps the underlying buffer
		void swap(Grid2d<T>& that)
		{
			VerifyElseCrash(m_width == that.Width() && m_height == that.Height());
			m_data.swap(that.m_data);
		}

		// Standard iterator overloads.
		std::vector<T>::iterator begin() noexcept
		{
			return m_data.begin();
		}
		std::vector<T>::iterator end() noexcept
		{
			return m_data.end();
		}
		std::vector<T>::const_iterator begin() const noexcept
		{
			return m_data.begin();
		}
		std::vector<T>::const_iterator end() const noexcept
		{
			return m_data.end();
		}
		std::vector<T>::const_iterator cbegin() const noexcept
		{
			return m_data.cbegin();
		}
		std::vector<T>::const_iterator cend() const noexcept
		{
			return m_data.cend();
		}
		std::vector<T>::reverse_iterator rbegin() noexcept
		{
			return m_data.rbegin();
		}
		std::vector<T>::reverse_iterator rend() noexcept
		{
			return m_data.rend();
		}
		std::vector<T>::const_reverse_iterator rbegin() const noexcept
		{
			return m_data.rbegin();
		}
		std::vector<T>::const_reverse_iterator rend() const noexcept
		{
			return m_data.rend();
		}
		std::vector<T>::const_reverse_iterator crbegin() const noexcept
		{
			return m_data.crbegin();
		}
		std::vector<T>::const_reverse_iterator crend() const noexcept
		{
			return m_data.crend();
		}

		// Spaceship operator overload for comparison operators.
		auto operator<=>(const Grid2d<T>&) const = default;

		// Insertion operator overload.
		friend std::ostream& operator<<(std::ostream& stream, const Grid2d<T>& grid)
		{
			auto bFirstRow = true;
			for (const auto& row : grid.GetRows())
			{
				if (bFirstRow)
				{
					bFirstRow = false;
				}
				else
				{
					stream << '\n';
				}

				auto bFirstCell = true;
				for (const auto& cell : row)
				{
					if (bFirstCell)
					{
						bFirstCell = false;
						stream << cell;
					}
					else
					{
						stream << ' ' << cell;
					}
				}
			}

			return stream;
		}

	private:
		int m_width = 0;
		int m_height = 0;
		std::vector<T> m_data;
	};
} // namespace Utilities

namespace std
{
	template<>
	struct hash<Utilities::Grid2d<char>>
	{
		size_t operator()(const Utilities::Grid2d<char>& value) const
		{
			auto result = 0ull;
			Utilities::HashCombine(result, value.Width(), value.Height(), std::string_view{ value.begin(), value.end() });
			return result;
		}
	};
} // namespace std