#pragma once

#include "Grid3d.h"
#include "Vector3d.h"

namespace Utilities
{
	// Decorates Grid3d accessors with an (x, y, z) offset for convenience, supporting ranges of coordinates that do not begin at (0, 0, 0).
	template<typename T>
	class OffsetGrid3d : public Grid3d<T>
	{
	public:
		template<std::ranges::input_range R>
		static std::pair<Vector3d<int>, Vector3d<int>> ComputeOffset(R&& range)
		{
			int minX = std::numeric_limits<int>::max();
			int minY = std::numeric_limits<int>::max();
			int minZ = std::numeric_limits<int>::max();
			int maxX = std::numeric_limits<int>::min();
			int maxY = std::numeric_limits<int>::min();
			int maxZ = std::numeric_limits<int>::min();
			for (const auto& vec : range)
			{
				minX = std::min(minX, vec.x);
				minY = std::min(minY, vec.y);
				minZ = std::min(minZ, vec.z);
				maxX = std::max(maxX, vec.x);
				maxY = std::max(maxY, vec.y);
				maxZ = std::max(maxZ, vec.z);
			}

			return std::make_pair(
				Vector3d<int>{ static_cast<int>(maxX - minX + 1), static_cast<int>(maxY - minY + 1), static_cast<int>(maxZ - minZ + 1) },
				Vector3d<int>{ 0 - minX, 0 - minY, 0 - minZ });
		}

		template<std::ranges::input_range R>
		static OffsetGrid3d<T> CreateFromVector3ds(R&& range)
		{
			auto [dimensions, offset] = ComputeOffset(range);
			return OffsetGrid3d<T>{ dimensions.x, dimensions.y, dimensions.z, offset.x, offset.y, offset.z };
		}

	public:
		OffsetGrid3d(int width, int height, int depth, int offsetX, int offsetY, int offsetZ)
			: Grid3d<T>(width, height, depth)
			, m_offsetX(offsetX)
			, m_offsetY(offsetY)
			, m_offsetZ(offsetZ)
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

		// Z offset of the grid.
		int OffsetZ() const
		{
			return m_offsetZ;
		}

		Grid3d<T> CopyGridWithoutOffset() const
		{
			auto result = Grid3d<T>{ Grid3d<T>::Width(), Grid3d<T>::Height(), Grid3d<T>::Depth() };
			for (int i = 0; i < result.size(); ++i)
			{
				result.at(i) = Grid3d<T>::at(i);
			}
			return result;
		}

		// Converts an index for the underlying vector into (x, y, z) coordinates.
		virtual Vector3d<int> GetCoordinatesFromIndex(int i) const override
		{
			auto result = Grid3d<T>::GetCoordinatesFromIndex(i);
			return Vector3d<int>(result.x - m_offsetX, result.y - m_offsetY, result.z - m_offsetZ);
		}

		// Converts (x, y, z) coordinates into an index for the underlying vector.
		virtual int GetIndexFromCoordinates(int x, int y, int z) const override
		{
			return Grid3d<T>::GetIndexFromCoordinates(x + m_offsetX, y + m_offsetY, z + m_offsetZ);
		}

		// Checks if the provided coordinates are within the bounds of the grid.
		virtual bool IsInBounds(int x, int y, int z) const override
		{
			return Grid3d<T>::IsInBounds(x + m_offsetX, y + m_offsetY, z + m_offsetZ);
		}

		// Checks if the provided vector is within the bounds of the grid.
		virtual bool IsInBounds(const Vector3d<int>& position) const override
		{
			return IsInBounds(position.x, position.y, position.z);
		}

		// Retrieves reference to the value stored at the provided coordinates.
		virtual T& at(int x, int y, int z) override
		{
			return Grid3d<T>::at(x + m_offsetX, y + m_offsetY, z + m_offsetZ);
		}

		// Retrieves constant reference to the value stored at the provided coordinates.
		virtual const T& at(int x, int y, int z) const override
		{
			return Grid3d<T>::at(x + m_offsetX, y + m_offsetY, z + m_offsetZ);
		}

		// Retrieves reference to the value stored at the provided vector.
		virtual T& at(const Vector3d<int>& position) override
		{
			return Grid3d<T>::at(Vector3d<int>{ position.x + m_offsetX, position.y + m_offsetY, position.z + m_offsetZ });
		}

		// Retrieves constant reference to the value stored at the provided vector.
		virtual const T& at(const Vector3d<int>& position) const override
		{
			return Grid3d<T>::at(Vector3d<int>{ position.x + m_offsetX, position.y + m_offsetY, position.z + m_offsetZ });
		}

		// Retrieves reference to the value stored at the provided index.
		virtual T& at(int i) override
		{
			return Grid3d<T>::at(i);
		}

		// Retrieves constant reference to the value stored at the provided index.
		virtual const T& at(int i) const override
		{
			return Grid3d<T>::at(i);
		}

		// Swaps the underlying buffer
		void swap(OffsetGrid3d<T>& that)
		{
			VerifyElseCrash(m_offsetX == that.OffsetX() && m_offsetY == that.OffsetY() && m_offsetZ == that.OffsetZ());
			Grid3d<T>::swap(that);
		}

		// Spaceship operator overload for comparison operators.
		auto operator<=>(const OffsetGrid3d<T>&) const = default;

	private:
		int m_offsetX = 0;
		int m_offsetY = 0;
		int m_offsetZ = 0;
	};
} // namespace Utilities
