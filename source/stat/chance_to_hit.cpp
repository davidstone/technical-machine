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

#include "chance_to_hit.hpp"

#include <algorithm>

#include "stage.hpp"
#include "stat_names.hpp"

#include "../ability.hpp"
#include "../rational.hpp"
#include "../weather.hpp"

#include "../move/accuracy.hpp"

#include "../pokemon/active_pokemon.hpp"
#include "../pokemon/pokemon.hpp"

namespace technicalmachine {
namespace {
using namespace bounded::literal;

auto move_can_miss(ActivePokemon const & user, BaseAccuracy const base_accuracy, Ability const & target_ability) -> bool;

using AccuracyItemModifier = bounded_rational<bounded::integer<1, 11>, bounded::integer<1, 10>>;
auto accuracy_item_modifier(Item const item, bool target_moved) -> AccuracyItemModifier;

using EvasionItemModifier = bounded_rational<bounded::integer<1, 19>, bounded::integer<1, 20>>;
auto evasion_item_modifier(Item const item) -> EvasionItemModifier;

using namespace detail_chance_to_hit;

}	// namespace

auto chance_to_hit(ActivePokemon const & user, ActivePokemon const & target, Weather const weather, bool target_moved) -> ChanceToHit {
	auto const base_accuracy = accuracy(current_move(user));
	if (!move_can_miss(user, base_accuracy, get_ability(target))) {
		return ChanceToHit(max, max);
	}
	constexpr auto gravity_denominator = 3_bi;
	auto const gravity_numerator = BOUNDED_CONDITIONAL(weather.gravity(), 5_bi, gravity_denominator);
	auto const gravity_multiplier = make_rational(gravity_numerator, gravity_denominator);
	auto const calculated_accuracy = *base_accuracy *
		modifier<StatNames::ACC>(stage(user)) *
		modifier<StatNames::EVA>(stage(target)) *
		accuracy_item_modifier(get_item(user), target_moved) *
		ability_accuracy_modifier(user) *
		evasion_item_modifier(get_item(target)) *
		ability_evasion_modifier(target, weather) *
		gravity_multiplier
	;
	
	return ChanceToHit(bounded::clamped_integer<min.value(), max.value()>(calculated_accuracy), max);
}

namespace {

auto move_can_miss(ActivePokemon const & user, BaseAccuracy const base_accuracy, Ability const & target_ability) -> bool {
	return static_cast<bool>(base_accuracy) and !get_ability(user).cannot_miss() and !target_ability.cannot_miss() and !locked_on(user);
}

auto accuracy_item_modifier(Item const item, bool target_moved) -> AccuracyItemModifier {
	switch (item) {
		case Item::Wide_Lens:
			return AccuracyItemModifier(11_bi, 10_bi);
		case Item::Zoom_Lens:
			return target_moved ? AccuracyItemModifier(6_bi, 5_bi) : AccuracyItemModifier(1_bi, 1_bi);
		default:
			return AccuracyItemModifier(1_bi, 1_bi);
	}
}

auto evasion_item_modifier(Item const item) -> EvasionItemModifier {
	switch (item) {
		case Item::BrightPowder:
			return EvasionItemModifier(9_bi, 10_bi);
		case Item::Lax_Incense:
			return EvasionItemModifier(19_bi, 20_bi);
		default:
			return EvasionItemModifier(1_bi, 1_bi);
	}
}

}	// namespace
}	// namespace technicalmachine
