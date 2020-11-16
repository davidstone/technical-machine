// Transformation for ska_sort to sort case-insensitive by alphanumeric only
// Copyright (C) 2019 David Stone
//
// This file is part of Technical Machine.
//
// Technical Machine is free software: you can redistribute it and / or modify
// it under the terms of the GNU Affero General Public License as
// published by the Free Software Foundation, either version 3 of the
// License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.

#pragma once

#include <containers/algorithms/filter_iterator.hpp>
#include <containers/algorithms/transform.hpp>
#include <containers/static_vector/static_vector.hpp>
#include <containers/data.hpp>
#include <containers/take.hpp>

#include <string_view>

namespace technicalmachine {

inline constexpr auto lowercase_alphanumeric = [](std::string_view input) {
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

template<int max_size>
struct fixed_capacity_lowercase_and_digit_string {
	constexpr explicit fixed_capacity_lowercase_and_digit_string(std::string_view const str):
		m_data(containers::take(lowercase_alphanumeric(str), bounded::constant<max_size>))
	{
	}
	constexpr operator std::string_view() const {
		return std::string_view(containers::data(m_data), size(m_data).value());
	}

private:
	containers::static_vector<char, max_size> m_data;
};

}	// namespace technicalmachine
