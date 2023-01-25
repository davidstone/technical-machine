// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.string_conversions.lowercase_alphanumeric;

import containers;
import std_module;

namespace technicalmachine {

export constexpr auto lowercase_alphanumeric = [](std::string_view input) {
	// Not portable because it does not respect character encodings.
	// We do not want to use cctype functions because we do not want to
	// use locales.
	auto to_lower = [](char c) {
		return static_cast<char>(('A' <= c and c <= 'Z') ? c + 'a' - 'A' : c);
	};
	static_assert(to_lower('A') == 'a');
	static_assert(to_lower('B') == 'b');
	static_assert(to_lower('Z') == 'z');
	static_assert(to_lower('a') == 'a');
	static_assert(to_lower('b') == 'b');
	static_assert(to_lower('0') == '0');
	auto is_valid = [](char c) {
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
	return containers::filter(containers::transform(std::move(input), to_lower), is_valid);
};

export template<auto max_size>
struct fixed_capacity_lowercase_and_digit_string {
	constexpr explicit fixed_capacity_lowercase_and_digit_string(std::string_view const str):
		m_data(containers::take(lowercase_alphanumeric(str), max_size))
	{
	}
	constexpr operator std::string_view() const {
		return std::string_view(containers::data(m_data), containers::size(m_data).value());
	}

private:
	containers::static_vector<char, max_size> m_data;
};

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

} // namespace technicalmachine
