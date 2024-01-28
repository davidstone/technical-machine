// Copyright David Stone 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.test.stat.calculate;

import tm.move.move_name;

import tm.pokemon.active_pokemon;
import tm.pokemon.level;
import tm.pokemon.pokemon;
import tm.pokemon.species;

import tm.stat.calculate;
import tm.stat.ev;
import tm.stat.evs;
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

constexpr auto is_self_ko = false;
constexpr auto critical_hit = false;

// Calculate max Attack
static_assert([]{
	constexpr auto environment = Environment();

	auto flags = ActivePokemonFlags<generation>();
	auto pokemon = Pokemon<generation>({
		.species = Species::Shuckle,
		.item = Item::Choice_Band,
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
			MoveName::Tackle,
		}}
	});
	auto active_pokemon = MutableActivePokemon<generation>(pokemon, flags);

	active_pokemon.switch_in(environment, true);

	active_pokemon.activate_power_trick();
	active_pokemon.stages()[BoostableStat::atk] += 6_bi;

	return calculate_attack(active_pokemon.as_const(), Type::Normal, Ability::Honey_Gather, environment, critical_hit) == 7368_bi;
}());

// Calculate max Special Attack
static_assert([]{
	auto environment = Environment();
	environment.activate_sun_from_move(false);

	auto flags = ActivePokemonFlags<generation>();
	auto pokemon = Pokemon<generation>({
		.species = Species::Deoxys_Attack,
		.item = Item::Choice_Specs,
		.ability = Ability::Solar_Power,
		.stats = {
			.nature = Nature::Modest,
			.evs = EVs(
				EV(0_bi),
				EV(0_bi),
				EV(0_bi),
				EV(252_bi),
				EV(0_bi),
				EV(0_bi)
			),
		},
		.moves = {{
			MoveName::Psychic,
		}}
	});
	auto active_pokemon = MutableActivePokemon<generation>(pokemon, flags);

	active_pokemon.switch_in(environment, true);

	active_pokemon.stages()[BoostableStat::spa] += 6_bi;

	return calculate_special_attack(active_pokemon.as_const(), Type::Water, Ability::Honey_Gather, environment, critical_hit) == 4536_bi;
}());

// Calculate max Defense
static_assert([]{
	constexpr auto environment = Environment();

	auto defender = Team<generation>({{
		{
			.species = Species::Shuckle,
			.ability = Ability::Marvel_Scale,
			.stats = {
				.nature = Nature::Bold,
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
				MoveName::Tackle,
			}}
		},
	}});
	defender.pokemon().switch_in(environment, true);

	defender.pokemon().stages()[BoostableStat::def] += 6_bi;

	defender.pokemon().set_status(StatusName::burn, environment);

	return calculate_defense(defender.pokemon().as_const(), environment, is_self_ko, critical_hit) == 3684_bi;
}());

// Calculate min Defense
static_assert([]{
	constexpr auto environment = Environment();

	auto defender = Team<generation>({{
		{
			.species = Species::Combee,
			.level = Level(1_bi),
			.moves = {{
				MoveName::Tackle,
			}}
		},
	}});
	auto pokemon = defender.pokemon();

	pokemon.switch_in(environment, true);

	for (auto const _ : containers::integer_range(3_bi)) {
		pokemon.stages()[BoostableStat::def] -= 2_bi;
	}

	return calculate_defense(defender.pokemon().as_const(), environment, is_self_ko, critical_hit) == 1_bi;
}());

// Calculate max Special Defense
static_assert([]{
	auto environment = Environment();
	environment.activate_sand_from_move(false);

	auto defender = Team<generation>({{
		{
			.species = Species::Shuckle,
			.stats = {
				.nature = Nature::Calm,
				.evs = EVs(
					EV(0_bi),
					EV(0_bi),
					EV(0_bi),
					EV(0_bi),
					EV(252_bi),
					EV(0_bi)
				),
			},
			.moves = {{
				MoveName::Tackle,
			}}
		},
	}});
	auto pokemon = defender.pokemon();

	pokemon.switch_in(environment, true);

	pokemon.stages()[BoostableStat::spd] += 6_bi;

	return calculate_special_defense(defender.pokemon().as_const(), Ability::Honey_Gather, environment, false) == 3684_bi;
}());

} // namespace
} // namespace technicalmachine
