// Gender string conversions
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

#include <tm/string_conversions/gender.hpp>

#include <tm/string_conversions/conversion.hpp>
#include <tm/string_conversions/invalid_string_conversion.hpp>
#include <tm/string_conversions/lowercase_alphanumeric.hpp>

#include <tm/gender.hpp>

#include <containers/array/array.hpp>
#include <containers/begin_end.hpp>
#include <containers/flat_map.hpp>

namespace technicalmachine {

auto to_string(Gender const gender) -> std::string_view {
	switch (gender) {
		case Gender::female: return "Female";
		case Gender::male: return "Male";
		case Gender::genderless: return "Genderless";
	}
}

template<>
auto from_string(std::string_view const str) -> Gender {
	using Storage = containers::array<containers::map_value_type<std::string_view, Gender>, 5>;
	constexpr auto converter = containers::basic_flat_map<Storage>(
		containers::assume_sorted_unique,
		Storage{{
			{ "female", Gender::female },
			{ "genderless", Gender::genderless },
			{ "male", Gender::male },
			{ "nogender", Gender::genderless },
			{ "none", Gender::genderless },
		}}
	);
	auto const converted = fixed_capacity_lowercase_and_digit_string<10>(str);
	auto const it = converter.find(converted);
	if (it != containers::end(converter)) {
		return it->mapped();
	} else {
		throw InvalidFromStringConversion("Gender", str);
	}
}

}	// namespace technicalmachine
