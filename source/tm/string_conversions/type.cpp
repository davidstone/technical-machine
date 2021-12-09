// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <tm/string_conversions/type.hpp>

#include <tm/string_conversions/invalid_string_conversion.hpp>
#include <tm/string_conversions/lowercase_alphanumeric.hpp>

#include <containers/array.hpp>
#include <containers/begin_end.hpp>
#include <containers/flat_map.hpp>
#include <containers/lookup.hpp>

namespace technicalmachine {

auto to_string(Type const type) -> std::string_view {
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

template<>
auto from_string(std::string_view const str) -> Type {
	constexpr auto converter = containers::basic_flat_map(
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
	auto const converted = fixed_capacity_lowercase_and_digit_string<8>(str);
	auto const result = containers::lookup(converter, converted);
	if (!result) {
		throw InvalidFromStringConversion("Type", str);
	}
	return *result;
}

} // namespace technicalmachine
