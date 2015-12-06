// Copyright (C) 2015 David Stone
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

#include "enum.hpp"
#include "operators.hpp"
#include "rational.hpp"
#include "status.hpp"
#include "move/base_power.hpp"
#include "stat/stat_names.hpp"

#include <cstdint>

namespace technicalmachine {

struct ActivePokemon;
struct MutableActivePokemon;
struct Move;
struct Weather;

struct Ability {
	enum Abilities : uint8_t {
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
		Water_Veil, White_Smoke, Wonder_Guard, END
	};
	Ability ();
	Ability(Abilities ability);
	Abilities name() const;
	bool is_set () const;
	void set_if_unknown (Abilities ability);
	bool blocks_switching (ActivePokemon switcher, Weather weather) const;
	bool blocks_weather () const;

	template<Statuses status>
	bool blocks_status (Weather weather) const;
	bool blocks_confusion () const;
	bool reflects_status () const;
	bool absorbs_poison_damage () const;
	bool blocks_burn_damage_penalty () const;
	bool blocks_paralysis_speed_penalty () const;
	bool can_clear_status (Status status) const;
	bool clears_status_on_switch () const;
	bool is_immune_to_ground () const;
	bool wakes_up_early () const;
	bool weakens_burn () const;
	
	bool harms_sleepers () const;

	bool blocks_phazing () const;
	bool blocks_recoil () const;
	bool blocks_secondary_damage () const;
	bool blocks_sound_moves () const;
	bool cannot_miss () const;
	bool damages_leechers () const;
	bool weakens_se_attacks () const;
	bool strengthens_nve_attacks () const;
	bool uses_extra_pp() const;
	bool ignores_blockers () const;
	
	bool boosts_critical_hits () const;
	bool boosts_defense (Status status) const;
	bool boosts_special_attack (Weather weather) const;
	bool boosts_special_defense (Weather weather) const;
	bool boosts_speed () const;
	bool boosts_speed_when_flinched () const;
	bool boosts_stab () const;
	
	bool is_loafing (bool loaf) const;
	
	static void activate_on_switch (MutableActivePokemon switcher, MutableActivePokemon other, Weather & weather);
	static void weather_healing(MutableActivePokemon pokemon, Weather weather);
	friend bool operator== (Ability lhs, Ability rhs);
private:
	Abilities m_name;
};

using AbilityAccuracyModifier = bounded_rational<
	bounded::integer<1, 13>,
	bounded::integer<1, 10>
>;
auto ability_accuracy_modifier(ActivePokemon user) -> AbilityAccuracyModifier;

using AbilityEvasionModifier = bounded_rational<
	bounded::integer<1, 4>,
	bounded::integer<1, 5>
>;
auto ability_evasion_modifier(ActivePokemon target, Weather weather) -> AbilityEvasionModifier;

auto attacker_ability_power_modifier(Pokemon const & attacker, Pokemon const & defender, VariableAdjustedBasePower base_power) -> bounded_rational<bounded::integer<1, 6>, bounded::integer<1, 5>>;

}	// namespace technicalmachine

namespace std {

template<>
struct numeric_limits<technicalmachine::Ability::Abilities> : technicalmachine::enum_numeric_limits<technicalmachine::Ability::Abilities> {};

}	// namespace std
