// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.exists_if;

import tm.compress;

import bounded;
import std_module;

namespace technicalmachine {
using namespace bounded::literal;

// The template parameter is used to make this type unique if there are multiple
// conditional variables in a row
template<typename>
struct Empty {
	Empty() = default;
	constexpr explicit Empty(auto const & ...) {}
	constexpr Empty & operator=(auto const &) & {
		return *this;
	}
	friend auto operator<=>(Empty, Empty) = default;
};

export template<typename T, bool condition, typename unique = void>
using ExistsIf = std::conditional_t<condition, T, Empty<unique>>;

export template<typename T>
constexpr auto exists = true;

template<typename unique>
constexpr auto exists<Empty<unique>> = false;

template<typename T>
constexpr auto exists<T const> = exists<T>;

template<typename T>
constexpr auto exists<T &> = exists<T>;

template<typename T>
constexpr auto exists<T &&> = exists<T>;

export template<bool condition, typename = void>
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

template<typename unique>
struct BoolIf<false, unique> {
	constexpr auto operator=(bool) & -> BoolIf & {
		// TODO: Assert? Throw? Require the value to be false?
		return *this;
	}
	constexpr operator bool() const {
		return false;
	}
};

export template<typename Integer, bool condition>
using IntegerIf = std::conditional_t<condition, Integer, bounded::constant_t<0>>;

} // namespace technicalmachine
