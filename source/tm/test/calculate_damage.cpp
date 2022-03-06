// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <tm/move/calculate_damage.hpp>
#include <tm/move/moves.hpp>
#include <tm/move/other_move.hpp>
#include <tm/move/power.hpp>
#include <tm/move/side_effects.hpp>

#include <tm/pokemon/species.hpp>

#include <tm/stat/ev.hpp>
#include <tm/stat/iv.hpp>

#include <tm/generation.hpp>
#include <tm/team.hpp>
#include <tm/weather.hpp>

#include <containers/integer_range.hpp>

#include <catch2/catch_test_macros.hpp>

namespace technicalmachine {
namespace {
using namespace bounded::literal;

constexpr auto generation = Generation::four;
constexpr auto critical_hit = true;
auto max_damage_physical_move() {
	return Move(generation, Moves::Rollout);
}
constexpr auto max_damage_physical_move_type = Type::Rock;
auto max_damage_executed_physical_move() {
	return ExecutedMove<Team<generation>>{
		{max_damage_physical_move().name(), max_damage_physical_move_type},
		max_damage_physical_move().pp(),
		no_effect_function,
		critical_hit,
		ContactAbilityEffect::nothing
	};
}

auto max_damage_physical_attacker(Item const item, Ability const ability) {
	auto attacker = Team<generation>({
		Pokemon<generation>(
			Species::Shuckle,
			Level(100_bi),
			Gender::male,
			item,
			ability,
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
			RegularMoves({max_damage_physical_move()})
		)
	});

	auto pokemon = attacker.pokemon();
	pokemon.switch_in(Weather());

	pokemon.defense_curl();
	for (auto const n [[maybe_unused]] : containers::integer_range(10_bi)) {
		pokemon.successfully_use_move(max_damage_physical_move().name());
	}

	return attacker;
}

auto max_damage_physical_defender() {
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
	pokemon.switch_in(Weather());
	for (auto const n [[maybe_unused]] : containers::integer_range(3_bi)) {
		pokemon.stages()[BoostableStat::def] -= 2_bi;
	}
	return defender;
}

TEST_CASE("Max physical power", "[Power]") {
	constexpr auto max_power = 1440_bi;

	auto const attacker = max_damage_physical_attacker(Item::Rock_Incense, Ability::Rivalry);

	auto const power = move_power(
		attacker,
		max_damage_executed_physical_move(),
		max_damage_physical_defender(),
		Weather()
	);
	CHECK(power == max_power);
}

TEST_CASE("Max special power", "[Power]") {
	constexpr auto max_power = 342_bi;

	auto weather = Weather();

	auto const move = Move(generation, Moves::Surf);
	auto attacker = Team<generation>({
		Pokemon<generation>(
			Species::Deoxys_Attack,
			Level(100_bi),
			Gender::genderless,
			Item::Wave_Incense,
			Ability::Torrent,
			default_combined_stats<generation>,
			RegularMoves({move})
		)
	});
	auto pokemon = attacker.pokemon();
	pokemon.switch_in(weather);
	pokemon.set_hp(weather, 1_bi);
	

	auto defender = Team<generation>({
		Pokemon<generation>(
			Species::Paras,
			Level(1_bi),
			Gender::male,
			Item::None,
			Ability::Dry_Skin,
			default_combined_stats<generation>,
			RegularMoves({Move(generation, Moves::Dive)})
		)
	});

	auto defender_pokemon = defender.pokemon();
	defender_pokemon.switch_in(weather);
	for (auto const n [[maybe_unused]] : containers::integer_range(3_bi)) {
		defender_pokemon.stages()[BoostableStat::spd] -= 2_bi;
	}
	defender_pokemon.successfully_use_move(Moves::Dive);
	defender_pokemon.use_vanish_move(weather);

	auto const power = move_power(
		attacker,
		ExecutedMove<Team<generation>>{
			{move.name(), Type::Water},
			move.pp(),
			no_effect_function,
			critical_hit,
			ContactAbilityEffect::nothing
		},
		defender,
		weather
	);
	CHECK(power == max_power);
}

constexpr auto resistance_berry_activated = false;

TEST_CASE("Max physical damage", "[Damage]") {
	constexpr auto max_damage = 95064912_bi;

	auto attacker = max_damage_physical_attacker(Item::Metronome, Ability::Pure_Power);

	auto attacker_pokemon = attacker.pokemon();

	attacker_pokemon.activate_power_trick();
	attacker_pokemon.stages()[BoostableStat::atk] += 6_bi;

	auto const defender = max_damage_physical_defender();

	auto const calculated_damage = calculate_damage(
		attacker,
		max_damage_executed_physical_move(),
		resistance_berry_activated,
		defender,
		FutureMove{false},
		Weather()
	);
	CHECK(calculated_damage == max_damage);
}

TEST_CASE("Max special damage", "[Damage]") {
	constexpr auto max_damage = 25696272_bi;
	auto weather = Weather();
	weather.activate_sun_from_move(false);

	auto const move = Move(generation, Moves::Blast_Burn);

	auto attacker = Team<generation>({
		Pokemon<generation>(
			Species::Deoxys_Attack,
			Level(100_bi),
			Gender::genderless,
			Item::Metronome,
			Ability::Blaze,
			CombinedStats<generation>{
				Nature::Modest,
				max_dvs_or_ivs<generation>,
				EVs(
					EV(0_bi),
					EV(0_bi),
					EV(0_bi),
					EV(EV::max),
					EV(0_bi),
					EV(0_bi)
				)
			},
			RegularMoves({move})
		)
	});

	auto attacker_pokemon = attacker.pokemon();
	attacker_pokemon.switch_in(weather);
	attacker_pokemon.set_hp(weather, 1_bi);

	attacker_pokemon.set_type(Type::Fire);

	attacker_pokemon.stages()[BoostableStat::spa] += 6_bi;

	for (auto const n [[maybe_unused]] : containers::integer_range(10_bi)) {
		attacker_pokemon.successfully_use_move(move.name());
	}

	attacker_pokemon.activate_flash_fire();

	auto defender = Team<generation>({
		Pokemon<generation>(
			Species::Paras,
			Level(1_bi),
			Gender::male,
			Item::None,
			Ability::Dry_Skin,
			default_combined_stats<generation>,
			RegularMoves({Move(generation, Moves::Tackle)})
		)
	});
	auto defender_pokemon = defender.pokemon();
	defender_pokemon.switch_in(weather);
	for (auto const n [[maybe_unused]] : containers::integer_range(3_bi)) {
		defender_pokemon.stages()[BoostableStat::spd] -= 2_bi;
	}

	auto const calculated_damage = calculate_damage(
		attacker,
		ExecutedMove<Team<generation>>{
			{move.name(), Type::Fire},
			move.pp(),
			no_effect_function,
			critical_hit,
			ContactAbilityEffect::nothing
		},
		resistance_berry_activated,
		defender,
		FutureMove{false},
		weather
	);
	CHECK(calculated_damage == max_damage);
}

} // namespace
} // namespace technicalmachine
