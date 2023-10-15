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
import tm.move.regular_moves;

import tm.pokemon.level;
import tm.pokemon.pokemon;
import tm.pokemon.species;

import tm.stat.combined_stats;
import tm.stat.default_evs;
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

namespace max_physical_power {

constexpr auto move = Move(generation, MoveName::Rollout);

constexpr auto attacker() {
	auto team = Team<generation>({
		Pokemon<generation>(
			Species::Shuckle,
			Level(100_bi),
			Gender::male,
			Item::Rock_Incense,
			Ability::Rivalry,
			CombinedStatsFor<generation>{
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
	for (auto const _ [[maybe_unused]] : containers::integer_range(4_bi)) {
		pokemon.successfully_use_move(move.name());
	}

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

constexpr auto move = Move(generation, MoveName::Surf);

constexpr auto attacker() {
	auto team = Team<generation>({
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
	auto pokemon = team.pokemon();
	auto environment = Environment();
	pokemon.switch_in(environment);
	pokemon.set_hp(environment, 1_bi);
	return team;
}

constexpr auto defender() {
	auto team = Team<generation>({
		Pokemon<generation>(
			Species::Blastoise,
			Level(1_bi),
			Gender::male,
			Item::None,
			Ability::Torrent,
			default_combined_stats<generation>,
			RegularMoves({Move(generation, MoveName::Dive)})
		)
	});
	auto pokemon = team.pokemon();
	auto environment = Environment();
	pokemon.switch_in(environment);
	pokemon.successfully_use_move(MoveName::Dive);
	pokemon.use_vanish_move(environment);
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
