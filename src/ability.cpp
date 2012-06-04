// Ability data structure
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

#include "ability.hpp"

#include <cassert>

#include "status.hpp"
#include "team.hpp"
#include "type.hpp"
#include "weather.hpp"

#include "pokemon/pokemon.hpp"

namespace technicalmachine {

Ability::Ability ():
	name (END) {
}

Ability::Ability (Abilities ability):
	name (ability) {
}

bool Ability::is_set () const {
	return name != END;
}

void Ability::set_if_unknown (Abilities const ability) {
	assert (ability < END);
	if (!is_set())
		name = ability;
}

bool Ability::blocks_switching (Team const & switcher, Weather const & weather) const {
	switch (name) {
		case SHADOW_TAG:
			return switcher.pokemon().ability.name != Ability::SHADOW_TAG;
		case ARENA_TRAP:
			return grounded (switcher, switcher.pokemon(), weather);
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

template<>
bool Ability::blocks_status<Status::BURN> (Weather const & weather) const {
	switch (name) {
		case LEAF_GUARD:
			return weather.sun();
		case WATER_VEIL:
			return true;
		default:
			return false;
	}
}

template<>
bool Ability::blocks_status<Status::FREEZE> (Weather const & weather) const {
	// Pass in weather to take advantage of template specialization, but I don't
	// want to be warned about unused variables.
	static_cast<void>(weather);
	return name == MAGMA_ARMOR;
}

template<>
bool Ability::blocks_status<Status::PARALYSIS> (Weather const & weather) const {
	switch (name) {
		case LEAF_GUARD:
			return weather.sun();
		case LIMBER:
			return true;
		default:
			return false;
	}
}

template<>
bool Ability::blocks_status<Status::POISON> (Weather const & weather) const {
	switch (name) {
		case IMMUNITY:
			return true;
		case LEAF_GUARD:
			return weather.sun();
		default:
			return false;
	}
}

template<>
bool Ability::blocks_status<Status::POISON_TOXIC> (Weather const & weather) const {
	return blocks_status<Status::POISON> (weather);
}

template<>
bool Ability::blocks_status<Status::SLEEP> (Weather const & weather) const {
	switch (name) {
		case INSOMNIA:
		case VITAL_SPIRIT:
			return true;
		case LEAF_GUARD:
			return weather.sun();
		default:
			return false;
	}
}

template<>
bool Ability::blocks_status<Status::REST> (Weather const & weather) const {
	return blocks_status<Status::SLEEP> (weather);
}

bool Ability::blocks_confusion () const {
	return name == OWN_TEMPO;
}

bool Ability::reflects_status () const {
	return name == SYNCHRONIZE;
}

bool Ability::absorbs_poison_damage () const {
	return name == POISON_HEAL;
}

bool Ability::blocks_burn_damage_penalty () const {
	return name == GUTS;
}

bool Ability::blocks_paralysis_speed_penalty () const {
	return name == QUICK_FEET;
}

bool Ability::blocks_sound_moves () const {
	return name == SOUNDPROOF;
}

bool Ability::can_clear_status (Status const status) const {
	return name == SHED_SKIN and !status.is_clear();
}

bool Ability::clears_status_on_switch () const {
	return name == NATURAL_CURE;
}

bool Ability::is_immune_to_ground () const {
	return name == LEVITATE;
}

bool Ability::wakes_up_early () const {
	return name == EARLY_BIRD;
}

bool Ability::weakens_burn () const {
	return name == HEATPROOF;
}

bool Ability::harms_sleepers () const {
	return name == BAD_DREAMS;
}

bool Ability::blocks_phazing () const {
	return name == SUCTION_CUPS;
}

bool Ability::blocks_recoil () const {
	switch (name) {
		case MAGIC_GUARD:
		case ROCK_HEAD:
			return true;
		default:
			return false;
	}
}

bool Ability::blocks_secondary_damage () const {
	return name == MAGIC_GUARD;
}

bool Ability::cannot_miss () const {
	return name == NO_GUARD;
}

bool Ability::damages_leechers () const {
	return name == LIQUID_OOZE;
}

bool Ability::weakens_se_attacks () const {
	switch (name) {
		case FILTER:
		case SOLID_ROCK:
			return true;
		default:
			return false;
	}
}

bool Ability::strengthens_nve_attacks () const {
	return name == TINTED_LENS;
}

bool Ability::uses_extra_pp() const {
	return name == PRESSURE;
}

bool Ability::ignores_blockers () const {
	return name == MOLD_BREAKER;
}

bool Ability::boosts_critical_hits () const {
	return name == SNIPER;
}

bool Ability::boosts_defense (Status const status) const {
	return name == MARVEL_SCALE and !status.is_clear();
}

bool Ability::boosts_special_attack (Weather const & weather) const {
	return name == SOLAR_POWER and weather.sun();
}

bool Ability::boosts_special_defense (Weather const & weather) const {
	return name == FLOWER_GIFT and weather.sun();
}

bool Ability::boosts_speed () const {
	return name == SPEED_BOOST;
}

bool Ability::boosts_speed_when_flinched () const {
	return name == STEADFAST;
}

bool Ability::boosts_stab () const {
	return name == ADAPTABILITY;
}

bool Ability::is_loafing (bool const loaf) const {
	return name == TRUANT and loaf;
}

void Ability::activate_on_switch (Team & switcher, Team & other, Weather & weather) {
	switch (switcher.pokemon().ability.name) {
		case SLOW_START:
			switcher.slow_start = 5;
			break;
		case DOWNLOAD: {
			Pokemon const & pokemon = other.pokemon();
			Stat::calculate_defense (other);
			Stat::calculate_special_defense (other, weather);
			switcher.stage.boost((pokemon.def.stat >= pokemon.spd.stat) ? Stat::SPA : Stat::ATK, 1);
			break;
		}
		case DRIZZLE:
			weather.set_rain (Weather::Duration::permanent);
			break;
		case DROUGHT:
			weather.set_sun (Weather::Duration::permanent);
			break;
		case FORECAST:	// TODO: fix this
			break;
		case INTIMIDATE:
			other.stage.boost(Stat::ATK, -1);
			break;
		case SAND_STREAM:
			weather.set_sand (Weather::Duration::permanent);
			break;
		case SNOW_WARNING:
			weather.set_hail (Weather::Duration::permanent);
			break;
		case TRACE:
			break;
		default:
			break;
	}
}

bool operator== (Ability const lhs, Ability const rhs) {
	return lhs.name == rhs.name;
}

bool operator!= (Ability const lhs, Ability const rhs) {
	return !(lhs == rhs);
}

}	// namespace technicalmachine
