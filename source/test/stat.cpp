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

constexpr unsigned team_size = 6;

void attack_tests () {
	std::cout << "\tRunning Attack tests.\n";
	constexpr unsigned max_attack = 7368;
	Team attacker;

	Level const level(100_ri);
	Gender const gender(Gender::MALE);
	attacker.add_pokemon(Species::Shuckle, level, gender);
	Pokemon & pokemon = attacker.pokemon();

	get_stat(pokemon, Stat::DEF).ev.set_value(252);
	get_nature(pokemon).name = Nature::IMPISH;
	attacker.pokemon().activate_power_trick();
	get_ability(pokemon) = Ability::Pure_Power;
	attacker.pokemon().stat_boost(Stat::ATK, 6);

	get_item(pokemon).name = Item::CHOICE_BAND;
	
	calculate_attack(attacker.pokemon(), Weather{});
	if (get_stat(attacker.pokemon(), Stat::ATK).stat != max_attack)
		throw IncorrectCalculation (get_stat(attacker.pokemon(), Stat::ATK).stat, max_attack);
}

void special_attack_tests () {
	std::cout << "\tRunning Special Attack tests.\n";
	constexpr unsigned max_special_attack = 4536;
	Weather weather;
	weather.set_sun (Weather::Duration::permanent);
	Team attacker;

	Level const level(100_ri);
	Gender const gender(Gender::GENDERLESS);
	attacker.add_pokemon(Species::Deoxys_Attack, level, gender);
	Pokemon & pokemon = attacker.pokemon();

	get_stat(pokemon, Stat::SPA).ev.set_value(252);
	get_nature(pokemon).name = Nature::MODEST;
	attacker.pokemon().stat_boost(Stat::SPA, 6);

	get_ability(pokemon) = Ability::Solar_Power;

	get_item(pokemon).name = Item::CHOICE_SPECS;
	
	calculate_special_attack(attacker.pokemon(), weather);
	if (get_stat(attacker.pokemon(), Stat::SPA).stat != max_special_attack)
		throw IncorrectCalculation (get_stat(attacker.pokemon(), Stat::SPA).stat, max_special_attack);
}

void defense_tests () {
	std::cout << "\tRunning Defense tests.\n";
	constexpr unsigned max_defense = 3684;

	Team defender;
	Weather weather;

	Level const level(100_ri);
	Gender const gender(Gender::MALE);
	defender.add_pokemon(Species::Shuckle, level, gender);
	Pokemon & pokemon = defender.pokemon();
	get_stat(pokemon, Stat::DEF).ev.set_value(252);
	get_nature(pokemon).name = Nature::BOLD;

	defender.pokemon().stat_boost(Stat::DEF, 6);

	get_ability(pokemon) = Ability::Marvel_Scale;
	Status::apply<Status::BURN>(pokemon, weather);

	calculate_defense(defender.pokemon(), weather);

	if (get_stat(pokemon, Stat::DEF).stat != max_defense)
		throw IncorrectCalculation(get_stat(pokemon, Stat::DEF).stat, max_defense);
}

void special_defense_tests () {
	std::cout << "\tRunning Special Defense tests.\n";
	constexpr unsigned max_special_defense = 3684;

	Team defender;
	Weather weather;
	weather.set_sand (Weather::Duration::permanent);

	Level const level(100_ri);
	Gender const gender(Gender::MALE);
	defender.add_pokemon(Species::Shuckle, level, gender);
	Pokemon & pokemon = defender.pokemon();
	get_stat(pokemon, Stat::SPD).ev.set_value(252);
	get_nature(pokemon).name = Nature::CALM;

	defender.pokemon().stat_boost(Stat::SPD, 6);

	calculate_special_defense(defender.pokemon(), weather);

	if (get_stat(pokemon, Stat::SPD).stat != max_special_defense)
		throw IncorrectCalculation (get_stat(pokemon, Stat::SPD).stat, max_special_defense);
}

void speed_tests () {
	std::cout << "\tRunning Speed tests.\n";
	constexpr unsigned max_speed = 12096;
	Weather weather;
	weather.set_rain (Weather::Duration::permanent);

	Team team;

	Level const level(100_ri);
	Gender const gender(Gender::GENDERLESS);
	team.add_pokemon(Species::Deoxys_Speed, level, gender);
	Pokemon & pokemon = team.pokemon();
	get_stat(pokemon, Stat::SPE).ev.set_value(252);
	get_nature(pokemon).name = Nature::TIMID;

	team.pokemon().stat_boost(Stat::SPE, 6);

	get_ability(pokemon) = Ability::Swift_Swim;

	get_item(pokemon).name = Item::CHOICE_SCARF;
	
	team.screens.activate_tailwind();
	
	calculate_speed(team, weather);

	if (get_stat(pokemon, Stat::SPE).stat != max_speed)
		throw IncorrectCalculation(get_stat(pokemon, Stat::SPE).stat, max_speed);
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
