// Test stat calculations
// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <tm/pokemon/max_pokemon_per_team.hpp>
#include <tm/pokemon/species.hpp>

#include <tm/stat/calculate.hpp>
#include <tm/stat/ev.hpp>
#include <tm/stat/iv.hpp>

#include <tm/team.hpp>
#include <tm/weather.hpp>

#include <containers/integer_range.hpp>

#include <catch2/catch_test_macros.hpp>

namespace technicalmachine {
namespace {
using namespace bounded::literal;

constexpr auto generation = Generation::four;

constexpr auto critical_hit = false;
constexpr auto physical_move = Moves::Tackle;

TEST_CASE("Calculate max Attack", "[calculate stat]") {
	constexpr auto max_attack = 7368_bi;

	auto weather = Weather();

	auto attacker = Team<generation>({
		Pokemon<generation>(
			Species::Shuckle,
			Level(100_bi),
			Gender::male,
			Item::Choice_Band,
			Ability::Pure_Power,
			CombinedStats<generation>{
				Nature::Impish,
				max_dvs_or_ivs<generation>,
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
		)
	});
	auto pokemon = attacker.pokemon();

	pokemon.switch_in(weather);

	pokemon.activate_power_trick();
	pokemon.stages()[BoostableStat::atk] += 6_bi;

	CHECK(calculate_attack(attacker.pokemon().as_const(), Type::Normal, Ability::Honey_Gather, weather, critical_hit) == max_attack);
}

TEST_CASE("Calculate max Special Attack", "[calculate stat]") {
	constexpr auto max_special_attack = 4536_bi;
	auto weather = Weather();
	weather.activate_sun_from_move(false);
	auto attacker = Team<generation>({
		Pokemon<generation>(
			Species::Deoxys_Attack,
			Level(100_bi),
			Gender::genderless,
			Item::Choice_Specs,
			Ability::Solar_Power,
			CombinedStats<generation>{
				Nature::Modest,
				max_dvs_or_ivs<generation>,
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
		)
	});
	auto pokemon = attacker.pokemon();

	pokemon.switch_in(weather);

	pokemon.stages()[BoostableStat::spa] += 6_bi;

	CHECK(calculate_special_attack(attacker.pokemon().as_const(), Type::Water, Ability::Honey_Gather, weather, critical_hit) == max_special_attack);
}

TEST_CASE("Calculate max Defense", "[calculate stat]") {
	constexpr auto max_defense = 3684_bi;

	constexpr auto weather = Weather();

	auto defender = Team<generation>({
		Pokemon<generation>(
			Species::Shuckle,
			Level(100_bi),
			Gender::male,
			Item::None,
			Ability::Marvel_Scale,
			CombinedStats<generation>{
				Nature::Bold,
				max_dvs_or_ivs<generation>,
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
		)
	});
	defender.pokemon().switch_in(weather);

	defender.pokemon().stages()[BoostableStat::def] += 6_bi;

	defender.pokemon().set_status(StatusName::burn, weather);

	CHECK(calculate_defense(defender.pokemon().as_const(), physical_move, weather, false) == max_defense);
}

TEST_CASE("Calculate min Defense", "[calculate stat]") {
	constexpr auto min_defense = 1_bi;

	auto weather = Weather();

	auto defender = Team<generation>({
		Pokemon<generation>(
			Species::Combee,
			Level(1_bi),
			Gender::male,
			Item::None,
			Ability::Honey_Gather,
			default_combined_stats<generation>,
			RegularMoves({Move(generation, Moves::Tackle)})
		)
	});
	auto pokemon = defender.pokemon();

	pokemon.switch_in(weather);

	for (auto const n [[maybe_unused]] : containers::integer_range(3_bi)) {
		pokemon.stages()[BoostableStat::def] -= 2_bi;
	}

	CHECK(calculate_defense(defender.pokemon().as_const(), physical_move, weather, false) == min_defense);
}

TEST_CASE("Calculate max Special Defense", "[calculate stat]") {
	constexpr auto max_special_defense = 3684_bi;

	auto weather = Weather();
	weather.activate_sand_from_move(false);

	auto defender = Team<generation>({
		Pokemon<generation>(
			Species::Shuckle,
			Level(100_bi),
			Gender::male,
			Item::None,
			Ability::Honey_Gather,
			CombinedStats<generation>{
				Nature::Calm,
				max_dvs_or_ivs<generation>,
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
		)
	});
	auto pokemon = defender.pokemon();

	pokemon.switch_in(weather);

	pokemon.stages()[BoostableStat::spd] += 6_bi;

	CHECK(calculate_special_defense(defender.pokemon().as_const(), Ability::Honey_Gather, weather, false) == max_special_defense);
}

TEST_CASE("Calculate max Speed", "[calculate stat]") {
	auto weather = Weather();
	weather.activate_rain_from_move(false);

	auto team = Team<generation>({
		Pokemon<generation>(
			Species::Deoxys_Speed,
			Level(100_bi),
			Gender::genderless,
			Item::Choice_Scarf,
			Ability::Swift_Swim,
			CombinedStats<generation>{
				Nature::Timid,
				max_dvs_or_ivs<generation>,
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
		)
	});
	auto pokemon = team.pokemon();

	pokemon.switch_in(weather);

	pokemon.stages()[BoostableStat::spe] += 6_bi;

	team.activate_tailwind();

	CHECK(calculate_speed(team, Ability::Honey_Gather, weather) == max_speed);
}

} // namespace
} // namespace technicalmachine
