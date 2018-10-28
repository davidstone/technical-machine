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
#include "../team_predictor/ev_optimizer/offensive.hpp"
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
	constexpr auto species = Species::Metagross;
	constexpr auto level = Level(100_bi);
	auto pokemon = Pokemon(team_size, species, level, Gender::genderless);
	set_hp_ev(pokemon, EV(252_bi));
	set_stat_ev(pokemon, StatNames::ATK, EV(96_bi));
	set_stat_ev(pokemon, StatNames::DEF, EV(96_bi));
	set_stat_ev(pokemon, StatNames::SPA, EV(0_bi));
	set_stat_ev(pokemon, StatNames::SPD, EV(4_bi));
	set_stat_ev(pokemon, StatNames::SPE, EV(60_bi));
	auto & nature = get_nature(pokemon);
	nature = Nature::Adamant;
	all_moves(pokemon).emplace_back(Moves::Meteor_Mash);

	auto const stats = pull_out_stats(pokemon);
	
	constexpr auto include_attack = true;
	constexpr auto include_special_attack = false;
	assert(minimize_evs(stats, species, level, include_attack, include_special_attack) == stats);
	assert(pad_random_evs(stats, include_attack, include_special_attack, random_engine) == stats);
	optimize_evs(pokemon, random_engine);
	assert(pull_out_stats(pokemon) == stats);
}

void defensive_tests() {
	std::cout << "\tRunning defensive tests.\n";
	constexpr auto species = Species::Celebi;
	constexpr auto level = Level(100_bi);
	constexpr auto nature = Nature::Bold;
	auto const hp = HP(species, level, EV(252_bi));
	auto const defense = Stat(species, StatNames::DEF, EV(252_bi));
	auto const special_defense = Stat(species, StatNames::SPD, EV(4_bi));
	
	auto defensive_evs = DefensiveEVs(species, level, nature, hp, defense, special_defense);
	for (auto const & candidate : defensive_evs) {
		assert(candidate.hp == hp.ev());
		assert(candidate.defense == defense.ev());
		assert(candidate.special_defense >= special_defense.ev());
		assert(boosts_stat(candidate.nature, StatNames::DEF));
	}
}

auto find(SpeedEVs const & container, Nature const nature) {
	auto const it = containers::find_if(container, [=](auto const & value) { return value.nature == nature; });
	assert(it != end(container));
	return it->ev;
}

void speed_tests() {
	std::cout << "\tRunning speed tests.\n";
	
	constexpr auto species = Species::Snorlax;
	constexpr auto level = Level(100_bi);
	constexpr auto original_nature = Nature::Hardy;
	auto const original_stat = Stat(species, StatNames::SPE, EV(76_bi));
	auto const speed_evs = SpeedEVs(original_nature, original_stat, level);
	auto const original_value = initial_stat(StatNames::SPE, original_stat, level, original_nature);
	for (auto const nature : containers::enum_range<Nature>()) {
		auto const new_value = initial_stat(StatNames::SPE, Stat(species, StatNames::SPE, find(speed_evs, nature)), level, nature);
		if (boosts_stat(nature, StatNames::SPE) and !boosts_stat(original_nature, StatNames::SPE)) {
			assert(new_value == original_value or new_value == original_value + 1_bi);
		} else {
			assert(new_value == original_value);
		}
	}
}

void not_level_100(std::mt19937 & random_engine) {
	constexpr auto team_size = 1_bi;
	constexpr auto species = Species::Masquerain;
	constexpr auto level = Level(83_bi);
	auto pokemon = Pokemon(team_size, species, level, Gender::genderless);
	set_hp_ev(pokemon, EV(192_bi));
	set_stat_ev(pokemon, StatNames::ATK, EV(0_bi));
	set_stat_ev(pokemon, StatNames::DEF, EV(8_bi));
	set_stat_ev(pokemon, StatNames::SPA, EV(120_bi));
	set_stat_ev(pokemon, StatNames::SPD, EV(60_bi));
	set_stat_ev(pokemon, StatNames::SPE, EV(128_bi));
	auto & nature = get_nature(pokemon);
	nature = Nature::Modest;
	for (auto const move : {Moves::Hydro_Pump, Moves::Bug_Buzz, Moves::Roost, Moves::Air_Slash}) {
		all_moves(pokemon).emplace_back(move);
	}

	optimize_evs(pokemon, random_engine);
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

	std::cout << "EV optimizer tests passed.\n\n";
}

}	// namespace technicalmachine
