// Copyright David Stone 2022.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.test.usage_bytes;

import tm.move.move_name;

import tm.pokemon.species;

import tm.ps_usage_stats.header;

import tm.team_predictor.usage_stats;

import tm.ability;
import tm.generation;
import tm.item;

import bounded;
import containers;
import std_module;

namespace technicalmachine {
using namespace bounded::literal;

constexpr auto cast_to_bytes(auto const value, auto const bytes) {
	return std::bit_cast<containers::array<std::byte, bytes>>(value);
}

export constexpr auto version_bytes(std::uint16_t const version) {
	return cast_to_bytes(version, 2_bi);
}

export constexpr auto generation_bytes(Generation const generation) {
	return cast_to_bytes(generation, 1_bi);
}

export constexpr auto count_bytes(std::uint16_t const count) {
	return cast_to_bytes(count, 2_bi);
}

export constexpr auto weight_bytes(double const weight) {
	return cast_to_bytes(weight, 8_bi);
}

export constexpr auto species_bytes(Species const species) {
	return cast_to_bytes(species, 2_bi);
}

export constexpr auto move_bytes(MoveName const move_name) {
	return cast_to_bytes(move_name, 2_bi);
}

constexpr auto item_bytes(Item const item) {
	return cast_to_bytes(item, 2_bi);
}

constexpr auto ability_bytes(Ability const ability) {
	return cast_to_bytes(ability, 2_bi);
}

export constexpr auto speed_bytes(std::uint16_t const value) {
	return cast_to_bytes(value, 2_bi);
}

constexpr auto no_items(Generation const generation) {
	return generation >= Generation::two ?
		containers::concatenate<containers::vector<std::byte>>(
			count_bytes(1),
			item_bytes(Item::None),
			weight_bytes(1.0)
		) :
		containers::vector<std::byte>();
}

constexpr auto maybe_ability(Generation const generation, Ability const ability) {
	return generation >= Generation::three ?
		containers::concatenate<containers::vector<std::byte>>(
			count_bytes(1),
			ability_bytes(ability),
			weight_bytes(1.0)
		) :
		containers::vector<std::byte>();
}

// This is the smallest file generated by my generator. It's possible to make a
// smaller legal file.
export constexpr auto smallest_team_bytes(Generation const generation) {
	return containers::concatenate<containers::vector<std::byte>>(
		usage_stats_magic_string,
		version_bytes(0),
		generation_bytes(generation),
		count_bytes(1),
			species_bytes(Species::Mew),
				weight_bytes(1.0),
				count_bytes(1),
					speed_bytes(7),
						weight_bytes(1.0),
				count_bytes(0), // Teammates
				count_bytes(1),
					move_bytes(MoveName::Cut),
						weight_bytes(1.0),
						count_bytes(1),
							speed_bytes(7),
								weight_bytes(1.0),
						count_bytes(0), // Teammates
						count_bytes(0), // Other moves
						no_items(generation),
						maybe_ability(generation, Ability::Synchronize),
				no_items(generation),
				maybe_ability(generation, Ability::Synchronize)
	);
}

export auto bytes_to_usage_stats(std::span<std::byte const> const bytes) -> UsageStats {
	return UsageStats::make(std::stringstream(std::string(
		reinterpret_cast<char const *>(containers::data(bytes)),
		static_cast<std::size_t>(containers::size(bytes))
	)));
}

} // namespace technicalmachine