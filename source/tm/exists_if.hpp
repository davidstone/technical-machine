// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <tm/compress.hpp>

#include <bounded/integer.hpp>

#include <compare>
#include <type_traits>

namespace technicalmachine {
using namespace bounded::literal;

// The template parameter is used to make this type unique if there are multiple
// conditional variables in a row
template<auto = +[]{}>
struct Empty {
	constexpr explicit Empty(auto const & ...) {}
	constexpr Empty & operator=(auto const &) & {
		return *this;
	}
	friend auto operator<=>(Empty, Empty) = default;
	friend constexpr auto compress(Empty) {
		return 0_bi;
	}
};

template<typename T, bool condition>
using ExistsIf = std::conditional_t<condition, T, Empty<>>;

template<typename T>
inline constexpr auto exists = true;

template<auto unique>
inline constexpr auto exists<Empty<unique>> = false;

template<typename T>
inline constexpr auto exists<T const> = exists<T>;

template<typename T>
inline constexpr auto exists<T &> = exists<T>;

template<typename T>
inline constexpr auto exists<T &&> = exists<T>;

template<bool condition, auto = +[]{}>
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

template<auto unique>
struct BoolIf<false, unique> {
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
