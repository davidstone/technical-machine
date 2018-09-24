// String conversions template
// Copyright (C) 2018 David Stone
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

#include <containers/algorithms/compare.hpp>
#include <containers/algorithms/filter_iterator.hpp>
#include <containers/algorithms/transform.hpp>

#include <string_view>

namespace technicalmachine {

struct lowercase_alphanumeric {
	constexpr auto operator()(std::string_view const lhs, std::string_view const rhs) const noexcept {
		auto transform_filter = [](std::string_view const & input) {
			// Not portable because it does not respect character encodings.
			// We do not want to use cctype functions because we do not want to
			// use locales.
			auto to_lower = [](char c) {
				return static_cast<char>(('A' <= c and c <= 'Z') ? c + 'a' - 'A' : c);
			};
			auto is_valid = [](char c) {
				return ('0' <= c and c <= '9') or ('a' <= c and c <= 'z');
			};
			return containers::filter(containers::transform(input, to_lower), is_valid);
		};
		return containers::compare(transform_filter(lhs), transform_filter(rhs)) < 0;
	}
};

template<typename T>
T from_string(std::string_view str);

}	// namespace technicalmachine
