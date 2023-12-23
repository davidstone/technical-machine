// Copyright David Stone 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.clients.ps.parsed_team;

import tm.clients.party;

import tm.move.max_moves_per_pokemon;
import tm.move.move_name;
import tm.move.move_names;

import tm.pokemon.happiness;
import tm.pokemon.level;
import tm.pokemon.max_pokemon_per_team;
import tm.pokemon.nickname;
import tm.pokemon.species;

import tm.stat.initial_stat;
import tm.stat.stat_style;

import tm.status.status_name;

import tm.string_conversions.species;

import tm.type.type;

import tm.ability;
import tm.gender;
import tm.item;
import tm.visible_hp;

import bounded;
import containers;
import tv;

namespace technicalmachine::ps {
using namespace bounded::literal;

// PS always sends split-special stats
export using ParsedStat = InitialStat<StatStyle::current>;

export struct ParsedStats {
	VisibleHP hp;
	ParsedStat atk;
	ParsedStat def;
	ParsedStat spa;
	ParsedStat spd;
	ParsedStat spe;
	friend auto operator==(ParsedStats, ParsedStats) -> bool = default;
};

export struct ParsedPokemon {
	Species species;
	Nickname nickname = to_string(species);
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

export using ParsedTeam = containers::static_vector<ParsedPokemon, max_pokemon_per_team>;

export struct ParsedSide {
	Party party;
	ParsedTeam team;
	friend auto operator==(ParsedSide, ParsedSide) -> bool = default;
};

} // namespace technicalmachine::ps
