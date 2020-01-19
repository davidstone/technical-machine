// Test stat calculations
// Copyright (C) 2016 David Stone
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

#include <tm/test/stat.hpp>

#include <tm/test/incorrect_calculation.hpp>

#include <tm/team.hpp>
#include <tm/weather.hpp>

#include <tm/pokemon/max_pokemon_per_team.hpp>
#include <tm/pokemon/species.hpp>

#include <tm/stat/calculate.hpp>
#include <tm/stat/stat.hpp>

#include <containers/integer_range.hpp>

#include <iostream>
#include <string>

namespace technicalmachine {
namespace {
using namespace bounded::literal;

constexpr auto generation = Generation::four;

constexpr auto critical_hit = false;
constexpr auto physical_move = Moves::Tackle;

void attack_tests() {
	std::cout << "\tRunning Attack tests.\n";
	constexpr auto max_attack = 7368_bi;
	auto attacker = Team(max_pokemon_per_team);

	attacker.add_pokemon(generation, Species::Shuckle, Level(100_bi), Gender::male, Item::Choice_Band, Ability::Pure_Power, Nature::Impish);
	auto pokemon = attacker.pokemon();

	auto weather = Weather();

	pokemon.switch_in(generation, weather);

	set_stat_ev(pokemon, StatNames::DEF, EV(EV::max));
	pokemon.activate_power_trick();
	pokemon.stage()[StatNames::ATK] += 6_bi;

	check_equal(calculate_attack(generation, pokemon, Type::Normal, Ability::Honey_Gather, weather, critical_hit), max_attack);
}

void special_attack_tests() {
	std::cout << "\tRunning Special Attack tests.\n";
	constexpr auto max_special_attack = 4536_bi;
	auto weather = Weather();
	weather.activate_sun_from_move(false);
	auto attacker = Team(max_pokemon_per_team);

	attacker.add_pokemon(generation, Species::Deoxys_Attack, Level(100_bi), Gender::genderless, Item::Choice_Specs, Ability::Solar_Power, Nature::Modest);
	auto pokemon = attacker.pokemon();
	pokemon.switch_in(generation, weather);

	set_stat_ev(pokemon, StatNames::SPA, EV(EV::max));
	pokemon.stage()[StatNames::SPA] += 6_bi;

	check_equal(calculate_special_attack(generation, pokemon, Type::Water, Ability::Honey_Gather, weather, critical_hit), max_special_attack);
}

void max_defense_test() {
	std::cout << "\t\tRunning max Defense test.\n";
	constexpr auto max_defense = 3684_bi;

	auto defender = Team(max_pokemon_per_team);
	constexpr auto weather = Weather();

	defender.add_pokemon(generation, Species::Shuckle, Level(100_bi), Gender::male, Item::None, Ability::Marvel_Scale, Nature::Bold);
	auto pokemon = defender.pokemon();
	pokemon.switch_in(generation, weather);
	set_stat_ev(pokemon, StatNames::DEF, EV(EV::max));

	pokemon.stage()[StatNames::DEF] += 6_bi;

	apply_status_to_self(generation, Statuses::burn, pokemon, weather);

	check_equal(calculate_defense(generation, pokemon, physical_move, weather), max_defense);
}

void min_defense_test() {
	std::cout << "\t\tRunning min Defense test.\n";
	constexpr auto min_defense = 1_bi;

	auto weather = Weather();

	auto defender = Team(max_pokemon_per_team);

	defender.add_pokemon(generation, Species::Combee, Level(1_bi), Gender::male, Item::None, Ability::Honey_Gather, Nature::Hasty);
	auto pokemon = defender.pokemon();
	pokemon.switch_in(generation, weather);
	set_stat_ev(pokemon, StatNames::DEF, EV(0_bi));

	for (auto const n [[maybe_unused]] : containers::integer_range(3_bi)) {
		pokemon.stage()[StatNames::DEF] += -2_bi;
	}

	check_equal(calculate_defense(generation, pokemon, physical_move, weather), min_defense);
}

void defense_tests() {
	std::cout << "\tRunning Defense tests.\n";
	max_defense_test();
	min_defense_test();
}

void special_defense_tests() {
	std::cout << "\tRunning Special Defense tests.\n";
	constexpr auto max_special_defense = 3684_bi;

	auto defender = Team(max_pokemon_per_team);
	auto weather = Weather{};
	weather.activate_sand_from_move(false);

	defender.add_pokemon(generation, Species::Shuckle, Level(100_bi), Gender::male, Item::None, Ability::Honey_Gather, Nature::Calm);
	auto pokemon = defender.pokemon();
	pokemon.switch_in(generation, weather);
	set_stat_ev(pokemon, StatNames::SPD, EV(EV::max));

	pokemon.stage()[StatNames::SPD] += 6_bi;

	check_equal(calculate_special_defense(generation, pokemon, Ability::Honey_Gather, weather), max_special_defense);
}

void speed_tests() {
	std::cout << "\tRunning Speed tests.\n";
	constexpr auto max_speed = 12096_bi;
	auto weather = Weather();
	weather.activate_rain_from_move(false);

	auto team = Team(max_pokemon_per_team);

	team.add_pokemon(generation, Species::Deoxys_Speed, Level(100_bi), Gender::genderless, Item::Choice_Scarf, Ability::Swift_Swim, Nature::Timid);
	auto pokemon = team.pokemon();
	pokemon.switch_in(generation, weather);
	set_stat_ev(pokemon, StatNames::SPE, EV(EV::max));

	pokemon.stage()[StatNames::SPE] += 6_bi;

	team.screens.activate_tailwind();
	
	check_equal(calculate_speed(generation, team, Ability::Honey_Gather, weather), max_speed);
}

}	// namespace

void stat_tests() {
	std::cout << "Running stat tests.\n";
	attack_tests();
	special_attack_tests();
	defense_tests();
	special_defense_tests();
	speed_tests();
	std::cout << "Stat tests passed.\n\n";
}

}	// namespace technicalmachine
