// Chance to hit calculations
// Copyright (C) 2013 David Stone
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

#include "../move/accuracy.hpp"

#include "../pokemon/active_pokemon.hpp"
#include "../pokemon/pokemon.hpp"

namespace technicalmachine {
namespace {
constexpr uint8_t max = 100;

bool move_can_miss(ActivePokemon const & user, Ability const & target_ability);
Rational accuracy_item_modifier (Item const & item, bool target_moved);
Rational evasion_item_modifier (Item const & item);

}	// unnamed namespace

ChanceToHit::ChanceToHit() :
	probability(max)
	{
}

Rational ChanceToHit::operator()() const {
	return Rational(probability, max);
}

void ChanceToHit::update(ActivePokemon const & user, ActivePokemon const & target, Weather const & weather, bool const target_moved) {
	if (move_can_miss(user, target.ability())) {
		unsigned calculated_accuracy = accuracy(user.move().name());
		calculated_accuracy *= user.stage_modifier<Stat::ACC>();
		calculated_accuracy *= target.stage_modifier<Stat::EVA>();

		calculated_accuracy *= accuracy_item_modifier(user.item(), target_moved);
		calculated_accuracy *= Ability::accuracy_modifier(user);
		
		calculated_accuracy *= evasion_item_modifier(target.item());
		calculated_accuracy *= Ability::evasion_modifier(target, weather);

		if (weather.gravity())
			calculated_accuracy *= Rational(5, 3);
		
		probability = std::min(static_cast<uint8_t>(calculated_accuracy), max);
	}
	else {
		probability = max;
	}
}

namespace {

bool move_can_miss(ActivePokemon const & user, Ability const & target_ability) {
	return can_miss(user.move()) and !user.ability().cannot_miss() and !target_ability.cannot_miss() and !user.locked_on();
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
