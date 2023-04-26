// Copyright David Stone 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.test.stat.calculate;

import tm.move.move;
import tm.move.move_name;
import tm.move.regular_moves;

import tm.pokemon.active_pokemon;
import tm.pokemon.level;
import tm.pokemon.pokemon;
import tm.pokemon.species;

import tm.stat.calculate;
import tm.stat.combined_stats;
import tm.stat.ev;
import tm.stat.iv;
import tm.stat.nature;
import tm.stat.stat_names;

import tm.status.status_name;

import tm.type.type;

import tm.ability;
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

constexpr auto critical_hit = false;
constexpr auto physical_move = MoveName::Tackle;

// Calculate max Attack
static_assert([]{
	constexpr auto environment = Environment();

	auto flags = ActivePokemonFlags<generation>();
	auto pokemon = Pokemon<generation>(
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
		RegularMoves({Move(generation, MoveName::Tackle)})
	);
	auto active_pokemon = MutableActivePokemon<generation>(pokemon, flags);

	active_pokemon.switch_in(environment);

	active_pokemon.activate_power_trick();
	active_pokemon.stages()[BoostableStat::atk] += 6_bi;

	return calculate_attack(active_pokemon.as_const(), Type::Normal, Ability::Honey_Gather, environment, critical_hit) == 7368_bi;
}());

// Calculate max Special Attack
static_assert([]{
	auto environment = Environment();
	environment.activate_sun_from_move(false);

	auto flags = ActivePokemonFlags<generation>();
	auto pokemon = Pokemon<generation>(
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
		RegularMoves({Move(generation, MoveName::Psychic)})
	);
	auto active_pokemon = MutableActivePokemon<generation>(pokemon, flags);

	active_pokemon.switch_in(environment);

	active_pokemon.stages()[BoostableStat::spa] += 6_bi;

	return calculate_special_attack(active_pokemon.as_const(), Type::Water, Ability::Honey_Gather, environment, critical_hit) == 4536_bi;
}());

// Calculate max Defense
static_assert([]{
	constexpr auto environment = Environment();

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
			RegularMoves({Move(generation, MoveName::Tackle)})
		)
	});
	defender.pokemon().switch_in(environment);

	defender.pokemon().stages()[BoostableStat::def] += 6_bi;

	defender.pokemon().set_status(StatusName::burn, environment);

	return calculate_defense(defender.pokemon().as_const(), physical_move, environment, false) == 3684_bi;
}());

// Calculate min Defense
static_assert([]{
	constexpr auto environment = Environment();

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

	pokemon.switch_in(environment);

	for (auto const n [[maybe_unused]] : containers::integer_range(3_bi)) {
		pokemon.stages()[BoostableStat::def] -= 2_bi;
	}

	return calculate_defense(defender.pokemon().as_const(), physical_move, environment, false) == 1_bi;
}());

// Calculate max Special Defense
static_assert([]{
	auto environment = Environment();
	environment.activate_sand_from_move(false);

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
			RegularMoves({Move(generation, MoveName::Tackle)})
		)
	});
	auto pokemon = defender.pokemon();

	pokemon.switch_in(environment);

	pokemon.stages()[BoostableStat::spd] += 6_bi;

	return calculate_special_defense(defender.pokemon().as_const(), Ability::Honey_Gather, environment, false) == 3684_bi;
}());

// Calculate max Speed
static_assert([]{
	auto environment = Environment();
	environment.activate_rain_from_move(false);

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
			RegularMoves({Move(generation, MoveName::Tackle)})
		)
	});
	auto pokemon = team.pokemon();

	pokemon.switch_in(environment);

	pokemon.stages()[BoostableStat::spe] += 6_bi;

	team.activate_tailwind();

	return calculate_speed(team, Ability::Honey_Gather, environment) == max_speed;
}());

} // namespace
} // namespace technicalmachine