// Copyright David Stone 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.test.move.calculate_damage;

import tm.move.calculate_damage;
import tm.move.executed_move;
import tm.move.future_selection;
import tm.move.move;
import tm.move.move_name;
import tm.move.no_effect_function;
import tm.move.power;

import tm.pokemon.level;
import tm.pokemon.species;

import tm.stat.combined_stats;
import tm.stat.ev;
import tm.stat.evs;
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
import tm.weather;

import bounded;
import containers;

namespace technicalmachine {
using namespace bounded::literal;

constexpr auto generation = Generation::four;
constexpr auto resistance_berry_activated = false;

constexpr auto make_executed_move(MoveName const move_name, Type const type) {
	constexpr auto critical_hit = true;
	auto const move = Move(generation, move_name);
	return ExecutedMove<Team<generation>>{
		{move.name(), type},
		move.pp(),
		no_effect_function,
		critical_hit,
		ContactAbilityEffect::nothing
	};
}

namespace max_physical_damage {

constexpr auto move = MoveName::Rollout;

constexpr auto attacker() {
	auto team = Team<generation>({{
		{
			.species = Species::Shuckle,
			.item = Item::Metronome,
			.ability = Ability::Pure_Power,
			.stats = {
				.nature = Nature::Impish,
				.evs = EVs(
					EV(0_bi),
					EV(0_bi),
					EV(252_bi),
					EV(0_bi),
					EV(0_bi),
					EV(0_bi)
				),
			},
			.moves = {{
				move,
			}}
		},
	}});

	auto pokemon = team.pokemon();
	pokemon.switch_in(Environment(), true);

	pokemon.defense_curl();
	for (auto const _ : containers::integer_range(10_bi)) {
		pokemon.successfully_use_move(move, Ability::Honey_Gather, Environment());
	}

	pokemon.activate_power_trick();
	pokemon.stages()[BoostableStat::atk] += 6_bi;

	return team;
}

constexpr auto defender() {
	auto team = Team<generation>({{
		{
			.species = Species::Combee,
			.level = Level(1_bi),
			.moves = {{
				MoveName::Tackle,
			}}
		},
	}});
	auto pokemon = team.pokemon();
	pokemon.switch_in(Environment(), true);
	for (auto const _ : containers::integer_range(3_bi)) {
		pokemon.stages()[BoostableStat::def] -= 2_bi;
	}
	return team;
}

constexpr auto calculated_damage = calculate_damage(
	attacker(),
	make_executed_move(move, Type::Rock),
	resistance_berry_activated,
	defender(),
	FutureSelection(false),
	Environment()
);
static_assert(calculated_damage == 95064912_bi);

}

namespace max_special_damage {

constexpr auto environment() {
	auto env = Environment();
	env.activate_weather_from_move(Weather::sun, false);
	return env;
}

constexpr auto move = MoveName::Blast_Burn;

constexpr auto attacker() {
	auto team = Team<generation>({{
		{
			.species = Species::Deoxys_Attack,
			.item = Item::Metronome,
			.ability = Ability::Blaze,
			.stats = {
				.nature = Nature::Modest,
				.evs = EVs(
					EV(0_bi),
					EV(0_bi),
					EV(0_bi),
					EV(EV::max),
					EV(0_bi),
					EV(0_bi)
				),
			},
			.moves = {{
				move,
			}}
		},
	}});
	auto pokemon = team.pokemon();
	pokemon.switch_in(environment(), true);
	pokemon.set_hp(environment(), 1_bi);

	pokemon.set_type(Type::Fire);

	pokemon.stages()[BoostableStat::spa] += 6_bi;

	for (auto const _ : containers::integer_range(10_bi)) {
		pokemon.successfully_use_move(move, Ability::Honey_Gather, environment());
	}

	pokemon.activate_flash_fire();
	return team;
}

constexpr auto defender() {
	auto team = Team<generation>({{
		{
			.species = Species::Paras,
			.level = Level(1_bi),
			.ability = Ability::Dry_Skin,
			.moves = {{
				MoveName::Tackle,
			}}
		},
	}});
	auto pokemon = team.pokemon();
	pokemon.switch_in(environment(), true);
	for (auto const _ : containers::integer_range(3_bi)) {
		pokemon.stages()[BoostableStat::spd] -= 2_bi;
	}
	return team;
}

constexpr auto calculated_damage = calculate_damage(
	attacker(),
	make_executed_move(move, Type::Fire),
	resistance_berry_activated,
	defender(),
	FutureSelection(false),
	environment()
);

static_assert(calculated_damage == 25696272_bi);

}

} // namespace technicalmachine
