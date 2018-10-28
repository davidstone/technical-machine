// Test damage-related functions
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

#include <tm/test/damage.hpp>

#include <tm/test/incorrect_calculation.hpp>

#include <tm/damage.hpp>
#include <tm/team.hpp>
#include <tm/variable.hpp>
#include <tm/weather.hpp>

#include <tm/move/power.hpp>
#include <tm/move/moves.hpp>

#include <tm/pokemon/species.hpp>

#include <containers/integer_range.hpp>

#include <iostream>

namespace technicalmachine {
namespace {
using namespace bounded::literal;

constexpr auto max_damage_physical_move = Move(Moves::Rollout);

Team max_damage_physical_attacker() {
	Team attacker(max_pokemon_per_team);
	
	Level const level(100_bi);
	attacker.add_pokemon(Species::Shuckle, level, Gender::male);
	Pokemon & pokemon = attacker.pokemon();
	all_moves(pokemon).emplace_back(max_damage_physical_move);

	attacker.pokemon().defense_curl();
	for (unsigned n = 0; n != 10; ++n) {
		attacker.pokemon().increment_move_use_counter(max_damage_physical_move);
	}
	
	return attacker;
}

Team max_damage_special_attacker() {
	Team attacker(max_pokemon_per_team);

	Level const level(100_bi);
	attacker.add_pokemon(Species::Deoxys_Attack, level, Gender::genderless);
	get_hp(attacker.pokemon()) = 1_bi;
	
	return attacker;
}

Team max_damage_physical_defender() {
	Team defender(max_pokemon_per_team);
	Level const level(1_bi);
	defender.add_pokemon(Species::Combee, level, Gender::male);
	auto && pokemon = defender.pokemon();
	set_stat_ev(pokemon, StatNames::DEF, EV(0_bi), IV(0_bi));
	get_nature(pokemon) = Nature::Hasty;
	for (auto const n [[maybe_unused]] : containers::integer_range(3_bi)) {
		boost(stage(pokemon), StatNames::DEF, -2_bi);
	}
	return defender;
}

Team max_damage_special_defender() {
	Team defender(max_pokemon_per_team);
	Level const level(1_bi);
	defender.add_pokemon(Species::Paras, level, Gender::male);
	auto && d = defender.pokemon();
	get_ability(d) = Ability::Dry_Skin;

	set_stat_ev(d, StatNames::SPD, EV(0_bi), IV(0_bi));
	for (auto const n [[maybe_unused]] : containers::integer_range(3_bi)) {
		boost(stage(d), StatNames::SPD, -2_bi);
	}

	return defender;
}

void physical_power_test() {
	std::cout << "\t\tRunning physical power tests.\n";
	constexpr auto max_power = 1440_bi;

	Team attacker = max_damage_physical_attacker();
	auto pokemon = attacker.pokemon();
	get_item(pokemon) = Item::Rock_Incense;
	get_ability(pokemon) = Ability::Rivalry;

	auto const power = move_power(attacker, max_damage_physical_move, false, max_damage_physical_defender(), false, Weather{}, Variable{});
	check_equal(power, max_power);
}

void special_power_test() {
	std::cout << "\t\tRunning special power tests.\n";
	constexpr auto max_power = 342_bi;

	Team attacker = max_damage_special_attacker();
	Pokemon & pokemon = attacker.pokemon();
	auto const move = all_moves(pokemon).emplace_back(Moves::Surf);
	get_item(pokemon) = Item::Wave_Incense;
	get_ability(pokemon) = Ability::Torrent;

	Team defender = max_damage_special_defender();
	defender.pokemon().dive();

	auto const power = move_power(attacker, move, false, defender, false, Weather{}, Variable{});
	check_equal(power, max_power);
}

void power_test() {
	std::cout << "\tRunning power tests.\n";
	physical_power_test();
	special_power_test();
}

constexpr auto critical_hit = true;

void physical_damage_test() {
	std::cout << "\t\tRunning max physical damage tests.\n";
	constexpr auto max_damage = 95064912_bi;
	auto const weather = Weather{};

	Team attacker = max_damage_physical_attacker();
	
	Pokemon & a = attacker.pokemon();

	set_stat_ev(a, StatNames::DEF, EV(EV::max));
	get_nature(a) = Nature::Impish;
	attacker.pokemon().activate_power_trick();
	get_ability(a) = Ability::Pure_Power;
	boost(stage(attacker.pokemon()), StatNames::ATK, 6_bi);

	get_item(a) = Item::Metronome;

	Team defender = max_damage_physical_defender();
	
	check_equal(damage_calculator(attacker, max_damage_physical_move, false, defender, false, weather, Variable{}, critical_hit), max_damage);
}

void special_damage_test() {
	std::cout << "\t\tRunning max special damage tests.\n";
	constexpr auto max_damage = 25696272_bi;
	Weather weather;
	weather.activate_sun(Weather::Duration::permanent);

	Team attacker = max_damage_special_attacker();
	Pokemon & a = attacker.pokemon();
	auto const move = all_moves(a).emplace_back(Moves::Blast_Burn);
	a.change_type(Type::Fire);

	set_stat_ev(a, StatNames::SPA, EV(EV::max));
	get_nature(a) = Nature::Modest;
	boost(stage(attacker.pokemon()), StatNames::SPA, 6_bi);
	
	get_item(a) = Item::Metronome;
	for (auto const n [[maybe_unused]] : containers::integer_range(10_bi)) {
		attacker.pokemon().increment_move_use_counter(move);
	}

	get_ability(a) = Ability::Blaze;
	attacker.pokemon().activate_flash_fire();

	Team defender = max_damage_special_defender();

	check_equal(damage_calculator(attacker, move, false, defender, false, weather, Variable{}, critical_hit), max_damage);
}

void damage_test() {
	std::cout << "\tRunning max damage tests.\n";
	physical_damage_test();
	special_damage_test();
}

}	// namespace

void damage_tests() {
	std::cout << "Running damage tests.\n";
	power_test();
	damage_test();
	std::cout << "Damage tests passed.\n\n";
}

}	// namespace technicalmachine
