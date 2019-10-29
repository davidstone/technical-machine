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

constexpr auto max_damage_physical_move = Move(generation, Moves::Rollout);

Team max_damage_physical_attacker(Item const item, Ability const ability, Nature const nature) {
	auto attacker = Team(max_pokemon_per_team);
	
	auto & pokemon = attacker.add_pokemon(generation, Species::Shuckle, Level(100_bi), Gender::male, item, ability, nature);
	containers::emplace_back(all_moves(pokemon), max_damage_physical_move);

	attacker.pokemon().defense_curl();
	for (unsigned n = 0; n != 10; ++n) {
		attacker.pokemon().increment_move_use_counter(max_damage_physical_move.name());
	}
	
	return attacker;
}

Team max_damage_special_attacker(Item const item, Ability const ability, Nature const nature) {
	auto attacker = Team(max_pokemon_per_team);

	attacker.add_pokemon(generation, Species::Deoxys_Attack, Level(100_bi), Gender::genderless, item, ability, nature);
	get_hp(attacker.pokemon()) = 1_bi;
	
	return attacker;
}

Team max_damage_physical_defender() {
	auto defender = Team(max_pokemon_per_team);
	defender.add_pokemon(generation, Species::Combee, Level(1_bi), Gender::male, Item::No_Item, Ability::Honey_Gather, Nature::Hasty);
	auto pokemon = defender.pokemon();
	set_stat_ev(pokemon, StatNames::DEF, EV(0_bi), IV(0_bi));
	for (auto const n [[maybe_unused]] : containers::integer_range(3_bi)) {
		boost(pokemon.stage(), StatNames::DEF, -2_bi);
	}
	return defender;
}

Team max_damage_special_defender() {
	auto defender = Team(max_pokemon_per_team);
	defender.add_pokemon(generation, Species::Paras, Level(1_bi), Gender::male, Item::No_Item, Ability::Dry_Skin, Nature::Hardy);

	auto d = defender.pokemon();
	set_stat_ev(d, StatNames::SPD, EV(0_bi), IV(0_bi));
	for (auto const n [[maybe_unused]] : containers::integer_range(3_bi)) {
		boost(d.stage(), StatNames::SPD, -2_bi);
	}

	return defender;
}

constexpr auto critical_hit = true;

void physical_power_test() {
	std::cout << "\t\tRunning physical power tests.\n";
	constexpr auto max_power = 1440_bi;

	auto const attacker = max_damage_physical_attacker(Item::Rock_Incense, Ability::Rivalry, Nature::Hardy);

	auto const power = move_power(
		generation,
		attacker,
		ExecutedMove{
			max_damage_physical_move.name(),
			max_damage_physical_move.pp(),
			Variable{},
			critical_hit
		},
		max_damage_physical_defender(),
		Weather{}
	);
	check_equal(power, max_power);
}

void special_power_test() {
	std::cout << "\t\tRunning special power tests.\n";
	constexpr auto max_power = 342_bi;

	auto attacker = max_damage_special_attacker(Item::Wave_Incense, Ability::Torrent, Nature::Hardy);
	Pokemon & pokemon = attacker.pokemon();
	auto const move = containers::emplace_back(all_moves(pokemon), generation, Moves::Surf);

	Team defender = max_damage_special_defender();
	defender.pokemon().dive();

	auto const power = move_power(
		generation,
		attacker,
		ExecutedMove{
			move.name(),
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

void physical_damage_test() {
	std::cout << "\t\tRunning max physical damage tests.\n";
	constexpr auto max_damage = 95064912_bi;
	auto const weather = Weather{};

	auto attacker = max_damage_physical_attacker(Item::Metronome, Ability::Pure_Power, Nature::Impish);
	
	auto a = attacker.pokemon();

	set_stat_ev(a, StatNames::DEF, EV(EV::max));
	a.activate_power_trick();
	boost(a.stage(), StatNames::ATK, 6_bi);

	auto const defender = max_damage_physical_defender();
	
	check_equal(
		calculate_uncapped_damage(
			generation,
			attacker,
			ExecutedMove{
				max_damage_physical_move.name(),
				max_damage_physical_move.pp(),
				Variable{},
				critical_hit
			},
			defender,
			FutureMove{false},
			weather
		),
		max_damage
	);
}

void special_damage_test() {
	std::cout << "\t\tRunning max special damage tests.\n";
	constexpr auto max_damage = 25696272_bi;
	Weather weather;
	weather.activate_sun(Weather::permanent);

	auto attacker = max_damage_special_attacker(Item::Metronome, Ability::Blaze, Nature::Modest);
	Pokemon & a = attacker.pokemon();
	auto const move = containers::emplace_back(all_moves(a), generation, Moves::Blast_Burn);
	a.change_type(Type::Fire);

	set_stat_ev(a, StatNames::SPA, EV(EV::max));
	boost(attacker.pokemon().stage(), StatNames::SPA, 6_bi);
	
	for (auto const n [[maybe_unused]] : containers::integer_range(10_bi)) {
		attacker.pokemon().increment_move_use_counter(move.name());
	}

	attacker.pokemon().activate_flash_fire();

	Team defender = max_damage_special_defender();

	check_equal(
		calculate_uncapped_damage(
			generation,
			attacker,
			ExecutedMove{move.name(), move.pp(), Variable{}, critical_hit},
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
