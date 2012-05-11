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
#include "../move_power.hpp"
#include "../team.hpp"
#include "../weather.hpp"

namespace technicalmachine {
namespace {

constexpr unsigned team_size = 6;

Team create_max_damage_physical_attacker () {
	Team attacker;

	attacker.add_pokemon(Species::SHUCKLE);
	Pokemon & pokemon = attacker.pokemon();
	pokemon.move.add(Move (Move::ROLLOUT, 3, team_size));

	attacker.defense_curl = true;
	pokemon.move().times_used = 10;
	
	return attacker;
}

Team create_max_damage_special_attacker () {
	Team attacker;

	attacker.add_pokemon(Species::DEOXYS_A);
	attacker.pokemon().hp.stat = 1;
	
	return attacker;
}

Team create_max_damage_physical_defender () {
	Team defender;
	defender.add_pokemon(Species::COMBEE);
	Pokemon & d = defender.pokemon();
	d.level = 1;
	d.def.iv = 0;
	d.def.ev = 0;
	defender.stage.boost(Stat::DEF, -6);
	return defender;
}

Team create_max_damage_special_defender () {
	Team defender;
	defender.add_pokemon(Species::PARAS);
	Pokemon & d = defender.pokemon();
	d.ability.name = Ability::DRY_SKIN;

	d.level = 1;
	d.spd.iv = 0;
	d.spd.ev = 0;
	defender.stage.boost(Stat::SPD, -6);

	return defender;
}

void physical_power_test () {
	std::cout << "\t\tRunning physical power tests.\n";
	constexpr unsigned max_power = 1440;

	Team attacker = create_max_damage_physical_attacker ();
	attacker.pokemon().item.name = Item::ROCK_INCENSE;
	attacker.pokemon().ability.name = Ability::RIVALRY;

	unsigned const power = move_power(attacker, create_max_damage_physical_defender(), Weather());
	if (power != max_power)
		throw IncorrectCalculation(power, max_power);
}

void special_power_test () {
	std::cout << "\t\tRunning special power tests.\n";
	constexpr unsigned max_power = 342;

	Team attacker = create_max_damage_special_attacker ();
	Pokemon & pokemon = attacker.pokemon();
	pokemon.move.add (Move (Move::SURF, 3, team_size));
	pokemon.item.name = Item::WAVE_INCENSE;
	pokemon.ability.name = Ability::TORRENT;

	Team defender = create_max_damage_special_defender ();
	defender.vanish = Vanish::DIVED;

	unsigned const power = move_power(attacker, defender, Weather());
	if (power != max_power)
		throw IncorrectCalculation(power, max_power);
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

	Team attacker = create_max_damage_physical_attacker ();
	
	Pokemon & a = attacker.pokemon();
	a.def.ev = 252 / 4;
	a.nature.name = Nature::IMPISH;
	attacker.power_trick = true;
	a.ability.name = Ability::PURE_POWER;
	attacker.stage.maximize_attack();
	calculate_attacking_stat (attacker, weather);

	a.item.name = Item::METRONOME;

	attacker.ch = true;

	Team defender = create_max_damage_physical_defender ();
	calculate_defending_stat (attacker, defender, weather);
	
	attacker.pokemon().move().r = 100;
	unsigned damage = uncapped_damage (attacker, defender, weather, move_power(attacker, defender, weather));
	if (damage != max_damage)
		throw IncorrectCalculation (damage, max_damage);
}

void special_damage_test () {
	std::cout << "\t\tRunning max special damage tests.\n";
	constexpr unsigned max_damage = 25696272;
	Weather weather;
	weather.set_sun (Weather::Duration::permanent);

	Team attacker = create_max_damage_special_attacker ();
	Pokemon & a = attacker.pokemon();
	a.move.add(Move (Move::BLAST_BURN, 3, team_size));
	a.type.change_type(Type::FIRE);

	a.spa.ev = 252 / 4;
	a.nature.name = Nature::MODEST;
	attacker.stage.boost(Stat::SPA, 6);
	calculate_attacking_stat (attacker, weather);
	
	a.item.name = Item::METRONOME;
	a.move().times_used = 10;

	a.ability.name = Ability::BLAZE;
	attacker.ch = true;
	attacker.flash_fire = true;

	Team defender = create_max_damage_special_defender ();
	calculate_defending_stat (attacker, defender, weather);

	attacker.pokemon().move().r = 100;
	unsigned damage = uncapped_damage(attacker, defender, weather, move_power(attacker, defender, weather));
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
