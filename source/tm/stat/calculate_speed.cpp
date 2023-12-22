// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

module;

#include <bounded/conditional.hpp>

export module tm.stat.calculate_speed;

import tm.stat.stat_names;

import tm.move.category;
import tm.move.move_name;

import tm.pokemon.any_pokemon;
import tm.pokemon.hp_ratio;
import tm.pokemon.is_type;
import tm.pokemon.species;

import tm.stat.stage;

import tm.status.status;
import tm.status.status_name;

import tm.type.type;

import tm.ability;
import tm.ability_blocks_weather;
import tm.any_team;
import tm.environment;
import tm.generation;
import tm.item;
import tm.rational;

import bounded;
import std_module;

namespace technicalmachine {
using namespace bounded::literal;

constexpr auto is_boosted_by_quick_powder(Species const species) {
	return species == Species::Ditto;
}

constexpr auto speed_ability_modifier(any_active_pokemon auto const pokemon, Ability const other_ability, Environment const environment) {
	constexpr auto denominator = 2_bi;
	auto const numerator = [&]() -> bounded::integer<1, 4> {
		auto const ability = pokemon.ability();
		switch (ability) {
			case Ability::Chlorophyll: return BOUNDED_CONDITIONAL(
				environment.sun() and !ability_blocks_weather(ability, other_ability),
				denominator * 2_bi,
				denominator
			);
			case Ability::Swift_Swim: return BOUNDED_CONDITIONAL(
				environment.rain() and !ability_blocks_weather(ability, other_ability),
				denominator * 2_bi,
				denominator
			);
			case Ability::Unburden: return BOUNDED_CONDITIONAL(
				pokemon.is_unburdened(),
				denominator * 2_bi,
				denominator
			);
			case Ability::Quick_Feet: return BOUNDED_CONDITIONAL(
				!is_clear(pokemon.status()),
				denominator * 3_bi / 2_bi,
				denominator
			);
			case Ability::Slow_Start: return BOUNDED_CONDITIONAL(
				pokemon.slow_start_is_active(),
				denominator * 1_bi / 2_bi,
				denominator
			);
			default: return denominator;
		}
	}();
	return rational(numerator, denominator);
}

constexpr auto item_modifier(Species const species, Item const item) {
	constexpr auto denominator = 2_bi;
	auto const numerator = [&] -> bounded::integer<1, 4> {
		switch (item) {
			case Item::Quick_Powder:
				return BOUNDED_CONDITIONAL(is_boosted_by_quick_powder(species), 2_bi * denominator, denominator);
			case Item::Choice_Scarf:
				return 3_bi;
			case Item::Iron_Ball:
			case Item::Macho_Brace:
			case Item::Power_Anklet:
			case Item::Power_Band:
			case Item::Power_Belt:
			case Item::Power_Bracer:
			case Item::Power_Lens:
			case Item::Power_Weight:
				return 1_bi;
			default:
				return denominator;
		}
	}();
	return rational(numerator, denominator);
}

export constexpr auto max_speed = 13152_bi;
export constexpr auto calculate_speed(any_team auto const & team, Ability const other_ability, Environment const environment) {
	constexpr auto stat = SplitSpecialRegularStat::spe;
	auto const & pokemon = team.pokemon();
	auto const paralysis_divisor = BOUNDED_CONDITIONAL(lowers_speed(pokemon.status(), pokemon.ability()), 4_bi, 1_bi);
	auto const tailwind_multiplier = BOUNDED_CONDITIONAL(team.tailwind(), 2_bi, 1_bi);
	auto const speed =
		pokemon.stat(stat) *
		modifier<BoostableStat(stat)>(pokemon.stages()) *
		speed_ability_modifier(pokemon, other_ability, environment) *
		item_modifier(pokemon.species(), pokemon.item(environment)) /
		paralysis_divisor *
		tailwind_multiplier;

	// Cast here because it looks as though the fastest Pokemon would hold Quick
	// Powder, but because of the restriction on the Pokemon being Ditto, it is
	// better to use a Regieleki with a Choice Scarf.
	return bounded::assume_in_range(
		bounded::max(speed, 1_bi),
		1_bi,
		max_speed
	);
}

} // namespace technicalmachine
