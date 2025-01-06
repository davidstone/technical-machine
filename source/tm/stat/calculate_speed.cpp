// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

module;

#include <bounded/conditional.hpp>

export module tm.stat.calculate_speed;

import tm.pokemon.species;

import tm.stat.stage;
import tm.stat.stat_names;

import tm.status.status_name;

import tm.type.type;

import tm.ability;
import tm.any_team;
import tm.environment;
import tm.generation;
import tm.item;
import tm.rational;
import tm.weather;

import bounded;
import std_module;

namespace technicalmachine {
using namespace bounded::literal;

constexpr auto is_boosted_by_quick_powder(Species const species) {
	return species == Species::Ditto;
}

constexpr auto speed_ability_modifier(
	Ability const ability,
	StatusName const status,
	bool const is_unburdened,
	bool const slow_start_is_active,
	Weather const weather
) {
	constexpr auto denominator = 2_bi;
	auto const numerator = [&] -> bounded::integer<1, 4> {
		switch (ability) {
			case Ability::Chlorophyll: return BOUNDED_CONDITIONAL(
				weather == Weather::sun,
				denominator * 2_bi,
				denominator
			);
			case Ability::Swift_Swim: return BOUNDED_CONDITIONAL(
				weather == Weather::rain,
				denominator * 2_bi,
				denominator
			);
			case Ability::Unburden: return BOUNDED_CONDITIONAL(
				is_unburdened,
				denominator * 2_bi,
				denominator
			);
			case Ability::Quick_Feet: return BOUNDED_CONDITIONAL(
				status != StatusName::clear,
				denominator * 3_bi / 2_bi,
				denominator
			);
			case Ability::Slow_Start: return BOUNDED_CONDITIONAL(
				slow_start_is_active,
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

constexpr auto stat_name = SplitSpecialRegularStat::spe;

constexpr auto lowers_speed(StatusName const status, Ability const ability) -> bool {
	return status == StatusName::paralysis and !blocks_paralysis_speed_penalty(ability);
}

export constexpr auto max_speed = 13152_bi;

export constexpr auto calculate_speed(
	auto const initial,
	Species const species,
	Item const item,
	Ability const ability,
	StatusName const status,
	Stages const stages,
	bool const is_unburdened,
	bool const slow_start_is_active,
	bool const tailwind,
	Weather const weather
) {
	auto const paralysis_divisor = BOUNDED_CONDITIONAL(lowers_speed(status, ability), 4_bi, 1_bi);
	auto const tailwind_multiplier = BOUNDED_CONDITIONAL(tailwind, 2_bi, 1_bi);
	auto const speed =
		initial *
		modifier<BoostableStat(stat_name)>(stages) *
		speed_ability_modifier(
			ability,
			status,
			is_unburdened,
			slow_start_is_active,
			weather
		) *
		item_modifier(species, item) /
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

export constexpr auto calculate_speed(any_team auto const & team, Ability const other_ability, Environment const environment) {
	auto const pokemon = team.pokemon();
	return calculate_speed(
		pokemon.stats().spe,
		pokemon.species(),
		pokemon.item(environment),
		pokemon.ability(),
		pokemon.status().name(),
		pokemon.stages(),
		pokemon.is_unburdened(),
		pokemon.slow_start_is_active(),
		static_cast<bool>(team.tailwind()),
		environment.effective_weather(pokemon.ability(), other_ability)
	);
}

} // namespace technicalmachine
