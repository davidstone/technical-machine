// Ability data structure
// Copyright (C) 2011 David Stone
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

#include "ability.hpp"
#include "pokemon.hpp"
#include "team.hpp"
#include "type.hpp"
#include "weather.hpp"

namespace technicalmachine {

Ability::Ability ():
	name (END) {
}

Ability::Ability (Abilities ability):
	name (ability) {
}

bool Ability::blocks_switching (Team const & switcher, Weather const & weather) const {
	switch (name) {
		case SHADOW_TAG:
			return switcher.pokemon ().ability.name != Ability::SHADOW_TAG;
		case ARENA_TRAP:
			return grounded (switcher, weather);
		case MAGNET_PULL:
			return is_type (switcher, Type::STEEL);
		default:
			return false;
	}
}

bool Ability::blocks_weather () const {
	switch (name) {
		case AIR_LOCK:
		case CLOUD_NINE:
			return true;
		default:
			return false;
	}
}

bool Ability::blocks_burn (Weather const & weather) const {
	switch (name) {
		case LEAF_GUARD:
			return weather.sun;
		case WATER_VEIL:
			return true;
		default:
			return false;
	}
}

bool Ability::blocks_freeze () const {
	switch (name) {
		case MAGMA_ARMOR:
			return true;
		default:
			return false;
	}
}

bool Ability::blocks_paralysis (Weather const & weather) const {
	switch (name) {
		case LEAF_GUARD:
			return weather.sun;
		case LIMBER:
			return true;
		default:
			return false;
	}
}

bool Ability::blocks_poison (Weather const & weather) const {
	switch (name) {
		case IMMUNITY:
			return true;
		case LEAF_GUARD:
			return weather.sun;
		default:
			return false;
	}
}

bool Ability::blocks_sleep (Weather const & weather) const {
	switch (name) {
		case INSOMNIA:
		case VITAL_SPIRIT:
			return true;
		case LEAF_GUARD:
			return weather.sun;
		default:
			return false;
	}
}

bool Ability::weakens_SE_attacks () const {
	switch (name) {
		case FILTER:
		case SOLID_ROCK:
			return true;
		default:
			return false;
	}
}

bool Ability::is_set () const {
	return name != END;
}

}	// namespace technicalmachine
