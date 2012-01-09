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
	Team attacker (false, team_size);

	attacker.pokemon.set.push_back (Pokemon (Species::SHUCKLE, team_size));
	attacker.pokemon().move.set.insert (attacker.pokemon().move.set.begin (), Move (Move::ROLLOUT, 3, team_size));

	attacker.defense_curl = true;
	attacker.pokemon().move().times_used = 10;
	
	attacker.pokemon().def.iv = 31;
	attacker.pokemon().def.ev = 252 / 4;
	attacker.pokemon().nature.name = Nature::IMPISH;
	attacker.power_trick = true;
	attacker.pokemon().ability.name = Ability::PURE_POWER;
	Stat::boost (attacker.stage [Stat::ATK], 6);

	attacker.ch = true;
	attacker.pokemon().item.name = Item::METRONOME;
	attacker.pokemon().move().r = 100;
	
	calculate_attacking_stat (attacker, weather);

	return attacker;
}

Team create_max_damage_defender (Team const & attacker, Weather const & weather) {
	constexpr unsigned team_size = 6;
	Team defender (false, team_size);
	defender.pokemon.set.push_back (Pokemon (Species::COMBEE, team_size));

	defender.pokemon().level = 1;
	defender.pokemon().def.iv = 0;
	defender.pokemon().def.ev = 0;
	Stat::boost (defender.stage [Stat::DEF], -6);
	
	defender.pokemon().hp.stat = 0xFFFFFFFF;	
	calculate_defending_stat (attacker, defender, weather);
	
	return defender;
}

void power_test () {
	std::cout << "\tRunning power tests.\n";
	constexpr int max_power = 960;
	Weather weather;
	Team attacker = create_max_damage_attacker (weather);
	Team defender = create_max_damage_defender (attacker, weather);
	move_power (attacker, defender, weather);
	if (attacker.pokemon().move().power != max_power)
		throw IncorrectCalculation (attacker.pokemon().move().power, max_power);
}

void attack_test () {
	std::cout << "\tRunning attack stat tests.\n";
	constexpr unsigned max_attack = 4912;
	Weather weather;
	Team attacker = create_max_damage_attacker (weather);
	if (attacker.pokemon().atk.stat != max_attack)
		throw IncorrectCalculation (attacker.pokemon().atk.stat, max_attack);
}

void damage_test () {
	std::cout << "\tRunning max damage tests.\n";
	constexpr unsigned max_damage = 95064912;
	Weather weather;
	Team attacker = create_max_damage_attacker (weather);
	Team defender = create_max_damage_defender (attacker, weather);
	move_power (attacker, defender, weather);
	unsigned damage = damagecalculator (attacker, defender, weather);
	if (damage != max_damage)
		throw IncorrectCalculation (damage, max_damage);
}

}	// anonymous namespace

void damage_tests () {
	std::cout << "Running damage tests.\n";
	power_test ();
	attack_test ();
	damage_test ();
}

}	// namespace technicalmachine
