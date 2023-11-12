#pragma once

#include "Hashing.h"

namespace Utilities
{
	template<typename T>
	struct Vector2d
	{
		T x = 0;
		T y = 0;

		// Addition operator
		Vector2d operator+(const Vector2d& that) const
		{
			return Vector2d{ x + that.x, y + that.y };
		}

		// Addition compound assignment operator
		Vector2d& operator+=(const Vector2d& that)
		{
			*this = *this + that;
			return *this;
		}

		// Subtraction operator
		Vector2d operator-(const Vector2d& that) const
		{
			return Vector2d{ x - that.x, y - that.y };
		}

		// Subtraction compound assignment operator
		Vector2d& operator-=(const Vector2d& that)
		{
			*this = *this - that;
			return *this;
		}

		// Scalar multiplication operator
		Vector2d operator*(T that) const
		{
			return Vector2d{ x * that, y * that };
		}

		// Scalar multiplication compound assignment operator
		Vector2d& operator*=(T that)
		{
			*this = *this * that;
			return *this;
		}

		// Spaceship operator overload for comparison operators
		auto operator<=>(const Vector2d&) const = default;

		// Returns string representation of the vector
		std::string ToString() const
		{
			auto stream = std::stringstream{};
			stream << *this;
			return stream.str();
		}

		// Insertion operator overload
		friend std::ostream& operator<<(std::ostream& stream, const Vector2d& vector)
		{
			stream << "(" << vector.x << ", " << vector.y << ")";
			return stream;
		}
	};
}

namespace std
{
	template<>
	struct hash<Utilities::Vector2d<int>>
	{
		size_t operator()(const Utilities::Vector2d<int>& value) const
		{
			auto result = 0ull;
			Utilities::HashCombine(result, value.x, value.y);
			return result;
		}
	};

	template<>
	struct hash<Utilities::Vector2d<int64_t>>
	{
		size_t operator()(const Utilities::Vector2d<int64_t>& value) const
		{
			auto result = 0ull;
			Utilities::HashCombine(result, value.x, value.y);
			return result;
		}
	};
} // namespace std