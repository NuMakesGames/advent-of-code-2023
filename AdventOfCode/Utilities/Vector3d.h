#pragma once

#include "Hashing.h"

namespace Utilities
{
	template<typename T>
	struct Vector3d
	{
		T x = 0;
		T y = 0;
		T z = 0;

		// Addition operator
		Vector3d operator+(const Vector3d& that) const
		{
			return Vector3d{ x + that.x, y + that.y, z + that.z };
		}

		// Addition compound assignment operator
		Vector3d& operator+=(const Vector3d& that)
		{
			*this = *this + that;
			return *this;
		}

		// Subtraction operator
		Vector3d operator-(const Vector3d& that) const
		{
			return Vector3d{ x - that.x, y - that.y, z - that.z };
		}

		// Subtraction compound assignment operator
		Vector3d& operator-=(const Vector3d& that)
		{
			*this = *this - that;
			return *this;
		}

		// Scalar multiplication operator
		Vector3d operator*(T that) const
		{
			return Vector3d{ x * that, y * that, z * that };
		}

		// Scalar multiplication compound assignment operator
		Vector3d& operator*=(T that)
		{
			*this = *this * that;
			return *this;
		}

		// Spaceship operator overload for comparison operators
		auto operator<=>(const Vector3d&) const = default;

		// Returns string representation of the vector
		std::string ToString() const
		{
			auto stream = std::stringstream{};
			stream << *this;
			return stream.str();
		}

		// Insertion operator overload
		friend std::ostream& operator<<(std::ostream& stream, const Vector3d& vector)
		{
			stream << "(" << vector.x << ", " << vector.y << ", " << vector.z << ")";
			return stream;
		}
	};
} // namespace Utilities

namespace std
{
	template<>
	struct hash<Utilities::Vector3d<int>>
	{
		size_t operator()(const Utilities::Vector3d<int>& value) const
		{
			auto result = 0ull;
			Utilities::HashCombine(result, value.x, value.y, value.z);
			return result;
		}
	};

	template<>
	struct hash<Utilities::Vector3d<int64_t>>
	{
		size_t operator()(const Utilities::Vector3d<int64_t>& value) const
		{
			auto result = 0ull;
			Utilities::HashCombine(result, value.x, value.y, value.z);
			return result;
		}
	};
} // namespace std