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

#include "ev_optimizer.hpp"

#include "../team.hpp"
#include "../pokemon/species.hpp"
#include "../stat/calculate.hpp"
#include "../team_predictor/ev_optimizer/defensive.hpp"
#include "../team_predictor/ev_optimizer/ev_optimizer.hpp"
#include "../team_predictor/ev_optimizer/speed.hpp"
#include "../move/moves.hpp"
#include "../string_conversions/nature.hpp"

#include <iostream>
#include <random>

#undef NDEBUG

#include <cassert>

namespace technicalmachine {
namespace {

void optimize_already_optimized(std::mt19937 & random_engine) {
	constexpr auto team_size = 1_bi;
	auto pokemon = Pokemon(team_size, Species::Metagross, Level(100_bi), Gender::genderless);
	set_hp_ev(pokemon, EV(252_bi));
	set_stat_ev(pokemon, StatNames::ATK, EV(96_bi));
	set_stat_ev(pokemon, StatNames::DEF, EV(96_bi));
	set_stat_ev(pokemon, StatNames::SPA, EV(0_bi));
	set_stat_ev(pokemon, StatNames::SPD, EV(4_bi));
	set_stat_ev(pokemon, StatNames::SPE, EV(60_bi));
	auto & nature = get_nature(pokemon);
	nature = Nature::Adamant;
	all_moves(pokemon).emplace_back(Moves::Meteor_Mash);

	auto assert_evs_unchanged = [original = pokemon](Pokemon const & test) {
		assert(get_hp(original).ev() == get_hp(test).ev());
		for (auto const stat : regular_stats()) {
			assert(get_stat(original, stat).ev() == get_stat(test, stat).ev());
		}
	};
	
	minimize_evs(pokemon);
	assert_evs_unchanged(pokemon);
	optimize_evs(pokemon, random_engine);
	assert_evs_unchanged(pokemon);
	pad_random_evs(pokemon, random_engine);
	assert_evs_unchanged(pokemon);
}

void defensive_tests() {
	std::cout << "\tRunning defensive tests.\n";
	constexpr auto team_size = max_pokemon_per_team;
	Pokemon pokemon(team_size, Species::Celebi, Level(100_bi), Gender::genderless);
	set_hp_ev(pokemon, EV(252_bi));
	set_stat_ev(pokemon, StatNames::DEF, EV(252_bi));
	set_stat_ev(pokemon, StatNames::SPD, EV(4_bi));
	auto & nature = get_nature(pokemon);
	nature = Nature::Bold;
	all_moves(pokemon).emplace_back(Moves::Psychic);
	
	for (auto const & candidate : DefensiveEVs(pokemon)) {
		assert(candidate.hp == get_hp(pokemon).ev());
		assert(candidate.defense == get_stat(pokemon, StatNames::DEF).ev());
		assert(candidate.special_defense >= get_stat(pokemon, StatNames::SPD).ev());
		assert(boosts_stat(candidate.nature, StatNames::DEF));
	}
}

Pokemon make_test_pokemon() {
	constexpr auto team_size = max_pokemon_per_team;
	Level const level(100_bi);
	Pokemon pokemon(team_size, Species::Snorlax, level, Gender::male);
	set_hp_ev(pokemon, EV(128_bi));
	for (auto const stat : containers::enum_range(StatNames::NORMAL_END)) {
		set_stat_ev(pokemon, stat, EV(76_bi));
	}
	get_nature(pokemon) = Nature::Hardy;
	all_moves(pokemon).emplace_back(Moves::Psychic);
	all_moves(pokemon).emplace_back(Moves::Earthquake);
	return pokemon;
}

auto find(SpeedEVs const & container, Nature const nature) {
	auto const it = containers::find_if(container, [=](auto const & value) { return value.nature == nature; });
	assert(it != end(container));
	return it->ev;
}

void speed_tests() {
	std::cout << "\tRunning speed tests.\n";
	
	auto const pokemon = make_test_pokemon();
	auto const species = static_cast<Species>(pokemon);
	auto const level = get_level(pokemon);
	auto const speed_evs = SpeedEVs(get_nature(pokemon), get_stat(pokemon, StatNames::SPE), get_level(pokemon));
	auto const original_stat = get_stat(pokemon, StatNames::SPE);
	auto const original_value = initial_stat(StatNames::SPE, original_stat, level, get_nature(pokemon));
	for (auto const nature : containers::enum_range<Nature>()) {
		auto const new_value = initial_stat(StatNames::SPE, Stat(species, StatNames::SPE, find(speed_evs, nature)), level, nature);
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
	optimize_already_optimized(random_engine);

	std::cout << "EV optimizer tests passed.\n\n";
}

}	// namespace technicalmachine
