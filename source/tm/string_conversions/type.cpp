// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.string_conversions.type;

export import tm.string_conversions.from_string;
import tm.string_conversions.invalid_string_conversion;
import tm.string_conversions.lowercase_alphanumeric;

import tm.type.type;

import bounded;
import containers;
import std_module;

namespace technicalmachine {
using namespace bounded::literal;
using namespace containers::string_literals;

export constexpr auto to_string(Type const type) -> containers::string_view {
	switch (type) {
		case Type::Bug: return "Bug"_s;
		case Type::Dark: return "Dark"_s;
		case Type::Dragon: return "Dragon"_s;
		case Type::Electric: return "Electric"_s;
		case Type::Fairy: return "Fairy"_s;
		case Type::Fighting: return "Fighting"_s;
		case Type::Fire: return "Fire"_s;
		case Type::Flying: return "Flying"_s;
		case Type::Ghost: return "Ghost"_s;
		case Type::Grass: return "Grass"_s;
		case Type::Ground: return "Ground"_s;
		case Type::Ice: return "Ice"_s;
		case Type::Normal: return "Normal"_s;
		case Type::Poison: return "Poison"_s;
		case Type::Psychic: return "Psychic"_s;
		case Type::Rock: return "Rock"_s;
		case Type::Steel: return "Steel"_s;
		case Type::Water: return "Water"_s;
		case Type::Typeless: return "Typeless"_s;
	}
}

export template<>
constexpr auto from_string(containers::string_view const str) -> Type {
	static constexpr auto converter = containers::basic_flat_map(
		containers::assume_sorted_unique,
		containers::to_array<containers::map_value_type<containers::string_view, Type>>({
			{"bug"_s, Type::Bug},
			{"dark"_s, Type::Dark},
			{"dragon"_s, Type::Dragon},
			{"electric"_s, Type::Electric},
			{"fairy"_s, Type::Fairy},
			{"fighting"_s, Type::Fighting},
			{"fire"_s, Type::Fire},
			{"flying"_s, Type::Flying},
			{"ghost"_s, Type::Ghost},
			{"grass"_s, Type::Grass},
			{"ground"_s, Type::Ground},
			{"ice"_s, Type::Ice},
			{"normal"_s, Type::Normal},
			{"poison"_s, Type::Poison},
			{"psychic"_s, Type::Psychic},
			{"rock"_s, Type::Rock},
			{"steel"_s, Type::Steel},
			{"typeless"_s, Type::Typeless},
			{"water"_s, Type::Water},
		})
	);
	auto const converted = fixed_capacity_lowercase_and_digit_string<8_bi>(str);
	auto const result = containers::lookup(converter, converted);
	if (!result) {
		throw InvalidFromStringConversion("Type"_s, str);
	}
	return *result;
}

} // namespace technicalmachine
