// Chance to hit calculations
// Copyright (C) 2012 David Stone
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
#include "stat.hpp"

#include "../ability.hpp"
#include "../rational.hpp"
#include "../weather.hpp"

#include "../pokemon/active_pokemon.hpp"
#include "../pokemon/pokemon.hpp"

namespace technicalmachine {
namespace {
constexpr ChanceToHit::value_type max = 100;

bool move_can_miss(ActivePokemon const & user, Ability const & target_ability);
Rational accuracy_item_modifier (Item const & item, bool target_moved);
Rational evasion_item_modifier (Item const & item);

}	// unnamed namespace

ChanceToHit::ChanceToHit() :
	probability(max)
	{
}

ChanceToHit::value_type ChanceToHit::operator()() const {
	return probability;
}

ChanceToHit::value_type ChanceToHit::inverse() const {
	return max - probability;
}

bool ChanceToHit::can_miss() const {
	return probability < max;
}

void ChanceToHit::update(ActivePokemon const & user, ActivePokemon const & target, Weather const & weather, bool const target_moved) {
	if (move_can_miss(user, target.ability())) {
		value_type accuracy = user.move().accuracy();
		accuracy *= user.stage_modifier<Stat::ACC>();
		accuracy *= target.stage_modifier<Stat::EVA>();

		accuracy *= accuracy_item_modifier(user.item(), target_moved);
		accuracy *= Ability::accuracy_modifier(user);
		
		accuracy *= evasion_item_modifier(target.item());
		accuracy *= Ability::evasion_modifier(target, weather);

		if (weather.gravity())
			accuracy *= Rational(5, 3);
		
		probability = std::min(accuracy, max);
	}
	else {
		probability = max;
	}
}

namespace {

bool move_can_miss(ActivePokemon const & user, Ability const & target_ability) {
	return user.move().can_miss() and !user.ability().cannot_miss() and !target_ability.cannot_miss() and !user.locked_on();
}

Rational accuracy_item_modifier(Item const & item, bool target_moved) {
	switch (item.name) {
		case Item::WIDE_LENS:
			return Rational(11, 10);
		case Item::ZOOM_LENS:
			return target_moved ? Rational(6, 5) : Rational(1, 1);
		default:
			return Rational(1, 1);
	}
}

Rational evasion_item_modifier(Item const & item) {
	switch (item.name) {
		case Item::BRIGHTPOWDER:
			return Rational(9, 10);
		case Item::LAX_INCENSE:
			return Rational(19, 20);
		default:
			return Rational(1, 1);
	}
}

}	// unnamed namespace

}	// namespace technicalmachine
