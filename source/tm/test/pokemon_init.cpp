// Copyright David Stone 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.test.pokemon_init;

import tm.move.max_moves_per_pokemon;
import tm.move.move;
import tm.move.move_name;
import tm.move.move_names;
import tm.move.regular_moves;

import tm.pokemon.known_pokemon;
import tm.pokemon.level;
import tm.pokemon.nickname;
import tm.pokemon.pokemon;
import tm.pokemon.seen_pokemon;
import tm.pokemon.species;

import tm.stat.combined_stats;
import tm.stat.default_evs;
import tm.stat.iv;
import tm.stat.nature;

import tm.string_conversions.species;

import tm.ability;
import tm.gender;
import tm.generation;
import tm.item;
import tm.team;

import bounded;
import containers;
import std_module;

namespace technicalmachine {
using namespace bounded::literal;

export template<Generation generation>
struct PokemonInit {
	Species species;
	Nickname nickname = to_string(species);
	Level level = Level(100_bi);
	Gender gender = Gender::genderless;
	Item item = Item::None;
	Ability ability = Ability::Honey_Gather;
	Nature nature = Nature::Hardy;
	decltype(max_dvs_or_ivs<generation>) ivs = max_dvs_or_ivs<generation>;
	decltype(default_evs<generation>) evs = default_evs<generation>;
	MoveNames moves = {{MoveName::Tackle}};
};

export struct SeenPokemonInit {
	Species species;
	Nickname nickname = to_string(species);
	Level level = Level(100_bi);
	Gender gender = Gender::genderless;
};

export template<Generation generation>
constexpr auto make_pokemon = [](PokemonInit<generation> const pokemon) {
	return Pokemon<generation>(
		pokemon.species,
		pokemon.level,
		pokemon.gender,
		pokemon.item,
		pokemon.ability,
		CombinedStatsFor<generation>(
			pokemon.nature,
			pokemon.ivs,
			pokemon.evs
		),
		RegularMoves(containers::transform(
			pokemon.moves,
			[](MoveName const move) {
				return Move(generation, move);
			}
		))
	);
};

export template<Generation generation>
constexpr auto make_known_pokemon = [](PokemonInit<generation> const pokemon) {
	return KnownPokemon<generation>(
		pokemon.species,
		pokemon.nickname,
		pokemon.level,
		pokemon.gender,
		pokemon.item,
		pokemon.ability,
		CombinedStatsFor<generation>(
			pokemon.nature,
			pokemon.ivs,
			pokemon.evs
		),
		RegularMoves(containers::transform(
			pokemon.moves,
			[](MoveName const move) {
				return Move(generation, move);
			}
		))
	);
};

export template<Generation generation>
constexpr auto make_seen_pokemon(SeenPokemonInit const pokemon) {
	return SeenPokemon<generation>(
		pokemon.species,
		pokemon.nickname,
		pokemon.level,
		pokemon.gender
	);
}

export template<Generation generation, std::size_t size>
constexpr auto make_team(containers::c_array<PokemonInit<generation>, size> const & init) {
	return Team<generation>(containers::transform(
		std::span(init),
		make_pokemon<generation>
	));
}

export template<Generation generation, std::size_t size>
constexpr auto make_known_team(containers::c_array<PokemonInit<generation>, size> const & init) {
	return KnownTeam<generation>(containers::transform(
		std::span(init),
		make_known_pokemon<generation>
	));
}

export template<Generation generation, std::size_t size>
constexpr auto make_seen_team(containers::c_array<SeenPokemonInit, size> const & init) {
	auto team = SeenTeam<generation>(bounded::constant<size>);
	for (SeenPokemonInit const pokemon : init) {
		team.add_pokemon(make_seen_pokemon<generation>(pokemon));
	}
	return team;
}

} // namespace technicalmachine
