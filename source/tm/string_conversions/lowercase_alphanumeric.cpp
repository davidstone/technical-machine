// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <tm/string_conversions/lowercase_alphanumeric.hpp>

namespace technicalmachine {
namespace {

constexpr auto compare = [](auto const lhs, auto const rhs) {
	return containers::lexicographical_compare_3way(lowercase_alphanumeric(lhs), lowercase_alphanumeric(rhs)) < 0;
};

constexpr auto a = std::string_view("adaptability");
constexpr auto b = std::string_view("Adaptability");
static_assert(!compare(a, b));
static_assert(!compare(b, a));
constexpr auto c = std::string_view("Aftermath");
static_assert(compare(a, c));
static_assert(compare(b, c));
static_assert(!compare(c, a));
static_assert(!compare(c, b));
constexpr auto d = std::string_view("----aftermath----");
static_assert(compare(a, d));
static_assert(!compare(d, a));
static_assert(!compare(d, c));
static_assert(!compare(c, d));

} // namespace
} // namespace technical machine
