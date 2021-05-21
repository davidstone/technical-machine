// Test stat calculations
// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <tm/test/stat.hpp>

#include <tm/test/incorrect_calculation.hpp>

#include <tm/pokemon/max_pokemon_per_team.hpp>
#include <tm/pokemon/species.hpp>

#include <tm/stat/calculate.hpp>
#include <tm/stat/ev.hpp>
#include <tm/stat/iv.hpp>

#include <tm/team.hpp>
#include <tm/weather.hpp>

#include <containers/integer_range.hpp>

#include <iostream>
#include <string>

namespace technicalmachine {
namespace {
using namespace bounded::literal;

constexpr auto generation = Generation::four;

constexpr auto critical_hit = false;
constexpr auto physical_move = Moves::Tackle;

void attack_tests() {
	std::cout << "\tRunning Attack tests.\n";
	constexpr auto max_attack = 7368_bi;

	auto weather = Weather();

	auto attacker = Team<generation>(max_pokemon_per_team);

	attacker.add_pokemon(Pokemon<generation>(
		Species::Shuckle,
		Level(100_bi),
		Gender::male,
		Item::Choice_Band,
		Ability::Pure_Power,
		CombinedStats<generation>{
			Nature::Impish,
			IVs(
				IV(31_bi),
				IV(31_bi),
				IV(31_bi),
				IV(31_bi),
				IV(31_bi),
				IV(31_bi)
			),
			EVs(
				EV(0_bi),
				EV(0_bi),
				EV(252_bi),
				EV(0_bi),
				EV(0_bi),
				EV(0_bi)
			)
		},
		RegularMoves({Move(generation, Moves::Tackle)})
	));
	auto pokemon = attacker.pokemon();

	pokemon.switch_in(weather);

	pokemon.activate_power_trick();
	pokemon.stage()[BoostableStat::atk] += 6_bi;

	check_equal(calculate_attack(attacker.pokemon().as_const(), Type::Normal, Ability::Honey_Gather, weather, critical_hit), max_attack);
}

void special_attack_tests() {
	std::cout << "\tRunning Special Attack tests.\n";
	constexpr auto max_special_attack = 4536_bi;
	auto weather = Weather();
	weather.activate_sun_from_move(false);
	auto attacker = Team<generation>(max_pokemon_per_team);

	attacker.add_pokemon(Pokemon<generation>(
		Species::Deoxys_Attack,
		Level(100_bi),
		Gender::genderless,
		Item::Choice_Specs,
		Ability::Solar_Power,
		CombinedStats<generation>{
			Nature::Modest,
			IVs(
				IV(31_bi),
				IV(31_bi),
				IV(31_bi),
				IV(31_bi),
				IV(31_bi),
				IV(31_bi)
			),
			EVs(
				EV(0_bi),
				EV(0_bi),
				EV(0_bi),
				EV(252_bi),
				EV(0_bi),
				EV(0_bi)
			)
		},
		RegularMoves({Move(generation, Moves::Psychic)})
	));
	auto pokemon = attacker.pokemon();

	pokemon.switch_in(weather);

	pokemon.stage()[BoostableStat::spa] += 6_bi;

	check_equal(calculate_special_attack(attacker.pokemon().as_const(), Type::Water, Ability::Honey_Gather, weather, critical_hit), max_special_attack);
}

void max_defense_test() {
	std::cout << "\t\tRunning max Defense test.\n";
	constexpr auto max_defense = 3684_bi;

	constexpr auto weather = Weather();

	auto defender = Team<generation>(max_pokemon_per_team);

	defender.add_pokemon(Pokemon<generation>(
		Species::Shuckle,
		Level(100_bi),
		Gender::male,
		Item::None,
		Ability::Marvel_Scale,
		CombinedStats<generation>{
			Nature::Bold,
			IVs(
				IV(31_bi),
				IV(31_bi),
				IV(31_bi),
				IV(31_bi),
				IV(31_bi),
				IV(31_bi)
			),
			EVs(
				EV(0_bi),
				EV(0_bi),
				EV(252_bi),
				EV(0_bi),
				EV(0_bi),
				EV(0_bi)
			)
		},
		RegularMoves({Move(generation, Moves::Tackle)})
	));
	defender.pokemon().switch_in(weather);

	defender.pokemon().stage()[BoostableStat::def] += 6_bi;

	defender.pokemon().set_status(Statuses::burn, weather);

	check_equal(calculate_defense(defender.pokemon().as_const(), physical_move, weather, false), max_defense);
}

void min_defense_test() {
	std::cout << "\t\tRunning min Defense test.\n";
	constexpr auto min_defense = 1_bi;

	auto weather = Weather();

	auto defender = Team<generation>(max_pokemon_per_team);

	defender.add_pokemon(Pokemon<generation>(
		Species::Combee,
		Level(1_bi),
		Gender::male,
		Item::None,
		Ability::Honey_Gather,
		CombinedStats<generation>{
			Nature::Hasty,
			IVs(
				IV(31_bi),
				IV(31_bi),
				IV(0_bi),
				IV(31_bi),
				IV(31_bi),
				IV(31_bi)
			),
			EVs(
				EV(0_bi),
				EV(0_bi),
				EV(0_bi),
				EV(0_bi),
				EV(0_bi),
				EV(0_bi)
			)
		},
		RegularMoves({Move(generation, Moves::Tackle)})
	));
	auto pokemon = defender.pokemon();

	pokemon.switch_in(weather);

	for (auto const n [[maybe_unused]] : containers::integer_range(3_bi)) {
		pokemon.stage()[BoostableStat::def] += -2_bi;
	}

	check_equal(calculate_defense(defender.pokemon().as_const(), physical_move, weather, false), min_defense);
}

void defense_tests() {
	std::cout << "\tRunning Defense tests.\n";
	max_defense_test();
	min_defense_test();
}

void special_defense_tests() {
	std::cout << "\tRunning Special Defense tests.\n";
	constexpr auto max_special_defense = 3684_bi;

	auto weather = Weather();
	weather.activate_sand_from_move(false);

	auto defender = Team<generation>(max_pokemon_per_team);

	defender.add_pokemon(Pokemon<generation>(
		Species::Shuckle,
		Level(100_bi),
		Gender::male,
		Item::None,
		Ability::Honey_Gather,
		CombinedStats<generation>{
			Nature::Calm,
			IVs(
				IV(31_bi),
				IV(31_bi),
				IV(31_bi),
				IV(31_bi),
				IV(31_bi),
				IV(31_bi)
			),
			EVs(
				EV(0_bi),
				EV(0_bi),
				EV(0_bi),
				EV(0_bi),
				EV(252_bi),
				EV(0_bi)
			)
		},
		RegularMoves({Move(generation, Moves::Tackle)})
	));
	auto pokemon = defender.pokemon();

	pokemon.switch_in(weather);

	pokemon.stage()[BoostableStat::spd] += 6_bi;

	check_equal(calculate_special_defense(defender.pokemon().as_const(), Ability::Honey_Gather, weather, false), max_special_defense);
}

void speed_tests() {
	std::cout << "\tRunning Speed tests.\n";
	constexpr auto max_speed = 12096_bi;
	auto weather = Weather();
	weather.activate_rain_from_move(false);

	auto team = Team<generation>(max_pokemon_per_team);

	team.add_pokemon(Pokemon<generation>(
		Species::Deoxys_Speed,
		Level(100_bi),
		Gender::genderless,
		Item::Choice_Scarf,
		Ability::Swift_Swim,
		CombinedStats<generation>{
			Nature::Timid,
			IVs(
				IV(31_bi),
				IV(31_bi),
				IV(31_bi),
				IV(31_bi),
				IV(31_bi),
				IV(31_bi)
			),
			EVs(
				EV(0_bi),
				EV(0_bi),
				EV(0_bi),
				EV(0_bi),
				EV(0_bi),
				EV(252_bi)
			)
		},
		RegularMoves({Move(generation, Moves::Tackle)})
	));
	auto pokemon = team.pokemon();

	pokemon.switch_in(weather);

	pokemon.stage()[BoostableStat::spe] += 6_bi;

	team.activate_tailwind();
	
	check_equal(calculate_speed(team, Ability::Honey_Gather, weather), max_speed);
}

}	// namespace

void stat_tests() {
	std::cout << "Running stat tests.\n";
	attack_tests();
	special_attack_tests();
	defense_tests();
	special_defense_tests();
	speed_tests();
	std::cout << "Stat tests passed.\n\n";
}

}	// namespace technicalmachine
