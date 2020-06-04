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

#include <tm/test/calculate_damage.hpp>

#include <tm/test/incorrect_calculation.hpp>

#include <tm/move/calculate_damage.hpp>
#include <tm/move/moves.hpp>
#include <tm/move/other_move.hpp>
#include <tm/move/power.hpp>

#include <tm/pokemon/species.hpp>

#include <tm/generation.hpp>
#include <tm/team.hpp>
#include <tm/variable.hpp>
#include <tm/weather.hpp>

#include <containers/integer_range.hpp>

#include <iostream>

namespace technicalmachine {
namespace {
using namespace bounded::literal;

constexpr auto generation = Generation::four;
constexpr auto critical_hit = true;
constexpr auto max_damage_physical_move = Move(generation, Moves::Rollout);
constexpr auto max_damage_physical_move_type = Type::Rock;
constexpr auto max_damage_executed_physical_move = ExecutedMove{
	{max_damage_physical_move.name(), max_damage_physical_move_type},
	max_damage_physical_move.pp(),
	Variable{},
	critical_hit
};

Team max_damage_physical_attacker(Item const item, Ability const ability, Nature const nature) {
	auto attacker = Team(max_pokemon_per_team);
	
	attacker.add_pokemon(generation, Species::Shuckle, Level(100_bi), Gender::male, item, ability, nature);
	auto pokemon = attacker.pokemon();
	all_moves(pokemon).add(Move(max_damage_physical_move));

	pokemon.switch_in(generation, Weather());
	
	pokemon.defense_curl();
	for (unsigned n = 0; n != 10; ++n) {
		pokemon.successfully_use_move(max_damage_physical_move.name());
	}

	return attacker;
}

Team max_damage_special_attacker(Item const item, Ability const ability, Nature const nature) {
	auto attacker = Team(max_pokemon_per_team);
	auto weather = Weather();

	attacker.add_pokemon(generation, Species::Deoxys_Attack, Level(100_bi), Gender::genderless, item, ability, nature);
	attacker.pokemon().switch_in(generation, weather);
	attacker.pokemon().set_hp(generation, weather, 1_bi);
	
	return attacker;
}

Team max_damage_physical_defender() {
	auto defender = Team(max_pokemon_per_team);
	defender.add_pokemon(generation, Species::Combee, Level(1_bi), Gender::male, Item::None, Ability::Honey_Gather, Nature::Hasty);
	auto pokemon = defender.pokemon();
	pokemon.switch_in(generation, Weather());
	set_stat_ev(pokemon, StatNames::DEF, IV(0_bi), EV(0_bi));
	for (auto const n [[maybe_unused]] : containers::integer_range(3_bi)) {
		pokemon.stage()[StatNames::DEF] += -2_bi;
	}
	return defender;
}

Team max_damage_special_defender() {
	auto defender = Team(max_pokemon_per_team);
	defender.add_pokemon(generation, Species::Paras, Level(1_bi), Gender::male, Item::None, Ability::Dry_Skin, Nature::Hardy);
	auto d = defender.pokemon();
	d.switch_in(generation, Weather());
	set_stat_ev(d, StatNames::SPD, IV(0_bi), EV(0_bi));
	for (auto const n [[maybe_unused]] : containers::integer_range(3_bi)) {
		d.stage()[StatNames::SPD] += -2_bi;
	}

	return defender;
}

void physical_power_test() {
	std::cout << "\t\tRunning physical power tests.\n";
	constexpr auto max_power = 1440_bi;

	auto const attacker = max_damage_physical_attacker(Item::Rock_Incense, Ability::Rivalry, Nature::Hardy);

	auto const power = move_power(
		generation,
		attacker,
		max_damage_executed_physical_move,
		max_damage_physical_defender(),
		Weather()
	);
	check_equal(power, max_power);
}

void special_power_test() {
	std::cout << "\t\tRunning special power tests.\n";
	constexpr auto max_power = 342_bi;

	auto attacker = max_damage_special_attacker(Item::Wave_Incense, Ability::Torrent, Nature::Hardy);
	Pokemon & pokemon = attacker.pokemon();
	auto const move = all_moves(pokemon).add(Move(generation, Moves::Surf));

	Team defender = max_damage_special_defender();
	auto defender_pokemon = defender.pokemon();
	all_moves(defender_pokemon).add(Move(generation, Moves::Dive));
	defender_pokemon.successfully_use_move(Moves::Dive);
	defender_pokemon.use_vanish_move(generation, Weather());

	auto const power = move_power(
		generation,
		attacker,
		ExecutedMove{
			{move.name(), Type::Water},
			move.pp(),
			Variable{},
			critical_hit
		},
		defender,
		Weather{}
	);
	check_equal(power, max_power);
}

void power_test() {
	std::cout << "\tRunning power tests.\n";
	physical_power_test();
	special_power_test();
}

constexpr auto resistance_berry_activated = false;

void physical_damage_test() {
	std::cout << "\t\tRunning max physical damage tests.\n";
	constexpr auto max_damage = 95064912_bi;

	auto attacker = max_damage_physical_attacker(Item::Metronome, Ability::Pure_Power, Nature::Impish);

	auto a = attacker.pokemon();

	set_stat_ev(a, StatNames::DEF, EV(EV::max));
	a.activate_power_trick();
	a.stage()[StatNames::ATK] += 6_bi;

	auto const defender = max_damage_physical_defender();
	
	check_equal(
		calculate_damage(
			generation,
			attacker,
			max_damage_executed_physical_move,
			resistance_berry_activated,
			defender,
			FutureMove{false},
			Weather()
		),
		max_damage
	);
}

void special_damage_test() {
	std::cout << "\t\tRunning max special damage tests.\n";
	constexpr auto max_damage = 25696272_bi;
	Weather weather;
	weather.activate_sun_from_move(false);

	auto attacker = max_damage_special_attacker(Item::Metronome, Ability::Blaze, Nature::Modest);
	auto a = attacker.pokemon();
	auto const move = all_moves(a).add(Move(generation, Moves::Blast_Burn));
	a.set_type(Type::Fire);

	set_stat_ev(a, StatNames::SPA, EV(EV::max));
	attacker.pokemon().stage()[StatNames::SPA] += 6_bi;
	
	for (auto const n [[maybe_unused]] : containers::integer_range(10_bi)) {
		attacker.pokemon().successfully_use_move(move.name());
	}

	attacker.pokemon().activate_flash_fire();

	Team defender = max_damage_special_defender();

	check_equal(
		calculate_damage(
			generation,
			attacker,
			ExecutedMove{{move.name(), Type::Fire}, move.pp(), Variable{}, critical_hit},
			resistance_berry_activated,
			defender,
			FutureMove{false},
			weather
		),
		max_damage
	);
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
