// Copyright (C) 2016 David Stone
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
#include "move/moves.hpp"

#include "pokemon/active_pokemon.hpp"

#include "stat/calculate.hpp"

#include "type/type.hpp"

namespace technicalmachine {

bool Ability::is_set() const {
	return name() != END;
}

void Ability::set_if_unknown(Abilities const ability) {
	assert(ability < END);
	if (!is_set())
		m_name = ability;
}

bool Ability::blocks_switching(ActivePokemon const switcher, Weather const weather) const {
	switch (name()) {
		case Shadow_Tag:
			return get_ability(switcher).name() != Shadow_Tag;
		case Arena_Trap:
			return grounded(switcher, weather);
		case Magnet_Pull:
			return is_type(switcher, Type::Steel, is_roosting(switcher));
		default:
			return false;
	}
}

bool Ability::blocks_weather() const {
	switch (name()) {
		case Air_Lock:
		case Cloud_Nine:
			return true;
		default:
			return false;
	}
}

bool Ability::blocks_status(Statuses const status, Weather const weather) const {
	switch (status) {
	case Statuses::burn:
		switch (name()) {
		case Leaf_Guard:
			return weather.sun();
		case Water_Veil:
			return true;
		default:
			return false;
		}
	case Statuses::freeze:
		return name() == Magma_Armor;
	case Statuses::paralysis:
		switch (name()) {
		case Leaf_Guard:
			return weather.sun();
		case Limber:
			return true;
		default:
			return false;
		}
	case Statuses::poison:
	case Statuses::poison_toxic:
		switch (name()) {
		case Immunity:
			return true;
		case Leaf_Guard:
			return weather.sun();
		default:
			return false;
		}
	case Statuses::sleep:
	case Statuses::sleep_rest:
		switch (name()) {
			case Insomnia:
			case Vital_Spirit:
				return true;
			case Leaf_Guard:
				return weather.sun();
			default:
				return false;
		}
	case Statuses::clear:
	case Statuses::END:
		return false;
	}
}

bool Ability::blocks_confusion() const {
	return name() == Own_Tempo;
}

bool Ability::reflects_status() const {
	return name() == Synchronize;
}

bool Ability::absorbs_poison_damage() const {
	return name() == Poison_Heal;
}

bool Ability::blocks_burn_damage_penalty() const {
	return name() == Guts;
}

bool Ability::blocks_paralysis_speed_penalty() const {
	return name() == Quick_Feet;
}

bool Ability::blocks_sound_moves() const {
	return name() == Soundproof;
}

bool Ability::can_clear_status(Status const status) const {
	return name() == Shed_Skin and !is_clear(status);
}

bool Ability::clears_status_on_switch() const {
	return name() == Natural_Cure;
}

bool Ability::is_immune_to_ground() const {
	return name() == Levitate;
}

bool Ability::wakes_up_early() const {
	return name() == Early_Bird;
}

bool Ability::weakens_burn() const {
	return name() == Heatproof;
}

bool Ability::harms_sleepers() const {
	return name() == Bad_Dreams;
}

bool Ability::blocks_phazing() const {
	return name() == Suction_Cups;
}

bool Ability::blocks_recoil() const {
	switch (name()) {
		case Magic_Guard:
		case Rock_Head:
			return true;
		default:
			return false;
	}
}

bool Ability::blocks_secondary_damage() const {
	return name() == Magic_Guard;
}

bool Ability::cannot_miss() const {
	return name() == No_Guard;
}

bool Ability::damages_leechers() const {
	return name() == Liquid_Ooze;
}

bool Ability::weakens_se_attacks() const {
	switch (name()) {
		case Filter:
		case Solid_Rock:
			return true;
		default:
			return false;
	}
}

bool Ability::strengthens_nve_attacks() const {
	return name() == Tinted_Lens;
}

bool Ability::uses_extra_pp() const {
	return name() == Pressure;
}

bool Ability::ignores_blockers() const {
	return name() == Mold_Breaker;
}

bool Ability::boosts_critical_hits() const {
	return name() == Sniper;
}

bool Ability::boosts_defense(Status const status) const {
	return name() == Marvel_Scale and !is_clear(status);
}

bool Ability::boosts_special_attack(Weather const weather) const {
	return name() == Solar_Power and weather.sun();
}

bool Ability::boosts_special_defense(Weather const weather) const {
	return name() == Flower_Gift and weather.sun();
}

bool Ability::boosts_speed() const {
	return name() == Speed_Boost;
}

bool Ability::boosts_speed_when_flinched() const {
	return name() == Steadfast;
}

bool Ability::boosts_stab() const {
	return name() == Adaptability;
}

bool Ability::is_loafing(bool const loaf) const {
	return name() == Truant and loaf;
}

auto ability_accuracy_modifier(ActivePokemon const user, Moves const move) -> AbilityAccuracyModifier {
	switch (get_ability(user).name()) {
		case Ability::Compoundeyes:
			return AbilityAccuracyModifier(13_bi, 10_bi);
		case Ability::Hustle:
			return is_physical(move) ? AbilityAccuracyModifier(4_bi, 5_bi) : AbilityAccuracyModifier(1_bi, 1_bi);
		default:
			return AbilityAccuracyModifier(1_bi, 1_bi);
	}
}

auto ability_evasion_modifier(ActivePokemon const target, Weather const weather) -> AbilityEvasionModifier {
	switch (get_ability(target).name()) {
		case Ability::Sand_Veil:
			return weather.sand() ? AbilityEvasionModifier(4_bi, 5_bi) : AbilityEvasionModifier(1_bi, 1_bi);
		case Ability::Snow_Cloak:
			return weather.hail() ? AbilityEvasionModifier(4_bi, 5_bi) : AbilityEvasionModifier(1_bi, 1_bi);
		case Ability::Tangled_Feet:
			return is_confused(target) ? AbilityEvasionModifier(4_bi, 5_bi) : AbilityEvasionModifier(1_bi, 1_bi);
		default:
			return AbilityEvasionModifier(1_bi, 1_bi);
	}
}

namespace {

bool is_boosted_by_iron_fist(Moves const move) {
	switch (move) {
		case Moves::Bullet_Punch:
		case Moves::Comet_Punch:
		case Moves::Dizzy_Punch:
		case Moves::Drain_Punch:
		case Moves::DynamicPunch:
		case Moves::Fire_Punch:
		case Moves::Focus_Punch:
		case Moves::Hammer_Arm:
		case Moves::Ice_Punch:
		case Moves::Mach_Punch:
		case Moves::Mega_Punch:
		case Moves::Meteor_Mash:
		case Moves::Shadow_Punch:
		case Moves::Sky_Uppercut:
		case Moves::ThunderPunch:
			return true;
		default:
			return false;
	}
}

bool is_boosted_by_reckless(Moves const move) {
	switch (move) {
		case Moves::Brave_Bird:
		case Moves::Double_Edge:
		case Moves::Flare_Blitz:
		case Moves::Head_Smash:
		case Moves::Submission:
		case Moves::Take_Down:
		case Moves::Volt_Tackle:
		case Moves::Wood_Hammer:
			return true;
		default:
			return false;
	}
}

auto pinch_ability_activates(Pokemon const & attacker, Type const type, Moves const move) {
	return get_type(move, attacker) == type and hp_ratio(attacker) <= make_rational(1_bi, 3_bi);
}

}	// namespace

auto attacker_ability_power_modifier(Pokemon const & attacker, Moves const move, Pokemon const & defender, VariableAdjustedBasePower const base_power) -> bounded_rational<bounded::integer<1, 6>, bounded::integer<1, 5>> {
	switch (get_ability(attacker).name()) {
		case Ability::Technician:
			return make_rational(BOUNDED_CONDITIONAL(base_power <= 60_bi, 3_bi, 2_bi), 2_bi);
		case Ability::Blaze:
			return make_rational(BOUNDED_CONDITIONAL(pinch_ability_activates(attacker, Type::Fire, move), 3_bi, 2_bi), 2_bi);
		case Ability::Overgrow:
			return make_rational(BOUNDED_CONDITIONAL(pinch_ability_activates(attacker, Type::Grass, move), 3_bi, 2_bi), 2_bi);
		case Ability::Swarm:
			return make_rational(BOUNDED_CONDITIONAL(pinch_ability_activates(attacker, Type::Bug, move), 3_bi, 2_bi), 2_bi);
		case Ability::Torrent:
			return make_rational(BOUNDED_CONDITIONAL(pinch_ability_activates(attacker, Type::Water, move), 3_bi, 2_bi), 2_bi);
		case Ability::Iron_Fist:
			return make_rational(BOUNDED_CONDITIONAL(is_boosted_by_iron_fist(move), 6_bi, 5_bi), 5_bi);
		case Ability::Reckless:
			return make_rational(BOUNDED_CONDITIONAL(is_boosted_by_reckless(move), 6_bi, 5_bi), 5_bi);
		case Ability::Rivalry:
			return make_rational(4_bi + multiplier(get_gender(attacker), get_gender(defender)), 4_bi);
		default:
			return make_rational(1_bi, 1_bi);
	}
}

void Ability::activate_on_switch(MutableActivePokemon switcher, MutableActivePokemon other, Weather & weather) {
	switch (get_ability(switcher).name()) {
		case Download: {
			auto const defense = calculate_defense(other, weather);
			auto const special_defense = calculate_special_defense(other, weather);
			boost(stage(switcher), defense >= special_defense ? StatNames::SPA : StatNames::ATK, 1_bi);
			break;
		}
		case Drizzle:
			weather.activate_rain(Weather::Duration::permanent);
			break;
		case Drought:
			weather.activate_sun(Weather::Duration::permanent);
			break;
		case Forecast:	// TODO: fix this
			break;
		case Intimidate:
			boost(stage(other), StatNames::ATK, -1_bi);
			break;
		case Sand_Stream:
			weather.activate_sand(Weather::Duration::permanent);
			break;
		case Snow_Warning:
			weather.activate_hail(Weather::Duration::permanent);
			break;
		case Trace:
			break;
		default:
			break;
	}
}

void Ability::weather_healing(MutableActivePokemon pokemon, Weather const weather) {
	switch (get_ability(pokemon).name()) {
		case Dry_Skin:
			if (weather.rain()) {
				heal(pokemon, make_rational(1_bi, 8_bi));
			} else if (weather.sun()) {
				heal(pokemon, make_rational(-1_bi, 8_bi));
			}
			break;
		case Hydration:
			if (weather.rain()) {
				get_status(pokemon) = Status{};
			}
			break;
		case Ice_Body:
			if (weather.hail()) {
				heal(pokemon, make_rational(1_bi, 16_bi));
			}
			break;
		case Rain_Dish:
			if (weather.rain()) {
				heal(pokemon, make_rational(1_bi, 16_bi));
			}
			break;
		default:
			break;
	}
}

}	// namespace technicalmachine
