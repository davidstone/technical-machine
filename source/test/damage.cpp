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

Team max_damage_physical_attacker () {
	Team attacker;
	
	constexpr unsigned level = 100;
	Gender const gender(Gender::MALE);
	attacker.add_pokemon(Species::Shuckle, level, gender);
	Pokemon & pokemon = attacker.pokemon();
	pokemon.move.add(Moves::Rollout, 3);

	attacker.pokemon().defense_curl();
	for (unsigned n = 0; n != 10; ++n)
		attacker.pokemon().increment_move_use_counter();
	
	return attacker;
}

Team max_damage_special_attacker () {
	Team attacker;

	constexpr unsigned level = 100;
	Gender const gender(Gender::GENDERLESS);
	attacker.add_pokemon(Species::Deoxys_Attack, level, gender);
	attacker.pokemon().stat(Stat::HP).stat = 1;
	
	return attacker;
}

Team max_damage_physical_defender () {
	Team defender;
	constexpr unsigned level = 1;
	Gender const gender(Gender::MALE);
	defender.add_pokemon(Species::Combee, level, gender);
	auto & d = defender.pokemon();
	d.stat(Stat::DEF).iv = 0;
	d.stat(Stat::DEF).ev.set_value(0);
	d.stat_boost(Stat::DEF, -6);
	return defender;
}

Team max_damage_special_defender () {
	Team defender;
	constexpr unsigned level = 1;
	Gender const gender(Gender::MALE);
	defender.add_pokemon(Species::Paras, level, gender);
	auto & d = defender.pokemon();
	d.ability(Ability::Dry_Skin);

	d.stat(Stat::SPD).iv = 0;
	d.stat(Stat::SPD).ev.set_value(0);
	d.stat_boost(Stat::SPD, -6);

	return defender;
}

void physical_power_test () {
	std::cout << "\t\tRunning physical power tests.\n";
	constexpr unsigned max_power = 1440;

	Team attacker = max_damage_physical_attacker ();
	attacker.pokemon().item().name = Item::ROCK_INCENSE;
	attacker.pokemon().ability(Ability::Rivalry);

	unsigned const power = move_power(attacker.pokemon(), max_damage_physical_defender().pokemon(), Weather(), Variable());
	if (power != max_power)
		throw IncorrectCalculation (power, max_power);
}

void special_power_test () {
	std::cout << "\t\tRunning special power tests.\n";
	constexpr unsigned max_power = 342;

	Team attacker = max_damage_special_attacker ();
	Pokemon & pokemon = attacker.pokemon();
	pokemon.move.add(Moves::Surf, 3);
	pokemon.item().name = Item::WAVE_INCENSE;
	pokemon.ability() = Ability(Ability::Torrent);

	Team defender = max_damage_special_defender ();
	defender.pokemon().dive();

	unsigned const power = move_power(attacker.pokemon(), defender.pokemon(), Weather(), Variable());
	if (power != max_power)
		throw IncorrectCalculation (power, max_power);
}

void power_test () {
	std::cout << "\tRunning power tests.\n";
	physical_power_test ();
	special_power_test ();
}

void physical_damage_test () {
	std::cout << "\t\tRunning max physical damage tests.\n";
	constexpr unsigned max_damage = 95064912;
	Weather const weather;

	Team attacker = max_damage_physical_attacker ();
	
	Pokemon & a = attacker.pokemon();
	a.stat(Stat::DEF).ev.set_value(252);
	a.nature().name = Nature::IMPISH;
	attacker.pokemon().activate_power_trick();
	a.ability() = Ability(Ability::Pure_Power);
	attacker.pokemon().stat_boost(Stat::ATK, 6);
	calculate_attacking_stat (attacker.pokemon(), weather);

	a.item().name = Item::METRONOME;

	attacker.pokemon().set_critical_hit(true);

	Team defender = max_damage_physical_defender();
	calculate_defending_stat(attacker.pokemon(), defender.pokemon(), weather);
	
	unsigned const damage = uncapped_damage(attacker.pokemon(), defender, weather, Variable());
	if (damage != max_damage)
		throw IncorrectCalculation (damage, max_damage);
}

void special_damage_test () {
	std::cout << "\t\tRunning max special damage tests.\n";
	constexpr unsigned max_damage = 25696272;
	Weather weather;
	weather.set_sun (Weather::Duration::permanent);

	Team attacker = max_damage_special_attacker();
	Pokemon & a = attacker.pokemon();
	a.move.add(Moves::Blast_Burn, 3);
	a.change_type(Type::Fire);

	a.stat(Stat::SPA).ev.set_value(252);
	a.nature().name = Nature::MODEST;
	attacker.pokemon().stat_boost(Stat::SPA, 6);
	calculate_attacking_stat(attacker.pokemon(), weather);
	
	a.item().name = Item::METRONOME;
	for (unsigned n = 0; n != 10; ++n)
		attacker.pokemon().increment_move_use_counter();

	a.ability() = Ability(Ability::Blaze);
	attacker.pokemon().set_critical_hit(true);
	attacker.pokemon().activate_flash_fire();

	Team defender = max_damage_special_defender();
	calculate_defending_stat(attacker.pokemon(), defender.pokemon(), weather);

	unsigned const damage = uncapped_damage(attacker.pokemon(), defender, weather, Variable());
	if (damage != max_damage)
		throw IncorrectCalculation (damage, max_damage);
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
