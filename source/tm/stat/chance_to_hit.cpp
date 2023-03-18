// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

module;

#include <bounded/conditional.hpp>

export module tm.stat.chance_to_hit;

import tm.move.accuracy;
import tm.move.category;
import tm.move.known_move;
import tm.move.move_name;

import tm.pokemon.any_pokemon;
import tm.pokemon.is_type;

import tm.stat.stage;
import tm.stat.stat_names;

import tm.type.type;

import tm.ability;
import tm.ability_blocks_weather;
import tm.environment;
import tm.generation;
import tm.item;
import tm.rational;

import bounded;

namespace technicalmachine {
using namespace bounded::literal;

export using ChanceToHit = double;

template<any_active_pokemon ActivePokemonType>
auto move_can_miss(ActivePokemonType const user, MoveName const move, BaseAccuracy const base_accuracy, ActivePokemonType const target) -> bool {
	return
		static_cast<bool>(base_accuracy) and
		!cannot_miss(user.ability()) and
		!cannot_miss(target.ability()) and
		!(move == MoveName::Body_Slam and target.minimized()) and
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

auto ability_evasion_modifier(any_active_pokemon auto const target, Environment const environment, bool const weather_is_blocked) {
	using Modifier = rational<
		bounded::integer<1, 4>,
		bounded::integer<1, 5>
	>;
	auto const target_ability = target.ability();
	switch (target_ability) {
		case Ability::Sand_Veil: return environment.sand() and !weather_is_blocked ? Modifier(4_bi, 5_bi) : Modifier(1_bi, 1_bi);
		case Ability::Snow_Cloak: return environment.hail() and !weather_is_blocked ? Modifier(4_bi, 5_bi) : Modifier(1_bi, 1_bi);
		case Ability::Tangled_Feet: return target.is_confused() ? Modifier(1_bi, 2_bi) : Modifier(1_bi, 1_bi);
		default: return Modifier(1_bi, 1_bi);
	}
}

export template<any_active_pokemon ActivePokemonType>
auto chance_to_hit(ActivePokemonType const user, KnownMove const move, ActivePokemonType const target, Environment const environment, bool target_moved) -> ChanceToHit {
	constexpr auto generation = generation_from<ActivePokemonType>;
	auto const user_ability = user.ability();
	auto const target_ability = target.ability();
	auto const weather_is_blocked = ability_blocks_weather(target_ability, user_ability);
	auto const base_accuracy = accuracy(generation, move.name, environment, weather_is_blocked, is_type(user, Type::Poison));
	if (!move_can_miss(user, move.name, base_accuracy, target)) {
		return 1.0;
	}
	constexpr auto gravity_denominator = 3_bi;
	auto const gravity_numerator = BOUNDED_CONDITIONAL(environment.gravity(), 5_bi, gravity_denominator);
	auto const gravity_multiplier = rational(gravity_numerator, gravity_denominator);
	auto const calculated_accuracy = *base_accuracy *
		modifier<BoostableStat::acc>(user.stages()) *
		modifier<BoostableStat::eva>(generation >= Generation::six and target_ability == Ability::Keen_Eye ? Stages() : target.stages()) *
		accuracy_item_modifier(user.item(environment), target_moved) *
		ability_accuracy_modifier(generation, user_ability, move) *
		evasion_item_modifier(generation, target.item(environment)) *
		ability_evasion_modifier(target, environment, weather_is_blocked) *
		gravity_multiplier
	;
	
	constexpr auto max = 100_bi;
	return static_cast<double>(bounded::clamp(calculated_accuracy, 0_bi, max)) / static_cast<double>(max);
}

} // namespace technicalmachine
