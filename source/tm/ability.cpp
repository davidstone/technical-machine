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

#include <tm/ability.hpp>

#include <cassert>

#include <tm/heal.hpp>
#include <tm/rational.hpp>
#include <tm/status.hpp>
#include <tm/weather.hpp>

#include <tm/move/move.hpp>
#include <tm/move/moves.hpp>

#include <tm/pokemon/active_pokemon.hpp>

#include <tm/stat/calculate.hpp>

#include <tm/type/type.hpp>

namespace technicalmachine {

bool blocks_switching(Ability const ability, ActivePokemon const switcher, Weather const weather) {
	switch (ability) {
		case Ability::Shadow_Tag:
			return get_ability(switcher) != Ability::Shadow_Tag;
		case Ability::Arena_Trap:
			return grounded(switcher, weather);
		case Ability::Magnet_Pull:
			return is_type(switcher, Type::Steel, is_roosting(switcher));
		default:
			return false;
	}
}

bool blocks_status(Ability const ability, Statuses const status, Weather const weather) {
	switch (status) {
	case Statuses::burn:
		switch (ability) {
		case Ability::Leaf_Guard:
			return weather.sun();
		case Ability::Water_Veil:
			return true;
		default:
			return false;
		}
	case Statuses::freeze:
		return ability == Ability::Magma_Armor;
	case Statuses::paralysis:
		switch (ability) {
		case Ability::Leaf_Guard:
			return weather.sun();
		case Ability::Limber:
			return true;
		default:
			return false;
		}
	case Statuses::poison:
	case Statuses::poison_toxic:
		switch (ability) {
		case Ability::Immunity:
			return true;
		case Ability::Leaf_Guard:
			return weather.sun();
		default:
			return false;
		}
	case Statuses::sleep:
	case Statuses::sleep_rest:
		switch (ability) {
			case Ability::Insomnia:
			case Ability::Vital_Spirit:
				return true;
			case Ability::Leaf_Guard:
				return weather.sun();
			default:
				return false;
		}
	case Statuses::clear:
		return false;
	}
}

bool boosts_special_attack(Ability const ability, Weather const weather) {
	return ability == Ability::Solar_Power and weather.sun();
}

bool boosts_special_defense(Ability const ability, Weather const weather) {
	return ability == Ability::Flower_Gift and weather.sun();
}

auto ability_accuracy_modifier(ActivePokemon const user, Moves const move) -> AbilityAccuracyModifier {
	switch (get_ability(user)) {
		case Ability::Compoundeyes:
			return AbilityAccuracyModifier(13_bi, 10_bi);
		case Ability::Hustle:
			return is_physical(move) ? AbilityAccuracyModifier(4_bi, 5_bi) : AbilityAccuracyModifier(1_bi, 1_bi);
		default:
			return AbilityAccuracyModifier(1_bi, 1_bi);
	}
}

auto ability_evasion_modifier(ActivePokemon const target, Weather const weather) -> AbilityEvasionModifier {
	switch (get_ability(target)) {
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
	return get_type(move, attacker) == type and hp_ratio(attacker) <= rational(1_bi, 3_bi);
}

}	// namespace

auto attacker_ability_power_modifier(Pokemon const & attacker, Moves const move, Pokemon const & defender, VariableAdjustedBasePower const base_power) -> rational<bounded::integer<1, 6>, bounded::integer<1, 5>> {
	switch (get_ability(attacker)) {
		case Ability::Technician:
			return rational(BOUNDED_CONDITIONAL(base_power <= 60_bi, 3_bi, 2_bi), 2_bi);
		case Ability::Blaze:
			return rational(BOUNDED_CONDITIONAL(pinch_ability_activates(attacker, Type::Fire, move), 3_bi, 2_bi), 2_bi);
		case Ability::Overgrow:
			return rational(BOUNDED_CONDITIONAL(pinch_ability_activates(attacker, Type::Grass, move), 3_bi, 2_bi), 2_bi);
		case Ability::Swarm:
			return rational(BOUNDED_CONDITIONAL(pinch_ability_activates(attacker, Type::Bug, move), 3_bi, 2_bi), 2_bi);
		case Ability::Torrent:
			return rational(BOUNDED_CONDITIONAL(pinch_ability_activates(attacker, Type::Water, move), 3_bi, 2_bi), 2_bi);
		case Ability::Iron_Fist:
			return rational(BOUNDED_CONDITIONAL(is_boosted_by_iron_fist(move), 6_bi, 5_bi), 5_bi);
		case Ability::Reckless:
			return rational(BOUNDED_CONDITIONAL(is_boosted_by_reckless(move), 6_bi, 5_bi), 5_bi);
		case Ability::Rivalry:
			return rational(4_bi + multiplier(get_gender(attacker), get_gender(defender)), 4_bi);
		default:
			return rational(1_bi, 1_bi);
	}
}

void activate_ability_on_switch(MutableActivePokemon switcher, MutableActivePokemon other, Weather & weather) {
	switch (get_ability(switcher)) {
		case Ability::Download: {
			auto const defense = calculate_defense(other, weather);
			auto const special_defense = calculate_special_defense(other, weather);
			boost(stage(switcher), defense >= special_defense ? StatNames::SPA : StatNames::ATK, 1_bi);
			break;
		}
		case Ability::Drizzle:
			weather.activate_rain(Weather::permanent);
			break;
		case Ability::Drought:
			weather.activate_sun(Weather::permanent);
			break;
		case Ability::Forecast:	// TODO: fix this
			break;
		case Ability::Intimidate:
			boost(stage(other), StatNames::ATK, -1_bi);
			break;
		case Ability::Sand_Stream:
			weather.activate_sand(Weather::permanent);
			break;
		case Ability::Snow_Warning:
			weather.activate_hail(Weather::permanent);
			break;
		case Ability::Trace:
			break;
		default:
			break;
	}
}

void weather_healing_ability(MutableActivePokemon pokemon, Weather const weather) {
	switch (get_ability(pokemon)) {
		case Ability::Dry_Skin:
			if (weather.rain()) {
				heal(pokemon, rational(1_bi, 8_bi));
			} else if (weather.sun()) {
				heal(pokemon, rational(-1_bi, 8_bi));
			}
			break;
		case Ability::Hydration:
			if (weather.rain()) {
				get_status(pokemon) = Status{};
			}
			break;
		case Ability::Ice_Body:
			if (weather.hail()) {
				heal(pokemon, rational(1_bi, 16_bi));
			}
			break;
		case Ability::Rain_Dish:
			if (weather.rain()) {
				heal(pokemon, rational(1_bi, 16_bi));
			}
			break;
		default:
			break;
	}
}

}	// namespace technicalmachine