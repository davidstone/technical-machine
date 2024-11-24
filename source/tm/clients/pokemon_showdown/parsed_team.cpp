// Copyright David Stone 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.clients.ps.parsed_team;

import tm.clients.ps.parsed_stats;

import tm.move.max_moves_per_pokemon;
import tm.move.move_name;
import tm.move.move_names;

import tm.pokemon.happiness;
import tm.pokemon.level;
import tm.pokemon.max_pokemon_per_team;
import tm.pokemon.species;

import tm.status.status_name;

import tm.type.type;

import tm.ability;
import tm.gender;
import tm.item;

import bounded;
import containers;
import tv;

namespace technicalmachine::ps {
using namespace bounded::literal;

export struct ParsedPokemon {
	Species species;
	Level level = Level(100_bi);
	Gender gender = Gender::genderless;
	StatusName status = StatusName::clear;
	Item item = Item::None;
	Ability ability = Ability::Honey_Gather;
	ParsedStats stats;
	MoveNames moves;
	tv::optional<Type> hidden_power_type = tv::none;
	Happiness happiness = Happiness();
	friend auto operator==(ParsedPokemon, ParsedPokemon) -> bool = default;
};

} // namespace techniclalmachine::ps

template<>
struct bounded::tombstone_traits<technicalmachine::ps::ParsedPokemon> {
private:
	using base = tombstone_traits<technicalmachine::ps::ParsedStats>;
public:
	static constexpr auto spare_representations = base::spare_representations;

	static constexpr auto make(auto const index) noexcept -> technicalmachine::ps::ParsedPokemon {
		return technicalmachine::ps::ParsedPokemon{
			.species = {},
			.stats = base::make(index),
			.moves = {}
		};
	}
	static constexpr auto index(technicalmachine::ps::ParsedPokemon const & value) noexcept {
		return base::index(value.stats);
	}
};

namespace technicalmachine::ps {

export using ParsedTeam = containers::static_vector<ParsedPokemon, max_pokemon_per_team>;

} // namespace technicalmachine::ps
