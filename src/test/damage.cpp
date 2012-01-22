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
#include <random>
#include <stdexcept>
#include <string>

#include "../damage.hpp"
#include "../move_power.hpp"
#include "../team.hpp"
#include "../weather.hpp"

namespace technicalmachine {
namespace {

class IncorrectCalculation : public std::logic_error {
	public:
		IncorrectCalculation (unsigned result, unsigned expected):
			logic_error ("Result of " + std::to_string (result) + " instead of the expected " + std::to_string (expected) + "\n")
			{
		}
};

Team create_max_damage_attacker (Weather const & weather) {
	constexpr unsigned team_size = 6;
	// Temporary until I redesign my Team constructor to not require a random
	// engine when it's not needed.
	std::mt19937 unused;
	Team attacker (false, team_size, unused);

	attacker.pokemon.set.push_back (Pokemon (Species::SHUCKLE, team_size));
	Pokemon & pokemon = attacker.pokemon();
	pokemon.move.set.insert (pokemon.move.set.begin (), Move (Move::ROLLOUT, 3, team_size));

	attacker.defense_curl = true;
	pokemon.move().times_used = 10;
	
	pokemon.def.iv = 31;
	pokemon.def.ev = 252 / 4;
	pokemon.nature.name = Nature::IMPISH;
	attacker.power_trick = true;
	pokemon.ability.name = Ability::PURE_POWER;
	Stat::boost (attacker.stage [Stat::ATK], 6);

	attacker.ch = true;
	pokemon.item.name = Item::METRONOME;
	pokemon.move().r = 100;
	
	calculate_attacking_stat (attacker, weather);

	return attacker;
}

Team create_max_damage_special_attacker (Weather const & weather, Ability::Abilities ability, Move::Moves move = Move::BLAST_BURN) {
	constexpr unsigned team_size = 6;
	// Temporary until I redesign my Team constructor to not require a random
	// engine when it's not needed.
	std::mt19937 unused;
	Team attacker (false, team_size, unused);

	attacker.pokemon.set.push_back (Pokemon (Species::DEOXYS_A, team_size));
	Pokemon & pokemon = attacker.pokemon();
	pokemon.move.set.insert (pokemon.move.set.begin (), Move (move, 3, team_size));

	pokemon.move().times_used = 10;
	
	pokemon.spa.iv = 31;
	pokemon.spa.ev = 252 / 4;
	pokemon.nature.name = Nature::MODEST;
	pokemon.ability.name = ability;
	Stat::boost (attacker.stage [Stat::SPA], 6);
	pokemon.type.types = std::vector <Type> { pokemon.move().type };

	attacker.ch = true;
	attacker.flash_fire = true;
	pokemon.item.name = Item::METRONOME;
	pokemon.move().r = 100;
	pokemon.hp.stat = 1;
	
	calculate_attacking_stat (attacker, weather);

	return attacker;
}

Team create_max_damage_physical_defender (Team const & attacker, Weather const & weather) {
	constexpr unsigned team_size = 6;
	// Temporary until I redesign my Team constructor to not require a random
	// engine when it's not needed.
	std::mt19937 unused;
	Team defender (false, team_size, unused);
	defender.pokemon.set.push_back (Pokemon (Species::COMBEE, team_size));
	Pokemon & pokemon = defender.pokemon();

	pokemon.level = 1;
	pokemon.def.iv = 0;
	pokemon.def.ev = 0;
	Stat::boost (defender.stage [Stat::DEF], -6);
	
	pokemon.hp.stat = 0xFFFFFFFF;	
	calculate_defending_stat (attacker, defender, weather);
	
	return defender;
}

Team create_max_damage_special_defender (Team const & attacker, Weather const & weather) {
	constexpr unsigned team_size = 6;
	// Temporary until I redesign my Team constructor to not require a random
	// engine when it's not needed.
	std::mt19937 unused;
	Team defender (false, team_size, unused);
	defender.pokemon.set.push_back (Pokemon (Species::PARAS, team_size));
	Pokemon & pokemon = defender.pokemon();

	pokemon.level = 1;
	pokemon.spd.iv = 0;
	pokemon.spd.ev = 0;
	Stat::boost (defender.stage [Stat::SPD], -6);
	pokemon.ability.name = Ability::DRY_SKIN;
	
	pokemon.hp.stat = 0xFFFFFFFF;	
	calculate_defending_stat (attacker, defender, weather);
	return defender;
}

void physical_power_test () {
	std::cout << "\t\tRunning physical power tests.\n";
	constexpr int max_power = 960;
	Weather weather;
	Team attacker = create_max_damage_attacker (weather);
	Team defender = create_max_damage_physical_defender (attacker, weather);
	move_power (attacker, defender, weather);
	if (attacker.pokemon().move().power != max_power)
		throw IncorrectCalculation (attacker.pokemon().move().power, max_power);
}

void special_power_test () {
	std::cout << "\t\tRunning special power tests.\n";
	constexpr int max_power = 342;
	Weather weather;
	Team attacker = create_max_damage_special_attacker (weather, Ability::TORRENT, Move::SURF);
	attacker.pokemon().item = Item (Item::WAVE_INCENSE);
	Team defender = create_max_damage_special_defender (attacker, weather);
	defender.vanish = Vanish::DIVED;
	move_power (attacker, defender, weather);
	if (attacker.pokemon().move().power != max_power)
		throw IncorrectCalculation (attacker.pokemon().move().power, max_power);
}

void power_test () {
	std::cout << "\tRunning power tests.\n";
	physical_power_test ();
	special_power_test ();
}

void attack_test () {
	std::cout << "\t\tRunning attack stat tests.\n";
	constexpr unsigned max_attack = 4912;
	Weather weather;
	Team attacker = create_max_damage_attacker (weather);
	if (attacker.pokemon().atk.stat != max_attack)
		throw IncorrectCalculation (attacker.pokemon().atk.stat, max_attack);
}

void special_attack_test () {
	std::cout << "\t\tRunning special attack stat tests.\n";
	constexpr unsigned max_special_attack = 3024;
	Weather weather;
	weather.set_sun (-1);
	Team attacker = create_max_damage_special_attacker (weather, Ability::SOLAR_POWER);
	if (attacker.pokemon().spa.stat != max_special_attack)
		throw IncorrectCalculation (attacker.pokemon().spa.stat, max_special_attack);
}

void attacking_stat_test () {
	std::cout << "\tRunning attacking stat tests.\n";
	attack_test ();
	special_attack_test ();
}

void physical_damage_test () {
	std::cout << "\t\tRunning max physical damage tests.\n";
	constexpr unsigned max_damage = 95064912;
	Weather weather;
	Team attacker = create_max_damage_attacker (weather);
	Team defender = create_max_damage_physical_defender (attacker, weather);
	move_power (attacker, defender, weather);
	unsigned damage = damagecalculator (attacker, defender, weather);
	if (damage != max_damage)
		throw IncorrectCalculation (damage, max_damage);
}

void special_damage_test () {
	std::cout << "\t\tRunning max special damage tests.\n";
	constexpr unsigned max_damage = 25696272;
	Weather weather;
	weather.set_sun (-1);
	Team attacker = create_max_damage_special_attacker (weather, Ability::BLAZE);
	Team defender = create_max_damage_special_defender (attacker, weather);
	move_power (attacker, defender, weather);
	unsigned damage = damagecalculator (attacker, defender, weather);
	if (damage != max_damage)
		throw IncorrectCalculation (damage, max_damage);
}

void damage_test () {
	std::cout << "\tRunning max damage tests.\n";
	physical_damage_test ();
	special_damage_test ();
}

}	// anonymous namespace

void damage_tests () {
	std::cout << "Running damage tests.\n";
	power_test ();
	attacking_stat_test ();
	damage_test ();
	std::cout << "Damage tests passed.\n\n";
}

}	// namespace technicalmachine
