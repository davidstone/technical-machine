// Copyright David Stone 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.test.stat.calculate_speed;

import tm.move.move_name;

import tm.pokemon.species;

import tm.stat.calculate_speed;
import tm.stat.ev;
import tm.stat.evs;
import tm.stat.nature;
import tm.stat.stat_names;

import tm.ability;
import tm.environment;
import tm.generation;
import tm.item;
import tm.team;
import tm.weather;

import bounded;

namespace technicalmachine {
using namespace bounded::literal;

constexpr auto generation = Generation::four;

// Calculate max Speed
static_assert([]{
	auto environment = Environment();
	environment.activate_weather_from_move(Weather::rain, false);

	auto team = Team<generation>({{
		{
			.species = Species::Regieleki,
			.item = Item::Choice_Scarf,
			.ability = Ability::Swift_Swim,
			.stats = {
				.nature = Nature::Timid,
				.evs = EVs(
					EV(0_bi),
					EV(0_bi),
					EV(0_bi),
					EV(0_bi),
					EV(0_bi),
					EV(252_bi)
				),
			},
			.moves = {{ MoveName::Tackle }},
		},
	}});
	auto pokemon = team.pokemon();

	pokemon.switch_in(environment, true);

	pokemon.stages()[BoostableStat::spe] += 6_bi;

	team.activate_tailwind();

	return calculate_speed(team, Ability::Honey_Gather, environment) == max_speed;
}());

} // namespace technicalmachine
