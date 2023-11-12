#pragma once

#include "Hashing.h"

namespace Utilities
{
	template<typename T>
	struct Vector4d
	{
		T x = 0;
		T y = 0;
		T z = 0;
		T w = 0;

		// Addition operator
		Vector4d operator+(const Vector4d& that) const
		{
			return Vector4d{ x + that.x, y + that.y, z + that.z, w + that.w };
		}

		// Addition compound assignment operator
		Vector4d& operator+=(const Vector4d& that)
		{
			*this = *this + that;
			return *this;
		}

		// Subtraction operator
		Vector4d operator-(const Vector4d& that) const
		{
			return Vector4d{ x - that.x, y - that.y, z - that.z, w - that.w };
		}

		// Subtraction compound assignment operator
		Vector4d& operator-=(const Vector4d& that)
		{
			*this = *this - that;
			return *this;
		}

		// Scalar multiplication operator
		Vector4d operator*(T that) const
		{
			return Vector4d{ x * that, y * that, z * that, w * that };
		}

		// Scalar multiplication compound assignment operator
		Vector4d& operator*=(T that)
		{
			*this = *this * that;
			return *this;
		}

		// Spaceship operator overload for comparison operators
		auto operator<=>(const Vector4d&) const = default;

		// Returns string representation of the vector
		std::string ToString() const
		{
			auto stream = std::stringstream{};
			stream << *this;
			return stream.str();
		}

		// Insertion operator overload
		friend std::ostream& operator<<(std::ostream& stream, const Vector4d& vector)
		{
			stream << "(" << vector.x << ", " << vector.y << ", " << vector.z << ", " << vector.w << ")";
			return stream;
		}
	};
} // namespace Utilities

namespace std
{
	template<>
	struct hash<Utilities::Vector4d<int>>
	{
		size_t operator()(const Utilities::Vector4d<int>& value) const
		{
			auto result = 0ull;
			Utilities::HashCombine(result, value.x, value.y, value.z, value.w);
			return result;
		}
	};

	template<>
	struct hash<Utilities::Vector4d<int64_t>>
	{
		size_t operator()(const Utilities::Vector4d<int64_t>& value) const
		{
			auto result = 0ull;
			Utilities::HashCombine(result, value.x, value.y, value.z, value.w);
			return result;
		}
	};
} // namespace std