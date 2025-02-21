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

export constexpr auto to_string(Type const type) -> std::string_view {
	switch (type) {
		case Type::Bug: return "Bug";
		case Type::Dark: return "Dark";
		case Type::Dragon: return "Dragon";
		case Type::Electric: return "Electric";
		case Type::Fairy: return "Fairy";
		case Type::Fighting: return "Fighting";
		case Type::Fire: return "Fire";
		case Type::Flying: return "Flying";
		case Type::Ghost: return "Ghost";
		case Type::Grass: return "Grass";
		case Type::Ground: return "Ground";
		case Type::Ice: return "Ice";
		case Type::Normal: return "Normal";
		case Type::Poison: return "Poison";
		case Type::Psychic: return "Psychic";
		case Type::Rock: return "Rock";
		case Type::Steel: return "Steel";
		case Type::Water: return "Water";
		case Type::Typeless: return "Typeless";
	}
}

export template<>
constexpr auto from_string(std::string_view const str) -> Type {
	static constexpr auto converter = containers::basic_flat_map(
		containers::assume_sorted_unique,
		containers::to_array<containers::map_value_type<std::string_view, Type>>({
			{"bug", Type::Bug},
			{"dark", Type::Dark},
			{"dragon", Type::Dragon},
			{"electric", Type::Electric},
			{"fairy", Type::Fairy},
			{"fighting", Type::Fighting},
			{"fire", Type::Fire},
			{"flying", Type::Flying},
			{"ghost", Type::Ghost},
			{"grass", Type::Grass},
			{"ground", Type::Ground},
			{"ice", Type::Ice},
			{"normal", Type::Normal},
			{"poison", Type::Poison},
			{"psychic", Type::Psychic},
			{"rock", Type::Rock},
			{"steel", Type::Steel},
			{"typeless", Type::Typeless},
			{"water", Type::Water},
		})
	);
	auto const converted = fixed_capacity_lowercase_and_digit_string<8_bi>(str);
	auto const result = containers::lookup(converter, converted);
	if (!result) {
		throw InvalidFromStringConversion("Type", str);
	}
	return *result;
}

} // namespace technicalmachine
