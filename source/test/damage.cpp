// Test damage-related functions
// Copyright (C) 2012 David Stone
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

#include "damage.hpp"

#include <iostream>
#include <vector>

#include "incorrect_calculation.hpp"

#include "../damage.hpp"
#include "../team.hpp"
#include "../variable.hpp"
#include "../weather.hpp"

#include "../move/power.hpp"
#include "../move/moves.hpp"

#include "../pokemon/species.hpp"

namespace technicalmachine {
namespace {
using namespace bounded_integer::literal;

Team max_damage_physical_attacker () {
	Team attacker;
	
	Level const level(100_bi);
	Gender const gender(Gender::MALE);
	attacker.add_pokemon(Species::Shuckle, level, gender);
	Pokemon & pokemon = attacker.pokemon();
	pokemon.move.add(Moves::Rollout);

	attacker.pokemon().defense_curl();
	for (unsigned n = 0; n != 10; ++n) {
		attacker.pokemon().increment_move_use_counter();
	}
	
	return attacker;
}

Team max_damage_special_attacker () {
	Team attacker;

	Level const level(100_bi);
	Gender const gender(Gender::GENDERLESS);
	attacker.add_pokemon(Species::Deoxys_Attack, level, gender);
	get_hp(attacker.pokemon()) = 1_bi;
	
	return attacker;
}

Team max_damage_physical_defender () {
	Team defender;
	Level const level(1_bi);
	Gender const gender(Gender::MALE);
	defender.add_pokemon(Species::Combee, level, gender);
	auto & d = defender.pokemon();
	get_stat(d, StatNames::DEF).iv = IV(0_bi);
	get_stat(d, StatNames::DEF).ev = EV(0_bi);
	for (unsigned n = 0; n != 3; ++n) {
		d.stat_boost(StatNames::DEF, -2_bi);
	}
	return defender;
}

Team max_damage_special_defender () {
	Team defender;
	Level const level(1_bi);
	Gender const gender(Gender::MALE);
	defender.add_pokemon(Species::Paras, level, gender);
	auto & d = defender.pokemon();
	get_ability(d) = Ability::Dry_Skin;

	get_stat(d, StatNames::SPD).iv = IV(0_bi);
	get_stat(d, StatNames::SPD).ev = EV(0_bi);
	for (unsigned n = 0; n != 3; ++n) {
		d.stat_boost(StatNames::SPD, -2_bi);
	}

	return defender;
}

void physical_power_test () {
	std::cout << "\t\tRunning physical power tests.\n";
	constexpr unsigned max_power = 1440;

	Team attacker = max_damage_physical_attacker ();
	get_item(attacker.pokemon()).name = Item::ROCK_INCENSE;
	get_ability(attacker.pokemon()) = Ability::Rivalry;

	unsigned const power = move_power(attacker.pokemon(), max_damage_physical_defender().pokemon(), Weather(), Variable());
	if (power != max_power) {
		throw IncorrectCalculation (power, max_power);
	}
}

void special_power_test () {
	std::cout << "\t\tRunning special power tests.\n";
	constexpr unsigned max_power = 342;

	Team attacker = max_damage_special_attacker();
	Pokemon & pokemon = attacker.pokemon();
	pokemon.move.add(Moves::Surf);
	get_item(pokemon).name = Item::WAVE_INCENSE;
	get_ability(pokemon) = Ability::Torrent;

	Team defender = max_damage_special_defender();
	defender.pokemon().dive();

	unsigned const power = move_power(attacker.pokemon(), defender.pokemon(), Weather{}, Variable{});
	if (power != max_power) {
		throw IncorrectCalculation(power, max_power);
	}
}

void power_test () {
	std::cout << "\tRunning power tests.\n";
	physical_power_test ();
	special_power_test ();
}

void physical_damage_test () {
	std::cout << "\t\tRunning max physical damage tests.\n";
	constexpr auto max_damage = 95064912_bi;
	Weather const weather;

	Team attacker = max_damage_physical_attacker ();
	
	Pokemon & a = attacker.pokemon();
	get_stat(a, StatNames::DEF).ev = EV(bounded_integer::make_bounded<EV::max>());
	get_nature(a).name = Nature::IMPISH;
	attacker.pokemon().activate_power_trick();
	get_ability(a) = Ability::Pure_Power;
	attacker.pokemon().stat_boost(StatNames::ATK, 6_bi);
	calculate_attacking_stat (attacker.pokemon(), weather);

	get_item(a).name = Item::METRONOME;

	attacker.pokemon().set_critical_hit(true);

	Team defender = max_damage_physical_defender();
	calculate_defending_stat(attacker.pokemon(), defender.pokemon(), weather);
	
	auto const damage = damage_calculator(attacker.pokemon(), defender, weather, Variable{});
	if (damage != max_damage) {
		throw IncorrectCalculation(static_cast<unsigned>(damage), static_cast<unsigned>(max_damage));
	}
}

void special_damage_test () {
	std::cout << "\t\tRunning max special damage tests.\n";
	constexpr auto max_damage = 25696272_bi;
	Weather weather;
	weather.set_sun (Weather::Duration::permanent);

	Team attacker = max_damage_special_attacker();
	Pokemon & a = attacker.pokemon();
	a.move.add(Moves::Blast_Burn);
	a.change_type(Type::Fire);

	get_stat(a, StatNames::SPA).ev = EV(bounded_integer::make_bounded<EV::max>());
	get_nature(a).name = Nature::MODEST;
	attacker.pokemon().stat_boost(StatNames::SPA, 6_bi);
	calculate_attacking_stat(attacker.pokemon(), weather);
	
	get_item(a).name = Item::METRONOME;
	for (unsigned n = 0; n != 10; ++n) {
		attacker.pokemon().increment_move_use_counter();
	}

	get_ability(a) = Ability::Blaze;
	attacker.pokemon().set_critical_hit(true);
	attacker.pokemon().activate_flash_fire();

	Team defender = max_damage_special_defender();
	calculate_defending_stat(attacker.pokemon(), defender.pokemon(), weather);

	auto const damage = damage_calculator(attacker.pokemon(), defender, weather, Variable{});
	if (damage != max_damage) {
		throw IncorrectCalculation(static_cast<unsigned>(damage), static_cast<unsigned>(max_damage));
	}
}

void damage_test () {
	std::cout << "\tRunning max damage tests.\n";
	physical_damage_test ();
	special_damage_test ();
}

}	// unnamed namespace

void damage_tests () {
	std::cout << "Running damage tests.\n";
	power_test ();
	damage_test ();
	std::cout << "Damage tests passed.\n\n";
}

}	// namespace technicalmachine
