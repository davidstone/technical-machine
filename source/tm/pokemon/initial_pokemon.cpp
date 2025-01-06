// Copyright David Stone 2024.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.pokemon.initial_pokemon;

import tm.move.initial_move;

import tm.pokemon.happiness;
import tm.pokemon.level;
import tm.pokemon.nickname;
import tm.pokemon.species;

import tm.stat.combined_stats;
import tm.stat.stat_style;

import tm.string_conversions.species;

import tm.ability;
import tm.gender;
import tm.item;

import bounded;
import std_module;

using namespace bounded::literal;
using namespace std::string_view_literals;

namespace technicalmachine {

export template<SpecialInputStyle style>
struct InitialPokemon {
	Species species;
	Nickname nickname = to_string(species);
	Level level = Level(100_bi);
	Gender gender = Gender::genderless;
	Item item = Item::None;
	Ability ability = Ability::Honey_Gather;
	CombinedStats<style> stats = {};
	InitialMoves moves;
	Happiness happiness = Happiness();
	friend auto operator==(InitialPokemon, InitialPokemon) -> bool = default;
};

} // namespace technicalmachine

template<technicalmachine::SpecialInputStyle style>
struct bounded::tombstone<technicalmachine::InitialPokemon<style>> {
	using base = tombstone_traits<technicalmachine::InitialMoves>;

	static constexpr auto make(auto const index) noexcept -> technicalmachine::InitialPokemon<style> {
		return {
			.species = {},
			.nickname = ""sv,
			.moves = base::make(index),
		};
	}

	static constexpr auto index(technicalmachine::InitialPokemon<style> const & value) noexcept {
		return base::index(value.moves);
	}
};
