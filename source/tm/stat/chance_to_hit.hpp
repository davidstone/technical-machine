// Chance to hit calculations
// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <tm/move/accuracy.hpp>
#include <tm/move/category.hpp>
#include <tm/move/known_move.hpp>

#include <tm/stat/stage.hpp>
#include <tm/stat/stat_names.hpp>

#include <tm/ability.hpp>
#include <tm/generation.hpp>
#include <tm/rational.hpp>
#include <tm/weather.hpp>

#include <bounded/clamp.hpp>
#include <bounded/integer.hpp>

namespace technicalmachine {

template<Generation>
struct ActivePokemon;
template<Generation>
struct MutableActivePokemon;

using ChanceToHit = double;

template<Generation generation>
auto move_can_miss(ActivePokemon<generation> const user, Moves const move, BaseAccuracy const base_accuracy, ActivePokemon<generation> const target) -> bool {
	return
		static_cast<bool>(base_accuracy) and
		!cannot_miss(user.ability()) and
		!cannot_miss(target.ability()) and
		!(move == Moves::Body_Slam and target.minimized()) and
		!user.last_used_move().locked_on();
}

inline auto accuracy_item_modifier(Item const item, bool target_moved) {
	using Modifier = rational<bounded::integer<1, 11>, bounded::integer<1, 10>>;
	switch (item) {
		case Item::Wide_Lens: return Modifier(11_bi, 10_bi);
		case Item::Zoom_Lens: return target_moved ? Modifier(6_bi, 5_bi) : Modifier(1_bi, 1_bi);
		default: return Modifier(1_bi, 1_bi);
	}
}

inline auto ability_accuracy_modifier(Generation const generation, Ability const ability, KnownMove const move) {
	using Modifier = rational<
		bounded::integer<1, 13>,
		bounded::integer<1, 10>
	>;
	switch (ability) {
		case Ability::Compound_Eyes: return Modifier(13_bi, 10_bi);
		case Ability::Hustle: return is_physical(generation, move) ? Modifier(4_bi, 5_bi) : Modifier(1_bi, 1_bi);
		default: return Modifier(1_bi, 1_bi);
	}
}

inline auto evasion_item_modifier(Generation const generation, Item const item) {
	using Modifier = rational<bounded::integer<1, 19>, bounded::integer<1, 64>>;
	switch (item) {
		case Item::Bright_Powder:
			return generation <= Generation::two ? Modifier(5_bi, 64_bi) : Modifier(9_bi, 10_bi);
		case Item::Lax_Incense:
			return generation <= Generation::three ? Modifier(19_bi, 20_bi) : Modifier(9_bi, 10_bi);
		default:
			return Modifier(1_bi, 1_bi);
	}
}

template<Generation generation>
auto ability_evasion_modifier(ActivePokemon<generation> const target, Weather const weather, bool const blocks_weather) {
	using Modifier = rational<
		bounded::integer<1, 4>,
		bounded::integer<1, 5>
	>;
	auto const target_ability = target.ability();
	switch (target_ability) {
		case Ability::Sand_Veil: return weather.sand(blocks_weather) ? Modifier(4_bi, 5_bi) : Modifier(1_bi, 1_bi);
		case Ability::Snow_Cloak: return weather.hail(blocks_weather) ? Modifier(4_bi, 5_bi) : Modifier(1_bi, 1_bi);
		case Ability::Tangled_Feet: return target.is_confused() ? Modifier(1_bi, 2_bi) : Modifier(1_bi, 1_bi);
		default: return Modifier(1_bi, 1_bi);
	}
}

template<Generation generation>
auto chance_to_hit(ActivePokemon<generation> const user, KnownMove const move, ActivePokemon<generation> const target, Weather const weather, bool target_moved) -> ChanceToHit {
	auto const user_ability = user.ability();
	auto const target_ability = target.ability();
	auto const blocks_weather = weather_is_blocked_by_ability(target_ability, user_ability);
	auto const base_accuracy = accuracy(generation, move.name, weather, blocks_weather, is_type(user, Type::Poison));
	if (!move_can_miss(user, move.name, base_accuracy, target)) {
		return 1.0;
	}
	constexpr auto gravity_denominator = 3_bi;
	auto const gravity_numerator = BOUNDED_CONDITIONAL(weather.gravity(), 5_bi, gravity_denominator);
	auto const gravity_multiplier = rational(gravity_numerator, gravity_denominator);
	auto const calculated_accuracy = *base_accuracy *
		modifier<BoostableStat::acc>(user.stage()) *
		modifier<BoostableStat::eva>(generation >= Generation::six and target_ability == Ability::Keen_Eye ? Stage() : target.stage()) *
		accuracy_item_modifier(user.item(weather), target_moved) *
		ability_accuracy_modifier(generation, user_ability, move) *
		evasion_item_modifier(generation, target.item(weather)) *
		ability_evasion_modifier(target, weather, blocks_weather) *
		gravity_multiplier
	;
	
	constexpr auto max = 100_bi;
	return static_cast<double>(bounded::clamp(calculated_accuracy, 0_bi, max)) / static_cast<double>(max);
}

} // namespace technicalmachine
