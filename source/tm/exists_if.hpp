// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <bounded/integer.hpp>

#include <compare>
#include <type_traits>

namespace technicalmachine {
using namespace bounded::literal;

// The size_t parameter is used to make this type unique if there are multiple
// conditional variables in a row with the same type
template<typename, std::size_t>
struct Empty {
	constexpr Empty(auto const & ...) {}
	constexpr Empty & operator=(auto const &) & {
		return *this;
	}
	friend auto operator<=>(Empty const &, Empty const &) = default;
	friend constexpr auto compress(Empty) {
		return 0_bi;
	}
};

template<typename T, bool condition, std::size_t index = 0>
using ExistsIf = std::conditional_t<condition, T, Empty<T, index>>;

template<typename T>
inline constexpr auto exists = true;

template<typename T, std::size_t index>
inline constexpr auto exists<Empty<T, index>> = false;

template<typename T>
inline constexpr auto exists<T const> = exists<T>;

template<typename T>
inline constexpr auto exists<T &> = exists<T>;

template<typename T>
inline constexpr auto exists<T &&> = exists<T>;

template<bool condition, std::size_t index = 0>
struct BoolIf {
	constexpr auto operator=(bool const value) & -> BoolIf & {
		m_value = value;
		return *this;
	}
	constexpr operator bool() const {
		return m_value;
	}

	friend constexpr auto compress(BoolIf const value) {
		return compress(value.m_value);
	}
private:
	bool m_value = false;
};

template<std::size_t index>
struct BoolIf<false, index> {
	constexpr auto operator=(bool) & -> BoolIf & {
		// TODO: Assert? Throw? Require the value to be false?
		return *this;
	}
	constexpr operator bool() const {
		return false;
	}

	friend constexpr auto compress(BoolIf) {
		return 0_bi;
	}
};

template<typename Integer, bool condition>
using IntegerIf = std::conditional_t<condition, Integer, bounded::constant_t<0>>;

} // namespace technicalmachine
