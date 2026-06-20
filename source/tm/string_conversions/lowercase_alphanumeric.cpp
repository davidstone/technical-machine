// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.string_conversions.lowercase_alphanumeric;

import bounded;
import containers;
import std_module;

namespace technicalmachine {
using namespace containers::string_literals;

// Not portable because it does not respect character encodings.
// We do not want to use cctype functions because we do not want to
// use locales.
constexpr auto to_lower(char const c) -> char {
	return static_cast<char>(('A' <= c and c <= 'Z') ? c + 'a' - 'A' : c);
};
static_assert(to_lower('A') == 'a');
static_assert(to_lower('B') == 'b');
static_assert(to_lower('Z') == 'z');
static_assert(to_lower('a') == 'a');
static_assert(to_lower('b') == 'b');
static_assert(to_lower('0') == '0');

constexpr auto is_valid(char const c) -> bool {
	return ('0' <= c and c <= '9') or ('a' <= c and c <= 'z');
};
static_assert(is_valid('0'));
static_assert(is_valid('1'));
static_assert(is_valid('9'));
static_assert(is_valid('a'));
static_assert(is_valid('b'));
static_assert(is_valid('z'));
static_assert(!is_valid('A'));
static_assert(!is_valid('D'));
static_assert(!is_valid('Z'));
static_assert(!is_valid('%'));
static_assert(!is_valid(' '));

export constexpr auto lowercase_alphanumeric = [](containers::string_view const input) {
	return containers::filter(containers::transform(std::move(input), to_lower), is_valid);
};

export template<auto max_size>
struct fixed_capacity_lowercase_and_digit_string {
	constexpr explicit fixed_capacity_lowercase_and_digit_string(containers::string_view const str):
		m_data(containers::take(lowercase_alphanumeric(str), max_size))
	{
	}
	constexpr operator containers::string_view() const {
		return containers::string_view(m_data);
	}

private:
	containers::static_vector<char, max_size> m_data;
};

constexpr auto compare(containers::string_view const lhs, containers::string_view const rhs) {
	return containers::lexicographical_compare(lowercase_alphanumeric(lhs), lowercase_alphanumeric(rhs)) < 0;
}

constexpr auto a = "adaptability"_s;
constexpr auto b = "Adaptability"_s;
static_assert(!compare(a, b));
static_assert(!compare(b, a));
constexpr auto c = "Aftermath"_s;
static_assert(compare(a, c));
static_assert(compare(b, c));
static_assert(!compare(c, a));
static_assert(!compare(c, b));
constexpr auto d = "----aftermath----"_s;
static_assert(compare(a, d));
static_assert(!compare(d, a));
static_assert(!compare(d, c));
static_assert(!compare(c, d));

} // namespace technicalmachine
