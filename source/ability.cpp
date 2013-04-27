// Ability data structure
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

#include "ability.hpp"

#include <cassert>

#include "heal.hpp"
#include "rational.hpp"
#include "status.hpp"
#include "weather.hpp"

#include "move/move.hpp"

#include "pokemon/active_pokemon.hpp"

#include "string_conversions/conversion.hpp"

#include "type/type.hpp"

namespace technicalmachine {
namespace {
bool pinch_ability_activates(Pokemon const & attacker, Type::Types type);
}	// namespace

Ability::Ability():
	name(END) {
}

Ability::Ability(Abilities ability):
	name(ability) {
}

Ability::Ability(std::string const & str):
	name(::technicalmachine::from_string<Abilities>(str))
	{
}

bool Ability::is_set() const {
	return name != END;
}

void Ability::set_if_unknown(Abilities const ability) {
	assert(ability < END);
	if (!is_set())
		name = ability;
}

bool Ability::blocks_switching(ActivePokemon const & switcher, Weather const & weather) const {
	switch (name) {
		case Shadow_Tag:
			return switcher.ability().name != Shadow_Tag;
		case Arena_Trap:
			return grounded(switcher, weather);
		case Magnet_Pull:
			return is_type(switcher, Type::Steel);
		default:
			return false;
	}
}

bool Ability::blocks_weather() const {
	switch (name) {
		case Air_Lock:
		case Cloud_Nine:
			return true;
		default:
			return false;
	}
}

template<>
bool Ability::blocks_status<Status::BURN>(Weather const & weather) const {
	switch (name) {
		case Leaf_Guard:
			return weather.sun();
		case Water_Veil:
			return true;
		default:
			return false;
	}
}

template<>
bool Ability::blocks_status<Status::FREEZE>(Weather const & weather) const {
	// Pass in weather to take advantage of template specialization, but I don't
	// want to be warned about unused variables.
	static_cast<void>(weather);
	return name == Magma_Armor;
}

template<>
bool Ability::blocks_status<Status::PARALYSIS>(Weather const & weather) const {
	switch (name) {
		case Leaf_Guard:
			return weather.sun();
		case Limber:
			return true;
		default:
			return false;
	}
}

template<>
bool Ability::blocks_status<Status::POISON>(Weather const & weather) const {
	switch (name) {
		case Immunity:
			return true;
		case Leaf_Guard:
			return weather.sun();
		default:
			return false;
	}
}

template<>
bool Ability::blocks_status<Status::POISON_TOXIC>(Weather const & weather) const {
	return blocks_status<Status::POISON>(weather);
}

template<>
bool Ability::blocks_status<Status::SLEEP>(Weather const & weather) const {
	switch (name) {
		case Insomnia:
		case Vital_Spirit:
			return true;
		case Leaf_Guard:
			return weather.sun();
		default:
			return false;
	}
}

template<>
bool Ability::blocks_status<Status::REST>(Weather const & weather) const {
	return blocks_status<Status::SLEEP>(weather);
}

bool Ability::blocks_confusion() const {
	return name == Own_Tempo;
}

bool Ability::reflects_status() const {
	return name == Synchronize;
}

bool Ability::absorbs_poison_damage() const {
	return name == Poison_Heal;
}

bool Ability::blocks_burn_damage_penalty() const {
	return name == Guts;
}

bool Ability::blocks_paralysis_speed_penalty() const {
	return name == Quick_Feet;
}

bool Ability::blocks_sound_moves() const {
	return name == Soundproof;
}

bool Ability::can_clear_status(Status const status) const {
	return name == Shed_Skin and !status.is_clear();
}

bool Ability::clears_status_on_switch() const {
	return name == Natural_Cure;
}

bool Ability::is_immune_to_ground() const {
	return name == Levitate;
}

bool Ability::wakes_up_early() const {
	return name == Early_Bird;
}

bool Ability::weakens_burn() const {
	return name == Heatproof;
}

bool Ability::harms_sleepers() const {
	return name == Bad_Dreams;
}

bool Ability::blocks_phazing() const {
	return name == Suction_Cups;
}

bool Ability::blocks_recoil() const {
	switch (name) {
		case Magic_Guard:
		case Rock_Head:
			return true;
		default:
			return false;
	}
}

bool Ability::blocks_secondary_damage() const {
	return name == Magic_Guard;
}

bool Ability::cannot_miss() const {
	return name == No_Guard;
}

bool Ability::damages_leechers() const {
	return name == Liquid_Ooze;
}

bool Ability::weakens_se_attacks() const {
	switch (name) {
		case Filter:
		case Solid_Rock:
			return true;
		default:
			return false;
	}
}

bool Ability::strengthens_nve_attacks() const {
	return name == Tinted_Lens;
}

bool Ability::uses_extra_pp() const {
	return name == Pressure;
}

bool Ability::ignores_blockers() const {
	return name == Mold_Breaker;
}

bool Ability::boosts_critical_hits() const {
	return name == Sniper;
}

bool Ability::boosts_defense(Status const status) const {
	return name == Marvel_Scale and !status.is_clear();
}

bool Ability::boosts_special_attack(Weather const & weather) const {
	return name == Solar_Power and weather.sun();
}

bool Ability::boosts_special_defense(Weather const & weather) const {
	return name == Flower_Gift and weather.sun();
}

bool Ability::boosts_speed() const {
	return name == Speed_Boost;
}

bool Ability::boosts_speed_when_flinched() const {
	return name == Steadfast;
}

bool Ability::boosts_stab() const {
	return name == Adaptability;
}

bool Ability::is_loafing(bool const loaf) const {
	return name == Truant and loaf;
}

Rational Ability::accuracy_modifier(ActivePokemon const & user) {
	switch (user.ability().name) {
		case Compoundeyes:
			return Rational(13, 10);
		case Hustle:
			return user.move().is_physical() ? Rational(4, 5) : Rational(1, 1);
		default:
			return Rational(1, 1);
	}
}

Rational Ability::evasion_modifier(ActivePokemon const & target, Weather const & weather) {
	switch (target.ability().name) {
		case Sand_Veil:
			return weather.sand() ? Rational(4, 5) : Rational(1, 1);
		case Snow_Cloak:
			return weather.hail() ? Rational(4, 5) : Rational(1, 1);
		case Tangled_Feet:
			return target.is_confused() ? Rational(4, 5) : Rational(1, 1);
		default:
			return Rational(1, 1);
	}
}

Rational Ability::attacker_modifier(Pokemon const & attacker, Pokemon const & defender, unsigned const base_power) {
	switch (attacker.ability().name) {
		case Technician:
			return (base_power <= 60) ? Rational(3, 2) : Rational(1);
		case Blaze:
			return pinch_ability_activates(attacker, Type::Fire) ? Rational(3, 2) : Rational(1);
		case Overgrow:
			return pinch_ability_activates(attacker, Type::Grass) ? Rational(3, 2) : Rational(1);
		case Swarm:
			return pinch_ability_activates(attacker, Type::Bug) ? Rational(3, 2) : Rational(1);
		case Torrent:
			return pinch_ability_activates(attacker, Type::Water) ? Rational(3, 2) : Rational(1);
		case Iron_Fist:
			return attacker.move().is_boosted_by_iron_fist() ? Rational(6, 5) : Rational(1);
		case Reckless:
			return attacker.move().is_boosted_by_reckless() ? Rational(6, 5) : Rational(1);
		case Rivalry:
			return Rational(static_cast<unsigned>(4 + attacker.gender().multiplier(defender.gender())), 4);
		default:
			return Rational(1);
	}
}

namespace {
bool pinch_ability_activates(Pokemon const & attacker, Type::Types const type) {
	return attacker.move().type() == type and attacker.current_hp() <= Rational(1, 3);
}
}	// namespace

Rational Ability::defender_modifier(Move const & move, Ability const ability) {
	switch (ability.name) {
		case Dry_Skin:
			return (move.type() == Type::Fire) ? Rational(5, 4) : Rational(1);
		case Heatproof:
			return (move.type() == Type::Fire) ? Rational(1, 2) : Rational(1);
		case Thick_Fat:
			return (move.type() == Type::Fire or move.type() == Type::Ice) ? Rational(1, 2) : Rational(1);
		default:
			return Rational(1);
	}
}

template<>
Rational Ability::stat_modifier<Stat::ATK>(ActivePokemon const & attacker, Weather const & weather) {
	switch (attacker.ability().name) {
		case Flower_Gift:
			return weather.sun() ? Rational(3, 2) : Rational(1);
		case Guts:
			return (!attacker.status().is_clear()) ? Rational(3, 2) : Rational(1);
		case Hustle:
			return Rational(3, 2);
		case Huge_Power:
		case Pure_Power:
			return Rational(2);
		case Slow_Start:
			return attacker.slow_start_is_active() ? Rational(1, 2) : Rational(1);
		default:
			return Rational(1);
	}
}
template<>
Rational Ability::stat_modifier<Stat::SPA>(ActivePokemon const & pokemon, Weather const & weather) {
	return pokemon.ability().boosts_special_attack(weather) ? Rational(3, 2) : Rational(1);
}
template<>
Rational Ability::stat_modifier<Stat::DEF>(ActivePokemon const & defender, Weather const &) {
	return defender.ability().boosts_defense(defender.status()) ? Rational(3, 2) : Rational(1);
}
template<>
Rational Ability::stat_modifier<Stat::SPD>(ActivePokemon const & pokemon, Weather const & weather) {
	return pokemon.ability().boosts_special_defense(weather) ? Rational(3, 2) : Rational(1);
}
template<>
Rational Ability::stat_modifier<Stat::SPE>(ActivePokemon const & pokemon, Weather const & weather) {
	switch (pokemon.ability().name) {
		case Chlorophyll:
			return weather.sun() ? Rational(2) : Rational(1);
		case Swift_Swim:
			return weather.rain() ? Rational(2) : Rational(1);
		case Unburden:
			return pokemon.item().was_lost() ? Rational(2) : Rational(1);
		case Quick_Feet:
			return (!pokemon.status().is_clear()) ? Rational(3, 2) : Rational(1);
		case Slow_Start:
			return pokemon.slow_start_is_active() ? Rational(1, 2) : Rational(1);
		default:
			return Rational(1);
	}
}

void Ability::activate_on_switch(ActivePokemon & switcher, ActivePokemon & other, Weather & weather) {
	switch (switcher.ability().name) {
		case Download: {
			calculate_defense(other, weather);
			calculate_special_defense(other, weather);
			switcher.stat_boost((other.stat(Stat::DEF).stat >= other.stat(Stat::SPD).stat) ? Stat::SPA : Stat::ATK, 1);
			break;
		}
		case Drizzle:
			weather.set_rain(Weather::Duration::permanent);
			break;
		case Drought:
			weather.set_sun(Weather::Duration::permanent);
			break;
		case Forecast:	// TODO: fix this
			break;
		case Intimidate:
			other.stat_boost(Stat::ATK, -1);
			break;
		case Sand_Stream:
			weather.set_sand(Weather::Duration::permanent);
			break;
		case Snow_Warning:
			weather.set_hail(Weather::Duration::permanent);
			break;
		case Trace:
			break;
		default:
			break;
	}
}

void Ability::weather_healing(Pokemon & pokemon, Weather const & weather) {
	switch (pokemon.ability().name) {
		case Dry_Skin:
			if (weather.rain()) {
				heal(pokemon, Rational(1, 8));
			}
			else if (weather.sun()) {
				drain(pokemon, Rational(1, 8));
			}
			break;
		case Hydration:
			if (weather.rain()) {
				pokemon.status().clear();
			}
			break;
		case Ice_Body:
			if (weather.hail()) {
				heal(pokemon, Rational(1, 16));
			}
			break;
		case Rain_Dish:
			if (weather.rain()) {
				heal(pokemon, Rational(1, 16));
			}
			break;
		default:
			break;
	}
}

bool operator==(Ability const lhs, Ability const rhs) {
	return lhs.name == rhs.name;
}

bool operator!=(Ability const lhs, Ability const rhs) {
	return !(lhs == rhs);
}

std::string Ability::to_string() const {
	return ::technicalmachine::to_string(name);
}

}	// namespace technicalmachine
