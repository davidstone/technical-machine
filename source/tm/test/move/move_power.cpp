// Copyright David Stone 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.test.move_power;

import tm.move.executed_move;
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

namespace max_physical_power {

constexpr auto move = MoveName::Rollout;

constexpr auto attacker() {
	auto team = Team<generation>({{
		{
			.species = Species::Shuckle,
			.gender = Gender::male,
			.item = Item::Rock_Incense,
			.ability = Ability::Rivalry,
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
	for (auto const _ : containers::integer_range(4_bi)) {
		pokemon.successfully_use_move(
			move,
			move,
			Ability::Honey_Gather,
			Environment()
		);
	}

	return team;
}

constexpr auto defender() {
	auto team = Team<generation>({{
		{
			.species = Species::Combee,
			.level = Level(1_bi),
			.gender = Gender::male,
			.moves = {{
				MoveName::Tackle,
			}}
		},
	}});
	team.pokemon().switch_in(Environment(), true);
	return team;
}

constexpr auto power = move_power(
	attacker(),
	make_executed_move(move, Type::Rock),
	defender(),
	Environment()
);
static_assert(power == 1440_bi);

}

namespace max_special_power {

constexpr auto move = MoveName::Surf;

constexpr auto attacker() {
	auto team = Team<generation>({{
		{
			.species = Species::Deoxys_Attack,
			.item = Item::Wave_Incense,
			.ability = Ability::Torrent,
			.moves = {{
				move,
			}}
		},
	}});
	auto pokemon = team.pokemon();
	auto environment = Environment();
	pokemon.switch_in(environment, true);
	pokemon.set_hp(environment, 1_bi);
	return team;
}

constexpr auto defender() {
	auto team = Team<generation>({{
		{
			.species = Species::Blastoise,
			.level = Level(1_bi),
			.ability = Ability::Torrent,
			.moves = {{
				MoveName::Dive,
			}}
		},
	}});
	auto pokemon = team.pokemon();
	auto environment = Environment();
	pokemon.switch_in(environment, true);
	pokemon.successfully_use_move(
		MoveName::Dive,
		MoveName::Dive,
		Ability::Honey_Gather,
		environment
	);
	return team;
}

constexpr auto power = move_power(
	attacker(),
	make_executed_move(move, Type::Water),
	defender(),
	Environment()
);
static_assert(power == 342_bi);

}

} // namespace technicalmachine
