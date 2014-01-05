// Test stat calculations
// Copyright (C) 2013 David Stone
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

#include "stat.hpp"

#include <iostream>
#include <string>

#include "incorrect_calculation.hpp"

#include "../team.hpp"
#include "../weather.hpp"

#include "../pokemon/species.hpp"

#include "../stat/stat.hpp"

namespace technicalmachine {
namespace {
using namespace bounded_integer::literal;

constexpr unsigned team_size = 6;

void attack_tests () {
	std::cout << "\tRunning Attack tests.\n";
	constexpr auto max_attack = 7368_bi;
	Team attacker;

	Level const level(100_bi);
	Gender const gender(Gender::MALE);
	attacker.add_pokemon(Species::Shuckle, level, gender);
	Pokemon & pokemon = attacker.pokemon();

	get_stat(pokemon, StatNames::DEF).ev = EV(bounded_integer::make_bounded<EV::max>());
	get_nature(pokemon).name = Nature::IMPISH;
	attacker.pokemon().activate_power_trick();
	get_ability(pokemon) = Ability::Pure_Power;
	attacker.pokemon().stat_boost(StatNames::ATK, 6_bi);

	get_item(pokemon).name = Item::CHOICE_BAND;
	
	check_equal(calculate_attack(attacker.pokemon(), Weather{}), max_attack);
}

void special_attack_tests () {
	std::cout << "\tRunning Special Attack tests.\n";
	constexpr auto max_special_attack = 4536_bi;
	Weather weather;
	weather.set_sun (Weather::Duration::permanent);
	Team attacker;

	Level const level(100_bi);
	Gender const gender(Gender::GENDERLESS);
	attacker.add_pokemon(Species::Deoxys_Attack, level, gender);
	Pokemon & pokemon = attacker.pokemon();

	get_stat(pokemon, StatNames::SPA).ev = EV(bounded_integer::make_bounded<EV::max>());
	get_nature(pokemon).name = Nature::MODEST;
	attacker.pokemon().stat_boost(StatNames::SPA, 6_bi);

	get_ability(pokemon) = Ability::Solar_Power;

	get_item(pokemon).name = Item::CHOICE_SPECS;
	
	check_equal(calculate_special_attack(attacker.pokemon(), weather), max_special_attack);
}

void max_defense_test() {
	std::cout << "\t\tRunning max Defense test.\n";
	constexpr auto max_defense = 3684_bi;

	Team defender;
	Weather weather;

	Level const level(100_bi);
	Gender const gender(Gender::MALE);
	defender.add_pokemon(Species::Shuckle, level, gender);
	Pokemon & pokemon = defender.pokemon();
	get_stat(pokemon, StatNames::DEF).ev = EV(bounded_integer::make_bounded<EV::max>());
	get_nature(pokemon).name = Nature::BOLD;

	defender.pokemon().stat_boost(StatNames::DEF, 6_bi);

	get_ability(pokemon) = Ability::Marvel_Scale;
	Status::apply<Status::BURN>(pokemon, weather);

	check_equal(calculate_defense(defender.pokemon(), weather), max_defense);
}

void min_defense_test() {
	std::cout << "\t\tRunning min Defense test.\n";
	constexpr auto min_defense = 1_bi;

	Team defender;

	Level const level(1_bi);
	Gender const gender(Gender::MALE);
	defender.add_pokemon(Species::Combee, level, gender);
	auto & pokemon = defender.pokemon();
	get_stat(pokemon, StatNames::DEF).ev = EV(0_bi);
	get_nature(pokemon) = Nature::HASTY;

	for (unsigned n = 0; n != 3; ++n) {
		pokemon.stat_boost(StatNames::DEF, -2_bi);
	}

	check_equal(calculate_defense(defender.pokemon(), Weather{}), min_defense);
}

void defense_tests () {
	std::cout << "\tRunning Defense tests.\n";
	max_defense_test();
	min_defense_test();
}

void special_defense_tests () {
	std::cout << "\tRunning Special Defense tests.\n";
	constexpr auto max_special_defense = 3684_bi;

	Team defender;
	Weather weather;
	weather.set_sand (Weather::Duration::permanent);

	Level const level(100_bi);
	Gender const gender(Gender::MALE);
	defender.add_pokemon(Species::Shuckle, level, gender);
	Pokemon & pokemon = defender.pokemon();
	get_stat(pokemon, StatNames::SPD).ev = EV(bounded_integer::make_bounded<EV::max>());
	get_nature(pokemon).name = Nature::CALM;

	defender.pokemon().stat_boost(StatNames::SPD, 6_bi);

	check_equal(calculate_special_defense(defender.pokemon(), weather), max_special_defense);
}

void speed_tests () {
	std::cout << "\tRunning Speed tests.\n";
	constexpr auto max_speed = 12096_bi;
	Weather weather;
	weather.set_rain (Weather::Duration::permanent);

	Team team;

	Level const level(100_bi);
	Gender const gender(Gender::GENDERLESS);
	team.add_pokemon(Species::Deoxys_Speed, level, gender);
	Pokemon & pokemon = team.pokemon();
	get_stat(pokemon, StatNames::SPE).ev = EV(bounded_integer::make_bounded<EV::max>());
	get_nature(pokemon).name = Nature::TIMID;

	team.pokemon().stat_boost(StatNames::SPE, 6_bi);

	get_ability(pokemon) = Ability::Swift_Swim;

	get_item(pokemon).name = Item::CHOICE_SCARF;
	
	team.screens.activate_tailwind();
	
	check_equal(calculate_speed(team, weather), max_speed);
}

}	// unnamed namespace

void stat_tests () {
	std::cout << "Running stat tests.\n";
	attack_tests ();
	special_attack_tests ();
	defense_tests ();
	special_defense_tests ();
	speed_tests ();
	std::cout << "Stat tests passed.\n\n";
}

}	// namespace technicalmachine
