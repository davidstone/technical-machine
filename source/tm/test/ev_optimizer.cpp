// Test EV optimizer
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

#include <tm/test/ev_optimizer.hpp>

#include <tm/move/moves.hpp>
#include <tm/pokemon/species.hpp>
#include <tm/stat/calculate.hpp>
#include <tm/stat/stat_to_ev.hpp>
#include <tm/string_conversions/nature.hpp>
#include <tm/team_predictor/ev_optimizer/defensive.hpp>
#include <tm/team_predictor/ev_optimizer/ev_optimizer.hpp>
#include <tm/team_predictor/ev_optimizer/offensive.hpp>
#include <tm/team_predictor/ev_optimizer/speed.hpp>
#include <tm/team.hpp>

#include <bounded/assert.hpp>

#include <containers/integer_range.hpp>

#include <iostream>
#include <random>

namespace technicalmachine {
namespace {

void optimize_already_optimized(std::mt19937 & random_engine) {
	constexpr auto generation = Generation::four;
	constexpr auto team_size = 1_bi;
	constexpr auto species = Species::Metagross;
	constexpr auto level = Level(100_bi);
	auto pokemon = Pokemon(generation, team_size, species, level, Gender::genderless, Item::None, Ability::Honey_Gather, Nature::Adamant);
	constexpr auto iv = IV(31_bi);
	set_hp_ev(generation, pokemon, iv, EV(252_bi));
	set_stat_ev(pokemon, StatNames::ATK, iv, EV(96_bi));
	set_stat_ev(pokemon, StatNames::DEF, iv, EV(96_bi));
	set_stat_ev(pokemon, StatNames::SPA, iv, EV(0_bi));
	set_stat_ev(pokemon, StatNames::SPD, iv, EV(4_bi));
	set_stat_ev(pokemon, StatNames::SPE, iv, EV(60_bi));
	all_moves(pokemon).add(Move(generation, Moves::Meteor_Mash));

	auto const stats = calculate_evs(generation, pokemon);
	
	constexpr auto include_attack = true;
	constexpr auto include_special_attack = false;
	BOUNDED_ASSERT(minimize_evs(generation, stats, species, level, include_attack, include_special_attack) == stats);
	BOUNDED_ASSERT(pad_random_evs(generation, stats, include_attack, include_special_attack, random_engine) == stats);
	optimize_evs(generation, pokemon, random_engine);
	BOUNDED_ASSERT(calculate_evs(generation, pokemon) == stats);
}

void defensive_tests() {
	std::cout << "\tRunning defensive tests.\n";

	constexpr auto generation = Generation::four;
	constexpr auto species = Species::Celebi;
	constexpr auto level = Level(100_bi);
	constexpr auto iv = IV(31_bi);
	constexpr auto hp_ev = EV(252_bi);
	constexpr auto defense_ev = EV(252_bi);
	constexpr auto special_defense_ev = EV(4_bi);
	auto const hp = HP(generation, species, level, iv, hp_ev);
	auto const defense = Stat(generation, species, StatNames::DEF, iv, defense_ev);
	auto const special_defense = Stat(generation, species, StatNames::SPD, iv, special_defense_ev);
	
	auto defensive_evs = DefensiveEVs(generation, species, level, Nature::Bold, hp, defense, special_defense);
	for (auto const & candidate : defensive_evs) {
		BOUNDED_ASSERT(candidate.hp == hp_ev);
		BOUNDED_ASSERT(candidate.defense == defense_ev);
		BOUNDED_ASSERT(candidate.special_defense >= special_defense_ev);
		BOUNDED_ASSERT(boosts_stat(candidate.nature, StatNames::DEF));
	}
}

auto find(SpeedEVs const & container, Nature const nature) {
	auto const it = containers::find_if(container, [=](auto const & value) { return value.nature == nature; });
	BOUNDED_ASSERT(it != end(container));
	return it->stat.ev;
}

void speed_tests() {
	std::cout << "\tRunning speed tests.\n";
	
	constexpr auto generation = Generation::four;
	constexpr auto species = Species::Snorlax;
	constexpr auto level = Level(100_bi);
	constexpr auto original_nature = Nature::Hardy;
	constexpr auto iv = IV(31_bi);
	auto const original_stat = Stat(generation, species, StatNames::SPE, iv, EV(76_bi));
	auto const speed_evs = SpeedEVs(original_nature, original_stat, level);
	auto const original_value = initial_stat(StatNames::SPE, original_stat, level, original_nature);
	for (auto const nature : containers::enum_range<Nature>()) {
		auto const new_value = initial_stat(StatNames::SPE, Stat(generation, species, StatNames::SPE, iv, find(speed_evs, nature)), level, nature);
		if (boosts_stat(nature, StatNames::SPE) and !boosts_stat(original_nature, StatNames::SPE)) {
			BOUNDED_ASSERT(new_value == original_value or new_value == original_value + 1_bi);
		} else {
			BOUNDED_ASSERT(new_value == original_value);
		}
	}
}

void not_level_100(std::mt19937 & random_engine) {
	constexpr auto generation = Generation::four;
	constexpr auto team_size = 1_bi;
	constexpr auto species = Species::Masquerain;
	constexpr auto level = Level(83_bi);
	auto pokemon = Pokemon(generation, team_size, species, level, Gender::genderless, Item::None, Ability::Honey_Gather, Nature::Modest);
	set_hp_ev(generation, pokemon, IV(31_bi), EV(192_bi));
	set_stat_ev(pokemon, StatNames::ATK, IV(0_bi), EV(0_bi));
	set_stat_ev(pokemon, StatNames::DEF, IV(31_bi), EV(8_bi));
	set_stat_ev(pokemon, StatNames::SPA, IV(31_bi), EV(120_bi));
	set_stat_ev(pokemon, StatNames::SPD, IV(31_bi), EV(60_bi));
	set_stat_ev(pokemon, StatNames::SPE, IV(31_bi), EV(128_bi));
	for (auto const move : {Moves::Hydro_Pump, Moves::Bug_Buzz, Moves::Roost, Moves::Air_Slash}) {
		all_moves(pokemon).add(Move(generation, move));
	}

	optimize_evs(generation, pokemon, random_engine);
}

void generation_two(std::mt19937 & random_engine) {
	constexpr auto generation = Generation::two;
	constexpr auto team_size = 1_bi;
	constexpr auto species = Species::Mew;
	constexpr auto level = Level(100_bi);
	auto pokemon = Pokemon(generation, team_size, species, level, Gender::genderless, Item::None, Ability::Honey_Gather, Nature::Hardy);
	set_hp_ev(generation, pokemon, IV(30_bi), EV(252_bi));
	set_stat_ev(pokemon, StatNames::ATK, IV(30_bi), EV(252_bi));
	set_stat_ev(pokemon, StatNames::DEF, IV(30_bi), EV(252_bi));
	set_stat_ev(pokemon, StatNames::SPA, IV(30_bi), EV(252_bi));
	set_stat_ev(pokemon, StatNames::SPD, IV(30_bi), EV(252_bi));
	set_stat_ev(pokemon, StatNames::SPE, IV(30_bi), EV(252_bi));
	all_moves(pokemon).add(Move(generation, Moves::Tackle));
	all_moves(pokemon).add(Move(generation, Moves::Psychic));

	auto const stats = calculate_evs(generation, pokemon);
	
	constexpr auto include_attack = true;
	constexpr auto include_special_attack = true;
	BOUNDED_ASSERT(minimize_evs(generation, stats, species, level, include_attack, include_special_attack) == stats);
	BOUNDED_ASSERT(pad_random_evs(generation, stats, include_attack, include_special_attack, random_engine) == stats);
	optimize_evs(generation, pokemon, random_engine);
	BOUNDED_ASSERT(calculate_evs(generation, pokemon) == stats);
}

}	// namespace

void ev_optimizer_tests() {
	std::cout << "Running EV optimizer tests.\n";
	
	defensive_tests();
	speed_tests();

	std::random_device rd;
	std::mt19937 random_engine(rd());
	optimize_already_optimized(random_engine);

	not_level_100(random_engine);

	generation_two(random_engine);

	std::cout << "EV optimizer tests passed.\n\n";
}

}	// namespace technicalmachine
