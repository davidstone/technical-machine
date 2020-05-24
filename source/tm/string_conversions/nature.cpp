// Nature string conversions
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

#include <tm/string_conversions/nature.hpp>

#include <tm/string_conversions/invalid_string_conversion.hpp>
#include <tm/string_conversions/lowercase_alphanumeric.hpp>

#include <tm/stat/nature.hpp>

#include <containers/array/array.hpp>
#include <containers/flat_map.hpp>

namespace technicalmachine {

auto to_string(Nature const nature) -> std::string_view {
	switch (nature) {
		case Nature::Adamant: return "Adamant";
		case Nature::Bashful: return "Bashful";
		case Nature::Bold: return "Bold";
		case Nature::Brave: return "Brave";
		case Nature::Calm: return "Calm";
		case Nature::Careful: return "Careful";
		case Nature::Docile: return "Docile";
		case Nature::Gentle: return "Gentle";
		case Nature::Hardy: return "Hardy";
		case Nature::Hasty: return "Hasty";
		case Nature::Impish: return "Impish";
		case Nature::Jolly: return "Jolly";
		case Nature::Lax: return "Lax";
		case Nature::Lonely: return "Lonely";
		case Nature::Mild: return "Mild";
		case Nature::Modest: return "Modest";
		case Nature::Naive: return "Naive";
		case Nature::Naughty: return "Naughty";
		case Nature::Quiet: return "Quiet";
		case Nature::Quirky: return "Quirky";
		case Nature::Rash: return "Rash";
		case Nature::Relaxed: return "Relaxed";
		case Nature::Sassy: return "Sassy";
		case Nature::Serious: return "Serious";
		case Nature::Timid: return "Timid";
	}
}

template<>
auto from_string<Nature>(std::string_view const str) -> Nature {
	using Storage = containers::array<containers::map_value_type<std::string_view, Nature>, 25>;
	static constexpr auto converter = containers::basic_flat_map<Storage>(
		containers::assume_sorted_unique,
		Storage{{
			{ "adamant", Nature::Adamant },
			{ "bashful", Nature::Bashful },
			{ "bold", Nature::Bold },
			{ "brave", Nature::Brave },
			{ "calm", Nature::Calm },
			{ "careful", Nature::Careful },
			{ "docile", Nature::Docile },
			{ "gentle", Nature::Gentle },
			{ "hardy", Nature::Hardy },
			{ "hasty", Nature::Hasty },
			{ "impish", Nature::Impish },
			{ "jolly", Nature::Jolly },
			{ "lax", Nature::Lax },
			{ "lonely", Nature::Lonely },
			{ "mild", Nature::Mild },
			{ "modest", Nature::Modest },
			{ "naive", Nature::Naive },
			{ "naughty", Nature::Naughty },
			{ "quiet", Nature::Quiet },
			{ "quirky", Nature::Quirky },
			{ "rash", Nature::Rash },
			{ "relaxed", Nature::Relaxed },
			{ "sassy", Nature::Sassy },
			{ "serious", Nature::Serious },
			{ "timid", Nature::Timid }
		}}
	);
	auto const converted = fixed_capacity_lowercase_and_digit_string<7>(str);
	auto const it = converter.find(converted);
	if (it != end(converter)) {
		return it->mapped();
	} else {
		throw InvalidFromStringConversion("Nature", str);
	}
}

}	// namespace technicalmachine
