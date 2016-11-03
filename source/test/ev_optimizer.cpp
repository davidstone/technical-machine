// Test EV optimizer
// Copyright (C) 2015 David Stone
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

#include "ev_optimizer.hpp"
#include <iostream>
#include <random>
#include "../enum_range.hpp"
#include "../team.hpp"
#include "../pokemon/species.hpp"
#include "../stat/calculate.hpp"
#include "../team_predictor/ev_optimizer/ev_optimizer.hpp"
#include "../team_predictor/ev_optimizer/single_classification_evs.hpp"
#include "../team_predictor/ev_optimizer/speed.hpp"
#include "../move/moves.hpp"
#include "../string_conversions/nature.hpp"

namespace technicalmachine {
namespace {

void defensive_tests() {
	std::cout << "\tRunning defensive tests.\n";
	constexpr auto team_size = max_pokemon_per_team;
	Pokemon pokemon(team_size, Species::Celebi, Level(100_bi), Gender::GENDERLESS);
	set_hp_ev(pokemon, EV(252_bi));
	set_stat_ev(pokemon, StatNames::DEF, EV(252_bi));
	set_stat_ev(pokemon, StatNames::SPD, EV(4_bi));
	auto & nature = get_nature(pokemon);
	nature = Nature::Bold;
	all_moves(pokemon).add(Moves::Psychic);
	
	constexpr auto physical = true;
	for (auto const candidate : equal_defensiveness<physical>(pokemon)) {
		assert(candidate.hp().value() == get_hp(pokemon).ev().value());
		assert(candidate.defensive().value() == get_stat(pokemon, StatNames::DEF).ev().value());
		assert(boosts_stat(candidate.nature(, StatNames::DEF)));
	}
}

Pokemon make_test_pokemon() {
	constexpr auto team_size = max_pokemon_per_team;
	Level const level(100_bi);
	Gender const gender(Gender::MALE);
	Pokemon pokemon(team_size, Species::Snorlax, level, gender);
	set_hp_ev(pokemon, EV(128_bi));
	for (auto const stat : enum_range<StatNames, StatNames::NORMAL_END>) {
		set_stat_ev(pokemon, stat, EV(76_bi));
	}
	get_nature(pokemon) = Nature::Hardy;
	all_moves(pokemon).add(Moves::Psychic);
	all_moves(pokemon).add(Moves::Earthquake);
	return pokemon;
}

void speed_tests() {
	std::cout << "\tRunning speed tests.\n";
	
	auto const pokemon = make_test_pokemon();
	auto const species = static_cast<Species>(pokemon);
	auto const level = get_level(pokemon);
	SpeedEVs speedEVs(pokemon);
	auto const original_stat = get_stat(pokemon, StatNames::SPE);
	auto const original_value = initial_stat<StatNames::SPE>(original_stat, level, get_nature(pokemon));
	for (auto const nature : enum_range<Nature>) {
		auto const new_value = initial_stat<StatNames::SPE>(Stat(species, StatNames::SPE, find(speedEVs, nature)), level, nature);
		if (boosts_stat(nature, StatNames::SPE) and !boosts_stat(get_nature(pokemon), StatNames::SPE)) {
			assert(new_value == original_value or new_value == original_value + 1_bi);
		} else {
			assert(new_value == original_value);
		}
	}
}

}	// namespace

void ev_optimizer_tests() {
	std::cout << "Running EV optimizer tests.\n";
	
	defensive_tests();
	speed_tests();

	auto pokemon = make_test_pokemon();
	minimize_evs(pokemon);
	std::random_device rd;
	std::mt19937 random_engine(rd());
	pad_random_evs(pokemon, random_engine);
	std::cout << "EV optimizer tests passed.\n\n";
}

}	// namespace technicalmachine
