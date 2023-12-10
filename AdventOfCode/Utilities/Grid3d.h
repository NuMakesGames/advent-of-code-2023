#pragma once

#include "Assertions.h"
#include "Vector3d.h"

namespace Utilities
{
	// Wraps a std::vector and for easier three-dimensional addressing.
	// Grid dimensions cannot be changed after construction.
	template <typename T>
	class Grid3d
	{
	public:
		// Initializes grid to specified width and height. Default initializes.
		Grid3d(int width, int height, int depth)
			: m_width(width), m_height(height), m_depth(depth), m_data(m_width * m_height * m_depth)
		{
		}

		Grid3d() : Grid3d(0, 0, 0)
		{
		}

		virtual ~Grid3d()
		{
		}

		// Width of the grid.
		int Width() const { return m_width; }

		// Height of the grid.
		int Height() const { return m_height; }

		// Depth of the grid.
		int Depth() const { return m_depth; }

		// Resizes the grid and attempts to maintain existing elements.
		void resize(int width, int height, int depth)
		{
			auto newGrid = Grid3d<T>(width, height, depth);
			for (int z = 0; z < std::min(depth, m_depth); ++z)
			{
				for (int y = 0; y < std::min(height, m_height); ++y)
				{
					for (int x = 0; x < std::min(width, m_width); ++x)
					{
						newGrid.at(x, y, z) = this->at(x, y, z);
					}
				}
			}
			*this = std::move(newGrid);
		}

		// Returns the size of the underlying vector.
		size_t size() const { return m_data.size(); }

		// Converts an index for the underlying vector into (x, y, z) coordinates.
		virtual Vector3d<int> GetCoordinatesFromIndex(int i) const
		{
			VerifyElseCrash(i < m_data.size());
			auto z = i / (m_width * m_height);
			i -= z * m_width * m_height;
			auto y = i / m_width;
			auto x = i % m_width;
			return Vector3d<int>(x, y, z);
		}

		// Converts (x, y, z) coordinates into an index for the underlying vector.
		virtual int GetIndexFromCoordinates(int x, int y, int z) const
		{
			VerifyElseCrash(x < m_width);
			VerifyElseCrash(y < m_height);
			VerifyElseCrash(z < m_depth);
			return (z * m_height + y) * m_width + x;
		}

		// Checks if the provided coordinates are within the bounds of the grid.
		virtual bool IsInBounds(int x, int y, int z) const
		{
			return x >= 0 && x < m_width && y >= 0 && y < m_height && z >= 0 && z < m_depth;
		}

		// Checks if the provided vector is within the bounds of the grid.
		virtual bool IsInBounds(const Vector3d<int>& position) const
		{
			return IsInBounds(position.x, position.y, position.z);
		}

		// Retrieves reference to the value stored at the provided coordinates.
		virtual T& at(int x, int y, int z)
		{
			VerifyElseCrash(x < m_width);
			VerifyElseCrash(y < m_height);
			VerifyElseCrash(z < m_depth);
			return m_data.at((z * m_height + y) * m_width + x);
		}

		// Retrieves constant reference to the value stored at the provided coordinates.
		virtual const T& at(int x, int y, int z) const
		{
			VerifyElseCrash(x < m_width);
			VerifyElseCrash(y < m_height);
			VerifyElseCrash(z < m_depth);
			return m_data.at((z * m_height + y) * m_width + x);
		}

		// Retrieves reference to the value stored at the provided vector.
		virtual T& at(const Vector3d<int>& position)
		{
			VerifyElseCrash(position.x < m_width);
			VerifyElseCrash(position.y < m_height);
			VerifyElseCrash(position.z < m_depth);
			return m_data.at((position.z * m_height + position.y) * m_width + position.x);
		}

		// Retrieves constant reference to the value stored at the provided vector.
		virtual const T& at(const Vector3d<int>& position) const
		{
			VerifyElseCrash(position.x < m_width);
			VerifyElseCrash(position.y < m_height);
			VerifyElseCrash(position.z < m_depth);
			return m_data.at((position.z * m_height + position.y) * m_width + position.x);
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
		void swap(Grid3d<T>& that)
		{
			VerifyElseCrash(m_width == that.Width() && m_height == that.Height() && m_depth == that.Depth());
			m_data.swap(that.m_data);
		}

		// Standard iterator overloads.
		std::vector<T>::iterator begin() noexcept { return m_data.begin(); }
		std::vector<T>::iterator end() noexcept { return m_data.end(); }
		std::vector<T>::const_iterator begin() const noexcept { return m_data.begin(); }
		std::vector<T>::const_iterator end() const noexcept { return m_data.end(); }
		std::vector<T>::const_iterator cbegin() const noexcept { return m_data.cbegin(); }
		std::vector<T>::const_iterator cend() const noexcept { return m_data.cend(); }
		std::vector<T>::reverse_iterator rbegin() noexcept { return m_data.rbegin(); }
		std::vector<T>::reverse_iterator rend() noexcept { return m_data.rend(); }
		std::vector<T>::const_reverse_iterator rbegin() const noexcept { return m_data.rbegin(); }
		std::vector<T>::const_reverse_iterator rend() const noexcept { return m_data.rend(); }
		std::vector<T>::const_reverse_iterator crbegin() const noexcept { return m_data.crbegin(); }
		std::vector<T>::const_reverse_iterator crend() const noexcept { return m_data.crend(); }

		// Spaceship operator overload for comparison operators.
		auto operator<=>(const Grid3d<T>&) const = default;

		// Insertion operator overload.
		friend std::ostream& operator<<(std::ostream& stream, const Grid3d<T>& grid)
		{
			for (int z = 0; z < grid.Depth(); ++z)
			{
				if (z > 0)
				{
					stream << "\n\n";
				}
				stream << "z = " << z << '\n';
				for (int y = 0; y < grid.Height(); ++y)
				{
					for (int x = 0; x < grid.Width(); ++x)
					{
						if (x > 0)
						{
							stream << ' ';
						}
						stream << grid.m_data.at((z * grid.m_height + y) * grid.m_width + x);
					}

					if (y < grid.Height() - 1)
					{
						stream << '\n';
					}
				}
			}

			return stream;
		}

	private:
		int m_width = 0;
		int m_height = 0;
		int m_depth = 0;
		std::vector<T> m_data;
	};
}

namespace std
{
	template<>
	struct hash<Utilities::Grid3d<char>>
	{
		size_t operator()(const Utilities::Grid3d<char>& value) const
		{
			auto result = 0ull;
			Utilities::HashCombine(result, value.Width(), value.Height(), value.Depth(), std::string_view{ value.begin(), value.end() });
			return result;
		}
	};
} // namespace std