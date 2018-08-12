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

#include <cctype>
#include <string_view>

namespace technicalmachine {

struct lowercase_ordering {
	auto operator()(std::string_view const lhs, std::string_view const rhs) const noexcept {
		auto const lowercase_comparison = [](auto const lhs_c, auto const rhs_c) {
			return std::tolower(lhs_c) < std::tolower(rhs_c);
		};
		return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end(), lowercase_comparison);
	}
};

template<typename T>
T from_string(std::string_view str);

}	// namespace technicalmachine
