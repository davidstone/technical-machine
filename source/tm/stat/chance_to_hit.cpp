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
#include <tm/generation.hpp>
#include <tm/rational.hpp>
#include <tm/weather.hpp>

namespace technicalmachine {
enum class Generation : std::uint8_t;
namespace {
using namespace bounded::literal;

auto move_can_miss(ActivePokemon const user, Moves const move, BaseAccuracy const base_accuracy, ActivePokemon const target) -> bool {
	return
		static_cast<bool>(base_accuracy) and
		!cannot_miss(user.ability()) and
		!cannot_miss(target.ability()) and
		!(move == Moves::Body_Slam and target.minimized()) and
		!user.locked_on();
}

auto accuracy_item_modifier(Item const item, bool target_moved) {
	using Modifier = rational<bounded::integer<1, 11>, bounded::integer<1, 10>>;
	switch (item) {
		case Item::Wide_Lens: return Modifier(11_bi, 10_bi);
		case Item::Zoom_Lens: return target_moved ? Modifier(6_bi, 5_bi) : Modifier(1_bi, 1_bi);
		default: return Modifier(1_bi, 1_bi);
	}
}

auto ability_accuracy_modifier(Generation const generation, Ability const ability, KnownMove const move) {
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


auto ability_evasion_modifier(ActivePokemon const target, Weather const weather, bool const blocks_weather) {
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

}	// namespace

auto chance_to_hit(Generation const generation, ActivePokemon const user, KnownMove const move, ActivePokemon const target, Weather const weather, bool target_moved) -> ChanceToHit {
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
		accuracy_item_modifier(user.item(generation, weather), target_moved) *
		ability_accuracy_modifier(generation, user_ability, move) *
		evasion_item_modifier(generation, target.item(generation, weather)) *
		ability_evasion_modifier(target, weather, blocks_weather) *
		gravity_multiplier
	;
	
	auto const max = 100;
	return static_cast<double>(bounded::clamped_integer<0, max>(calculated_accuracy)) / static_cast<double>(max);
}

}	// namespace technicalmachine
