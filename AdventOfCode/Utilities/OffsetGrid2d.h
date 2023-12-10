#pragma once

#include "Grid2d.h"
#include "Vector2d.h"

namespace Utilities
{
	// Decorates Grid2d accessors with an (x, y) offset for convenience, supporting ranges of coordinates that do not begin at (0, 0).
	template<typename T>
	class OffsetGrid2d : public Grid2d<T>
	{
	public:
		template<std::ranges::input_range R>
		static std::pair<Vector2d<int>, Vector2d<int>> ComputeOffset(R&& range)
		{
			int minX = std::numeric_limits<int>::max();
			int minY = std::numeric_limits<int>::max();
			int maxX = std::numeric_limits<int>::min();
			int maxY = std::numeric_limits<int>::min();
			for (const auto& vec : range)
			{
				minX = std::min(minX, vec.x);
				minY = std::min(minY, vec.y);
				maxX = std::max(maxX, vec.x);
				maxY = std::max(maxY, vec.y);
			}

			return std::make_pair(
				Vector2d<int>{ static_cast<int>(maxX - minX + 1), static_cast<int>(maxY - minY + 1) },
				Vector2d<int>{ 0 - minX, 0 - minY });
		}

		template<std::ranges::input_range R>
		static OffsetGrid2d<T> CreateFromVector2ds(R&& range)
		{
			auto [dimensions, offset] = ComputeOffset(range);
			return OffsetGrid2d<T>{ dimensions.x, dimensions.y, offset.x, offset.y };
		}

	public:
		OffsetGrid2d(int width, int height, int offsetX, int offsetY)
			: Grid2d<T>(width, height)
			, m_offsetX(offsetX)
			, m_offsetY(offsetY)
		{
		}

		// X offset of the grid.
		int OffsetX() const
		{
			return m_offsetX;
		}

		// Y offset of the grid.
		int OffsetY() const
		{
			return m_offsetY;
		}

		Grid2d<T> CopyGridWithoutOffset() const
		{
			auto result = Grid2d<T>{ Grid2d<T>::Width(), Grid2d<T>::Height() };
			for (int i = 0; i < result.size(); ++i)
			{
				result.at(i) = Grid2d<T>::at(i);
			}
			return result;
		}

		// Converts an index for the underlying vector into (x, y) coordinates.
		virtual Vector2d<int> GetCoordinatesFromIndex(int i) const override
		{
			auto result = Grid2d<T>::GetCoordinatesFromIndex(i);
			return Vector2d<int>(result.x - m_offsetX, result.y - m_offsetY);
		}

		// Converts (x, y) coordinates into an index for the underlying vector.
		virtual int GetIndexFromCoordinates(int x, int y) const override
		{
			return Grid2d<T>::GetIndexFromCoordinates(x + m_offsetX, y + m_offsetY);
		}

		// Checks if the provided coordinates are within the bounds of the grid.
		virtual bool IsInBounds(int x, int y) const override
		{
			return Grid2d<T>::IsInBounds(x + m_offsetX, y + m_offsetY);
		}

		// Checks if the provided vector is within the bounds of the grid.
		virtual bool IsInBounds(const Vector2d<int>& position) const override
		{
			return IsInBounds(position.x, position.y);
		}

		// Retrieves reference to the value stored at the provided coordinates.
		virtual T& at(int x, int y) override
		{
			return Grid2d<T>::at(x + m_offsetX, y + m_offsetY);
		}

		// Retrieves constant reference to the value stored at the provided coordinates.
		virtual const T& at(int x, int y) const override
		{
			return Grid2d<T>::at(x + m_offsetX, y + m_offsetY);
		}

		// Retrieves reference to the value stored at the provided vector.
		virtual T& at(const Vector2d<int>& position) override
		{
			return Grid2d<T>::at(Vector2d<int>{ position.x + m_offsetX, position.y + m_offsetY });
		}

		// Retrieves constant reference to the value stored at the provided vector.
		virtual const T& at(const Vector2d<int>& position) const override
		{
			return Grid2d<T>::at(Vector2d<int>{ position.x + m_offsetX, position.y + m_offsetY });
		}

		// Retrieves reference to the value stored at the provided index.
		virtual T& at(int i) override
		{
			return Grid2d<T>::at(i);
		}

		// Retrieves constant reference to the value stored at the provided index.
		virtual const T& at(int i) const override
		{
			return Grid2d<T>::at(i);
		}

		// Swaps the underlying buffer
		void swap(OffsetGrid2d<T>& that)
		{
			VerifyElseCrash(m_offsetX == that.OffsetX() && m_offsetY == that.OffsetY());
			Grid2d<T>::swap(that);
		}

		// Spaceship operator overload for comparison operators.
		auto operator<=>(const OffsetGrid2d<T>&) const = default;

	private:
		int m_offsetX = 0;
		int m_offsetY = 0;
	};
} // namespace Utilities
