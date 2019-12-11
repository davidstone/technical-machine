// Chance to hit calculations
// Copyright (C) 2014 David Stone
//
// This file is part of Technical Machine.
//
// Technical Machine is free software: you can redistribute it and / or modify
// it under the terms of the GNU Affero General Public License as
// published by the Free Software Foundation, either version 3 of the
// License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.

#include <tm/stat/chance_to_hit.hpp>

#include <algorithm>

#include <tm/stat/stage.hpp>
#include <tm/stat/stat_names.hpp>

#include <tm/move/accuracy.hpp>
#include <tm/move/category.hpp>

#include <tm/pokemon/active_pokemon.hpp>
#include <tm/pokemon/pokemon.hpp>

#include <tm/ability.hpp>
#include <tm/rational.hpp>
#include <tm/weather.hpp>

namespace technicalmachine {
enum class Generation : std::uint8_t;
namespace {
using namespace bounded::literal;

auto move_can_miss(ActivePokemon const user, BaseAccuracy const base_accuracy, Ability const target_ability) -> bool {
	return static_cast<bool>(base_accuracy) and !cannot_miss(get_ability(user)) and !cannot_miss(target_ability) and !user.locked_on();
}

auto accuracy_item_modifier(Item const item, bool target_moved) {
	using Modifier = rational<bounded::integer<1, 11>, bounded::integer<1, 10>>;
	switch (item) {
		case Item::Wide_Lens: return Modifier(11_bi, 10_bi);
		case Item::Zoom_Lens: return target_moved ? Modifier(6_bi, 5_bi) : Modifier(1_bi, 1_bi);
		default: return Modifier(1_bi, 1_bi);
	}
}

auto ability_accuracy_modifier(ActivePokemon const user, Moves const move) {
	using Modifier = rational<
		bounded::integer<1, 13>,
		bounded::integer<1, 10>
	>;
	switch (get_ability(user)) {
		case Ability::Compoundeyes: return Modifier(13_bi, 10_bi);
		case Ability::Hustle: return is_physical(move) ? Modifier(4_bi, 5_bi) : Modifier(1_bi, 1_bi);
		default: return Modifier(1_bi, 1_bi);
	}
}

auto evasion_item_modifier(Generation const generation, Item const item) {
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


auto ability_evasion_modifier(ActivePokemon const target, Ability const user_ability, Weather const weather) {
	using Modifier = rational<
		bounded::integer<1, 4>,
		bounded::integer<1, 5>
	>;
	auto const target_ability = get_ability(target);
	auto const blocks_weather = weather_is_blocked_by_ability(target_ability, user_ability);
	switch (target_ability) {
		case Ability::Sand_Veil: return weather.sand(blocks_weather) ? Modifier(4_bi, 5_bi) : Modifier(1_bi, 1_bi);
		case Ability::Snow_Cloak: return weather.hail(blocks_weather) ? Modifier(4_bi, 5_bi) : Modifier(1_bi, 1_bi);
		case Ability::Tangled_Feet: return target.is_confused() ? Modifier(4_bi, 5_bi) : Modifier(1_bi, 1_bi);
		default: return Modifier(1_bi, 1_bi);
	}
}

}	// namespace

auto chance_to_hit(Generation const generation, ActivePokemon const user, Moves const move, ActivePokemon const target, Weather const weather, bool target_moved) -> ChanceToHit {
	auto const base_accuracy = accuracy(generation, move);
	if (!move_can_miss(user, base_accuracy, get_ability(target))) {
		return 1.0;
	}
	constexpr auto gravity_denominator = 3_bi;
	auto const gravity_numerator = BOUNDED_CONDITIONAL(weather.gravity(), 5_bi, gravity_denominator);
	auto const gravity_multiplier = rational(gravity_numerator, gravity_denominator);
	auto const calculated_accuracy = *base_accuracy *
		modifier<StatNames::ACC>(user.stage()) *
		modifier<StatNames::EVA>(target.stage()) *
		accuracy_item_modifier(get_item(user), target_moved) *
		ability_accuracy_modifier(user, move) *
		evasion_item_modifier(generation, get_item(target)) *
		ability_evasion_modifier(target, get_ability(user), weather) *
		gravity_multiplier
	;
	
	auto const max = 100;
	return static_cast<double>(bounded::clamped_integer<0, max>(calculated_accuracy)) / static_cast<double>(max);
}

}	// namespace technicalmachine
