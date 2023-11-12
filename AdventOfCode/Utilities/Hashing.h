#pragma once

namespace Utilities
{
	// Creates a new hash by hashing the provided values. Algorithm taken from Boost's hash_combine function.
	// https://www.boost.org/doc/libs/1_37_0/doc/html/hash/reference.html#boost.hash_combine
	template<typename T>
	void HashCombine(size_t& seed, const T& value)
	{
		auto hasher = std::hash<T>{};
		seed ^= hasher(value) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
	}

	// Variadic hash combiner.
	template<typename T, typename... Args>
	void HashCombine(size_t& seed, const T& value, const Args&... args)
	{
		HashCombine(seed, value);
		HashCombine(seed, args...);
	}

	// Hashes a range. Order matters. Algorithm taken from Boost's hash_range function.
	// https://www.boost.org/doc/libs/1_37_0/doc/html/hash/reference.html#boost.hash_range
	template <typename It>
	void HashRange(size_t& seed, It first, It last)
	{
		for (; first != last; ++first)
		{
			HashCombine(seed, *first);
		}
	}
}

// Macro to implement std::hash specialization for provided type, enabling use in std::unordered_map and std::unordered_set.
// Reference members in variadic arguments using "value" prefix.
// Sample usage:
//   struct Point { int x = 0; int y = 0; };
//   IMPLEMENT_STD_HASH(Point, value.x, value.y);
//   [...]
//   std::undordered_set<Point> pointSet;
#define IMPLEMENT_STD_HASH(T, ...) \
	namespace std\
	{\
		template<>\
		struct hash<T>\
		{\
			size_t operator()(const T& value) const\
			{\
				auto result = 0ull;\
				Utilities::HashCombine(result, __VA_ARGS__);\
				return result;\
			}\
		};\
	}