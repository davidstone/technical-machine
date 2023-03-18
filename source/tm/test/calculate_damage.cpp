// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <compare>
#include <catch2/catch_test_macros.hpp>

import tm.move.calculate_damage;
import tm.move.executed_move;
import tm.move.move;
import tm.move.move_name;
import tm.move.no_effect_function;
import tm.move.other_move;
import tm.move.power;
import tm.move.regular_moves;

import tm.pokemon.level;
import tm.pokemon.pokemon;
import tm.pokemon.species;

import tm.stat.combined_stats;
import tm.stat.ev;
import tm.stat.iv;
import tm.stat.nature;
import tm.stat.stat_names;

import tm.type.type;

import tm.ability;
import tm.contact_ability_effect;
import tm.environment;
import tm.gender;
import tm.generation;
import tm.item;
import tm.team;

import bounded;
import containers;

namespace technicalmachine {
namespace {
using namespace bounded::literal;

constexpr auto generation = Generation::four;
constexpr auto critical_hit = true;
auto max_damage_physical_move() {
	return Move(generation, MoveName::Rollout);
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
	pokemon.switch_in(Environment());

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
			RegularMoves({Move(generation, MoveName::Tackle)})
		)
	});
	auto pokemon = defender.pokemon();
	pokemon.switch_in(Environment());
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
		Environment()
	);
	CHECK(power == max_power);
}

TEST_CASE("Max special power", "[Power]") {
	constexpr auto max_power = 342_bi;

	auto environment = Environment();

	auto const move = Move(generation, MoveName::Surf);
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
	pokemon.switch_in(environment);
	pokemon.set_hp(environment, 1_bi);
	

	auto defender = Team<generation>({
		Pokemon<generation>(
			Species::Paras,
			Level(1_bi),
			Gender::male,
			Item::None,
			Ability::Dry_Skin,
			default_combined_stats<generation>,
			RegularMoves({Move(generation, MoveName::Dive)})
		)
	});

	auto defender_pokemon = defender.pokemon();
	defender_pokemon.switch_in(environment);
	for (auto const n [[maybe_unused]] : containers::integer_range(3_bi)) {
		defender_pokemon.stages()[BoostableStat::spd] -= 2_bi;
	}
	defender_pokemon.successfully_use_move(MoveName::Dive);
	defender_pokemon.use_vanish_move(environment);

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
		environment
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
		Environment()
	);
	CHECK(calculated_damage == max_damage);
}

TEST_CASE("Max special damage", "[Damage]") {
	constexpr auto max_damage = 25696272_bi;
	auto environment = Environment();
	environment.activate_sun_from_move(false);

	auto const move = Move(generation, MoveName::Blast_Burn);

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
	attacker_pokemon.switch_in(environment);
	attacker_pokemon.set_hp(environment, 1_bi);

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
			RegularMoves({Move(generation, MoveName::Tackle)})
		)
	});
	auto defender_pokemon = defender.pokemon();
	defender_pokemon.switch_in(environment);
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
		environment
	);
	CHECK(calculated_damage == max_damage);
}

} // namespace
} // namespace technicalmachine
