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
		case Type::Bug: return "Bug"_sv;
		case Type::Dark: return "Dark"_sv;
		case Type::Dragon: return "Dragon"_sv;
		case Type::Electric: return "Electric"_sv;
		case Type::Fairy: return "Fairy"_sv;
		case Type::Fighting: return "Fighting"_sv;
		case Type::Fire: return "Fire"_sv;
		case Type::Flying: return "Flying"_sv;
		case Type::Ghost: return "Ghost"_sv;
		case Type::Grass: return "Grass"_sv;
		case Type::Ground: return "Ground"_sv;
		case Type::Ice: return "Ice"_sv;
		case Type::Normal: return "Normal"_sv;
		case Type::Poison: return "Poison"_sv;
		case Type::Psychic: return "Psychic"_sv;
		case Type::Rock: return "Rock"_sv;
		case Type::Steel: return "Steel"_sv;
		case Type::Water: return "Water"_sv;
		case Type::Typeless: return "Typeless"_sv;
	}
}

export template<>
constexpr auto from_string(containers::string_view const str) -> Type {
	static constexpr auto converter = containers::basic_flat_map(
		containers::assume_sorted_unique,
		containers::to_array<containers::map_value_type<containers::string_view, Type>>({
			{"bug"_sv, Type::Bug},
			{"dark"_sv, Type::Dark},
			{"dragon"_sv, Type::Dragon},
			{"electric"_sv, Type::Electric},
			{"fairy"_sv, Type::Fairy},
			{"fighting"_sv, Type::Fighting},
			{"fire"_sv, Type::Fire},
			{"flying"_sv, Type::Flying},
			{"ghost"_sv, Type::Ghost},
			{"grass"_sv, Type::Grass},
			{"ground"_sv, Type::Ground},
			{"ice"_sv, Type::Ice},
			{"normal"_sv, Type::Normal},
			{"poison"_sv, Type::Poison},
			{"psychic"_sv, Type::Psychic},
			{"rock"_sv, Type::Rock},
			{"steel"_sv, Type::Steel},
			{"typeless"_sv, Type::Typeless},
			{"water"_sv, Type::Water},
		})
	);
	auto const converted = fixed_capacity_lowercase_and_digit_string<8_bi>(str);
	auto const result = containers::lookup(converter, converted);
	if (!result) {
		throw InvalidFromStringConversion("Type"_sv, str);
	}
	return *result;
}

} // namespace technicalmachine
