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

#include <cmath>

#include "stat.hpp"

#include "../rational.hpp"
#include "../team.hpp"
#include "../weather.hpp"

namespace technicalmachine {
namespace {
constexpr ChanceToHit::value_type max = 100;

bool move_can_miss (Team const & user, Ability target_ability);
Rational accuracy_stage_modifier (int stage);
Rational evasion_stage_modifier (int stage);
Rational accuracy_item_modifier (Item const & item, bool target_moved);
Rational accuracy_ability_modifier (Pokemon const & pokemon);
Rational evasion_item_modifier (Item const & item);
Rational evasion_ability_modifier (Team const & target, Weather const & weather);

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

void ChanceToHit::update(Team const & user, Team const & target, Weather const & weather) {
	if (move_can_miss(user, target.pokemon().ability)) {
		value_type accuracy = user.pokemon().move().accuracy();
		accuracy *= accuracy_stage_modifier(target.stage[Stat::ACC]);
		accuracy *= evasion_stage_modifier(target.stage[Stat::EVA]);

		accuracy *= accuracy_item_modifier(user.pokemon().item, target.moved);
		accuracy *= accuracy_ability_modifier(user.pokemon());
		
		accuracy *= evasion_item_modifier(target.pokemon().item);
		accuracy *= evasion_ability_modifier (target, weather);

		if (weather.gravity())
			accuracy *= Rational(5, 3);
		
		probability = std::min(accuracy, max);
	}
	else {
		probability = max;
	}
}

namespace {

bool move_can_miss(Team const & user, Ability target_ability) {
	return user.pokemon().move().can_miss() and !user.pokemon().ability.cannot_miss() and !target_ability.cannot_miss() and !user.lock_on;
}

Rational accuracy_stage_modifier(int const stage) {
	return (stage >= 0) ?
		positive_stage_boost<3>(static_cast<unsigned>(stage)) :
		negative_stage_boost<3>(static_cast<unsigned>(-stage));
}

Rational evasion_stage_modifier(int const stage) {
	return (stage < 0) ?
		positive_stage_boost<3>(static_cast<unsigned>(-stage)) :
		negative_stage_boost<3>(static_cast<unsigned>(stage));
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

Rational accuracy_ability_modifier(Pokemon const & pokemon) {
	switch (pokemon.ability.name) {
		case Ability::COMPOUNDEYES:
			return Rational(13, 10);
		case Ability::HUSTLE:
			return pokemon.move().is_physical() ? Rational(4, 5) : Rational(1, 1);
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

Rational evasion_ability_modifier(Team const & target, Weather const & weather) {
	switch (target.pokemon().ability.name) {
		case Ability::SAND_VEIL:
			return weather.sand() ? Rational(4, 5) : Rational(1, 1);
		case Ability::SNOW_CLOAK:
			return weather.hail() ? Rational(4, 5) : Rational(1, 1);
		case Ability::TANGLED_FEET:
			return target.confused ? Rational(4, 5) : Rational(1, 1);
		default:
			return Rational(1, 1);
	}
}

}	// unnamed namespace

}	// namespace technicalmachine
