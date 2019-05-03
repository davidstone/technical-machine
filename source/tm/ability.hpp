// Copyright (C) 2018 David Stone
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

#pragma once

#include <tm/enum.hpp>
#include <tm/status.hpp>

#include <cstdint>

namespace technicalmachine {

struct ActivePokemon;
struct MutableActivePokemon;
struct Weather;

enum class Ability : std::uint8_t {
	Adaptability, Aftermath, Air_Lock, Anger_Point, Anticipation,
	Arena_Trap, Bad_Dreams, Battle_Armor, Blaze, Chlorophyll,
	Clear_Body, Cloud_Nine, Color_Change, Compoundeyes, Cute_Charm,
	Damp, Download, Drizzle, Drought, Dry_Skin,
	Early_Bird, Effect_Spore, Filter, Flame_Body, Flash_Fire,
	Flower_Gift, Forecast, Forewarn, Frisk, Gluttony,
	Guts, Heatproof, Honey_Gather, Huge_Power, Hustle,
	Hydration, Hyper_Cutter, Ice_Body, Illuminate, Immunity,
	Inner_Focus, Insomnia, Intimidate, Iron_Fist, Keen_Eye,
	Klutz, Leaf_Guard, Levitate, Lightningrod, Limber,
	Liquid_Ooze, Magic_Guard, Magma_Armor, Magnet_Pull, Marvel_Scale,
	Minus, Mold_Breaker, Motor_Drive, Multitype, Natural_Cure,
	No_Guard, Normalize, Oblivious, Overgrow, Own_Tempo,
	Pickup, Plus, Poison_Heal, Poison_Point, Pressure,
	Pure_Power, Quick_Feet, Rain_Dish, Reckless, Rivalry,
	Rock_Head, Rough_Skin, Run_Away, Sand_Stream, Sand_Veil,
	Scrappy, Serene_Grace, Shadow_Tag, Shed_Skin, Shell_Armor,
	Shield_Dust, Simple, Skill_Link, Slow_Start, Sniper,
	Snow_Cloak, Snow_Warning, Solar_Power, Solid_Rock, Soundproof,
	Speed_Boost, Stall, Static, Steadfast, Stench,
	Sticky_Hold, Storm_Drain, Sturdy, Suction_Cups, Super_Luck,
	Swarm, Swift_Swim, Synchronize, Tangled_Feet, Technician,
	Thick_Fat, Tinted_Lens, Torrent, Trace, Truant,
	Unaware, Unburden, Vital_Spirit, Volt_Absorb, Water_Absorb,
	Water_Veil, White_Smoke, Wonder_Guard
};

bool blocks_switching(Ability ability, ActivePokemon switcher, Weather weather);

constexpr bool blocks_weather(Ability const ability) {
	switch (ability) {
		case Ability::Air_Lock:
		case Ability::Cloud_Nine:
			return true;
		default:
			return false;
	}
}

bool blocks_status(Ability ability, Statuses status, Weather weather);

constexpr bool blocks_confusion(Ability const ability) {
	return ability == Ability::Own_Tempo;
}

constexpr bool reflects_status(Ability const ability) {
	return ability == Ability::Synchronize;
}

constexpr bool absorbs_poison_damage(Ability const ability) {
	return ability == Ability::Poison_Heal;
}

constexpr bool blocks_burn_damage_penalty(Ability const ability) {
	return ability == Ability::Guts;
}

constexpr bool blocks_paralysis_speed_penalty(Ability const ability) {
	return ability == Ability::Quick_Feet;
}

constexpr bool blocks_sound_moves(Ability const ability) {
	return ability == Ability::Soundproof;
}

constexpr bool can_clear_status(Ability const ability, Status const status) {
	return ability == Ability::Shed_Skin and !is_clear(status);
}

constexpr bool clears_status_on_switch(Ability const ability) {
	return ability == Ability::Natural_Cure;
}

constexpr bool is_immune_to_ground(Ability const ability) {
	return ability == Ability::Levitate;
}

constexpr bool wakes_up_early(Ability const ability) {
	return ability == Ability::Early_Bird;
}

constexpr bool weakens_burn(Ability const ability) {
	return ability == Ability::Heatproof;
}

constexpr bool harms_sleepers(Ability const ability) {
	return ability == Ability::Bad_Dreams;
}

constexpr bool blocks_phazing(Ability const ability) {
	return ability == Ability::Suction_Cups;
}

constexpr bool blocks_recoil(Ability const ability) {
	switch (ability) {
		case Ability::Magic_Guard:
		case Ability::Rock_Head:
			return true;
		default:
			return false;
	}
}

constexpr bool blocks_secondary_damage(Ability const ability) {
	return ability == Ability::Magic_Guard;
}

constexpr bool cannot_miss(Ability const ability) {
	return ability == Ability::No_Guard;
}

constexpr bool damages_leechers(Ability const ability) {
	return ability == Ability::Liquid_Ooze;
}

constexpr bool weakens_se_attacks(Ability const ability) {
	switch (ability) {
		case Ability::Filter:
		case Ability::Solid_Rock:
			return true;
		default:
			return false;
	}
}

constexpr bool strengthens_nve_attacks(Ability const ability) {
	return ability == Ability::Tinted_Lens;
}

constexpr bool uses_extra_pp(Ability const ability) {
	return ability == Ability::Pressure;
}

constexpr bool ignores_blockers(Ability const ability) {
	return ability == Ability::Mold_Breaker;
}

constexpr bool boosts_critical_hits(Ability const ability) {
	return ability == Ability::Sniper;
}

constexpr bool boosts_defense(Ability const ability, Status const status) {
	return ability == Ability::Marvel_Scale and !is_clear(status);
}

bool boosts_special_attack(Ability ability, Weather weather);

bool boosts_special_defense(Ability ability, Weather weather);

constexpr bool boosts_speed(Ability const ability) {
	return ability == Ability::Speed_Boost;
}

constexpr bool boosts_speed_when_flinched(Ability const ability) {
	return ability == Ability::Steadfast;
}

constexpr bool boosts_stab(Ability const ability) {
	return ability == Ability::Adaptability;
}

void activate_ability_on_switch(MutableActivePokemon switcher, MutableActivePokemon other, Weather & weather);
void weather_healing_ability(MutableActivePokemon pokemon, Weather weather);

}	// namespace technicalmachine

namespace std {

template<>
struct numeric_limits<technicalmachine::Ability> : technicalmachine::enum_numeric_limits<technicalmachine::Ability::Wonder_Guard> {};

}	// namespace std
