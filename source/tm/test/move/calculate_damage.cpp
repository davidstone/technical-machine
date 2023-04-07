// Copyright David Stone 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.test.move.calculate_damage;

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
using namespace bounded::literal;

constexpr auto generation = Generation::four;
constexpr auto resistance_berry_activated = false;

constexpr auto make_executed_move(Move const move, Type const type) {
	constexpr auto critical_hit = true;
	return ExecutedMove<Team<generation>>{
		{move.name(), type},
		move.pp(),
		no_effect_function,
		critical_hit,
		ContactAbilityEffect::nothing
	};
}

namespace max_physical_damage {

constexpr auto move = Move(generation, MoveName::Rollout);

constexpr auto attacker() {
	auto team = Team<generation>({
		Pokemon<generation>(
			Species::Shuckle,
			Level(100_bi),
			Gender::male,
			Item::Metronome,
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
			RegularMoves({move})
		)
	});

	auto pokemon = team.pokemon();
	pokemon.switch_in(Environment());

	pokemon.defense_curl();
	for (auto const _ [[maybe_unused]] : containers::integer_range(10_bi)) {
		pokemon.successfully_use_move(move.name());
	}

	pokemon.activate_power_trick();
	pokemon.stages()[BoostableStat::atk] += 6_bi;

	return team;
}

constexpr auto defender() {
	auto team = Team<generation>({
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
	auto pokemon = team.pokemon();
	pokemon.switch_in(Environment());
	for (auto const n [[maybe_unused]] : containers::integer_range(3_bi)) {
		pokemon.stages()[BoostableStat::def] -= 2_bi;
	}
	return team;
}

constexpr auto calculated_damage = calculate_damage(
	attacker(),
	make_executed_move(move, Type::Rock),
	resistance_berry_activated,
	defender(),
	FutureMove{false},
	Environment()
);
static_assert(calculated_damage == 95064912_bi);

}

namespace max_special_damage {

constexpr auto environment() {
	auto env = Environment();
	env.activate_sun_from_move(false);
	return env;
}

constexpr auto move = Move(generation, MoveName::Blast_Burn);

constexpr auto attacker() {
	auto team = Team<generation>({
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
	auto pokemon = team.pokemon();
	pokemon.switch_in(environment());
	pokemon.set_hp(environment(), 1_bi);

	pokemon.set_type(Type::Fire);

	pokemon.stages()[BoostableStat::spa] += 6_bi;

	for (auto const _ [[maybe_unused]] : containers::integer_range(10_bi)) {
		pokemon.successfully_use_move(move.name());
	}

	pokemon.activate_flash_fire();
	return team;
}

constexpr auto defender() {
	auto team = Team<generation>({
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
	auto pokemon = team.pokemon();
	pokemon.switch_in(environment());
	for (auto const _ [[maybe_unused]] : containers::integer_range(3_bi)) {
		pokemon.stages()[BoostableStat::spd] -= 2_bi;
	}
	return team;
}

constexpr auto calculated_damage = calculate_damage(
	attacker(),
	make_executed_move(move, Type::Fire),
	resistance_berry_activated,
	defender(),
	FutureMove{false},
	environment()
);

static_assert(calculated_damage == 25696272_bi);

}

} // namespace technicalmachine
