// Move power calculator
// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <tm/move/power.hpp>

#include <tm/move/base_power.hpp>
#include <tm/move/category.hpp>
#include <tm/move/move.hpp>
#include <tm/move/moves.hpp>

#include <tm/stat/calculate.hpp>

#include <tm/pokemon/active_pokemon.hpp>
#include <tm/pokemon/happiness.hpp>
#include <tm/pokemon/pokemon.hpp>
#include <tm/pokemon/species.hpp>

#include <tm/ability.hpp>
#include <tm/gender.hpp>
#include <tm/generation.hpp>
#include <tm/item.hpp>
#include <tm/rational.hpp>
#include <tm/status.hpp>
#include <tm/team.hpp>
#include <tm/weather.hpp>

namespace technicalmachine {
namespace {
using namespace bounded::literal;

template<Generation generation>
auto doubling(ActivePokemon<generation> const attacker, Moves const move, ActivePokemon<generation> const defender, Weather const weather) -> bool {
	// I account for the doubling of the base power for Pursuit in the
	// switching function by simply multiplying the final base power by 2.
	// Regardless of the combination of modifiers, this does not change the
	// final base power. The exception is if the attacker's ally uses Helping
	// Hand, the target uses U-turn, and the attacker uses Pursuit with a
	// slower Pokemon that has Rivalry and a Muscle Band and neither the
	// attacker nor target is genderless. This will cause the base power to be
	// 1 less than it should be.

	if (defender.last_used_move().vanish_doubles_power(generation, move))
		return true;
	switch (move) {
		case Moves::Assurance:
			return defender.damaged();
		case Moves::Avalanche: 
		case Moves::Revenge:
			return attacker.damaged();
		case Moves::Body_Slam:
			return generation >= Generation::six and defender.minimized();
		case Moves::Brine:
			return defender.hp().current() <= defender.hp().max() / 2_bi;
		case Moves::Facade:
			return boosts_facade(attacker.status());
		case Moves::Ice_Ball:
		case Moves::Rollout:
			return attacker.defense_curled();
		case Moves::Payback:
			return defender.last_used_move().moved_this_turn();
		case Moves::Smelling_Salts:
			return boosts_smellingsalt(defender.status());
		case Moves::Solar_Beam: {
			auto const blocks_weather = weather_is_blocked_by_ability(attacker.ability(), defender.ability());
			switch (generation) {
				case Generation::one:
					return true;
				case Generation::two:
					return !weather.rain(blocks_weather);
				case Generation::three:
				case Generation::four:
				case Generation::five:
				case Generation::six:
				case Generation::seven:
				case Generation::eight:
					return !weather.hail(blocks_weather) and !weather.rain(blocks_weather) and !weather.sand(blocks_weather);
			}
		}
		case Moves::Steamroller:
		case Moves::Stomp:
			return defender.minimized();
		case Moves::Wake_Up_Slap:
			return is_sleeping(defender.status());
		case Moves::Weather_Ball: {
			auto const blocks_weather = weather_is_blocked_by_ability(attacker.ability(), defender.ability());
			return weather.hail(blocks_weather) or weather.rain(blocks_weather) or weather.sand(blocks_weather) or weather.sun(blocks_weather);
		}
		default:
			return false;
	}
}


constexpr auto is_boosted_by_adamant_orb(Species const species, Type const move_type) {
	return
		species == Species::Dialga and
		(move_type == Type::Dragon or move_type == Type::Steel);
}

constexpr auto is_boosted_by_griseous_orb(Species const species, Type const move_type) {
	return
		species == Species::Palkia and
		(move_type == Type::Dragon or move_type == Type::Water);
}

constexpr auto is_boosted_by_lustrous_orb(Species const species, Type const move_type) {
	return
		species == Species::Giratina_Origin and
		(move_type == Type::Dragon or move_type == Type::Ghost);
}

constexpr auto is_boosted_by_soul_dew(Generation const generation, Species const species, Type const move_type) {
	return
		generation >= Generation::seven and
		(species == Species::Latias or species == Species::Latios) and
		(move_type == Type::Dragon or move_type == Type::Psychic);
}


constexpr auto item_modifier_denominator = 20_bi;
using ItemModifierNumerator = bounded::integer<20, 24>;
template<Generation generation>
auto item_modifier_numerator(ActivePokemon<generation> const attacker, KnownMove const move, Weather const weather) -> ItemModifierNumerator {
	constexpr auto none = item_modifier_denominator;
	auto type_boost = [=](Type const type) -> ItemModifierNumerator {
		if (move.type != type) {
			return none;
		}
		return BOUNDED_CONDITIONAL(generation <= Generation::three, none * 11_bi / 10_bi, none * 12_bi / 10_bi);
	};
	switch (attacker.item(weather)) {
		case Item::Muscle_Band:
			return BOUNDED_CONDITIONAL(is_physical(generation, move), none * 11_bi / 10_bi, none);
		case Item::Wise_Glasses:
			return BOUNDED_CONDITIONAL(is_special(generation, move), none * 11_bi / 10_bi, none);
		case Item::Insect_Plate:
		case Item::Silver_Powder:
			return type_boost(Type::Bug);
		case Item::Dread_Plate:	
		case Item::Black_Glasses:
			return type_boost(Type::Dark);
		case Item::Draco_Plate:
			return type_boost(Type::Dragon);
		case Item::Dragon_Fang:
			return BOUNDED_CONDITIONAL(generation <= Generation::two, none, type_boost(Type::Dragon));
		case Item::Dragon_Scale:
			return BOUNDED_CONDITIONAL(generation <= Generation::two, type_boost(Type::Dragon), none);
		case Item::Zap_Plate:
		case Item::Magnet:
			return type_boost(Type::Electric);
		case Item::Pixie_Plate:
			return type_boost(Type::Fairy);
		case Item::Fist_Plate:
		case Item::Black_Belt:
			return type_boost(Type::Fighting);
		case Item::Flame_Plate:
		case Item::Charcoal:
			return type_boost(Type::Fire);
		case Item::Sky_Plate:
		case Item::Sharp_Beak:
			return type_boost(Type::Flying);
		case Item::Spooky_Plate:
		case Item::Spell_Tag:
			return type_boost(Type::Ghost);
		case Item::Meadow_Plate:
		case Item::Miracle_Seed:
			return type_boost(Type::Grass);
		case Item::Earth_Plate:
		case Item::Soft_Sand:
			return type_boost(Type::Ground);
		case Item::Icicle_Plate:
		case Item::Never_Melt_Ice:
			return type_boost(Type::Ice);
		case Item::Pink_Bow:
		case Item::Polkadot_Bow:
		case Item::Silk_Scarf:
			return type_boost(Type::Normal);
		case Item::Toxic_Plate:
		case Item::Poison_Barb:
			return type_boost(Type::Poison);
		case Item::Mind_Plate:
		case Item::Twisted_Spoon:
		case Item::Odd_Incense:
			return type_boost(Type::Psychic);
		case Item::Stone_Plate:
		case Item::Hard_Stone:
		case Item::Rock_Incense:
			return type_boost(Type::Rock);
		case Item::Iron_Plate:
		case Item::Metal_Coat:
			return type_boost(Type::Steel);
		case Item::Sea_Incense:
			return BOUNDED_CONDITIONAL(generation <= Generation::three,
				BOUNDED_CONDITIONAL(move.type == Type::Water, none * 21_bi / 20_bi, none),
				type_boost(Type::Water)
			);
		case Item::Splash_Plate:
		case Item::Mystic_Water:
		case Item::Wave_Incense:
			return type_boost(Type::Water);
		case Item::Adamant_Orb:
			return BOUNDED_CONDITIONAL(
				is_boosted_by_adamant_orb(attacker.species(), move.type),
				none * 12_bi / 10_bi,
				none
			);
		case Item::Griseous_Orb:
			return BOUNDED_CONDITIONAL(
				is_boosted_by_griseous_orb(attacker.species(), move.type),
				none * 12_bi / 10_bi,
				none
			);
		case Item::Lustrous_Orb:
			return BOUNDED_CONDITIONAL(
				is_boosted_by_lustrous_orb(attacker.species(), move.type),
				none * 12_bi / 10_bi,
				none
			);
		case Item::Soul_Dew:
			return BOUNDED_CONDITIONAL(
				is_boosted_by_soul_dew(generation, attacker.species(), move.type),
				none * 12_bi / 10_bi,
				none
			);
		default:
			return none;
	}
}

template<Generation generation>
auto item_modifier(ActivePokemon<generation> const attacker, KnownMove const move, Weather const weather) {
	return rational(
		item_modifier_numerator(attacker, move, weather),
		item_modifier_denominator
	);
}

bool is_boosted_by_iron_fist(Moves const move) {
	switch (move) {
		case Moves::Bullet_Punch:
		case Moves::Comet_Punch:
		case Moves::Dizzy_Punch:
		case Moves::Drain_Punch:
		case Moves::Double_Iron_Bash:
		case Moves::Dynamic_Punch:
		case Moves::Fire_Punch:
		case Moves::Focus_Punch:
		case Moves::Hammer_Arm:
		case Moves::Ice_Hammer:
		case Moves::Ice_Punch:
		case Moves::Mach_Punch:
		case Moves::Mega_Punch:
		case Moves::Meteor_Mash:
		case Moves::Plasma_Fists:
		case Moves::Power_Up_Punch:
		case Moves::Shadow_Punch:
		case Moves::Sky_Uppercut:
		case Moves::Thunder_Punch:
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
		case Moves::Head_Charge:
		case Moves::Head_Smash:
		case Moves::High_Jump_Kick:
		case Moves::Jump_Kick:
		case Moves::Light_of_Ruin:
		case Moves::Submission:
		case Moves::Take_Down:
		case Moves::Volt_Tackle:
		case Moves::Wood_Hammer:
		case Moves::Wild_Charge:
			return true;
		default:
			return false;
	}
}

template<Generation generation>
auto attacker_ability_power_modifier(ActivePokemon<generation> const attacker, KnownMove const move, ActivePokemon<generation> const defender, BasePower const base) -> rational<bounded::integer<1, 6>, bounded::integer<1, 5>> {
	auto pinch_ability_activates = [&](Type const type) {
		return generation <= Generation::four and move.type == type and hp_ratio(attacker) <= rational(1_bi, 3_bi);
	};
	switch (attacker.ability()) {
		case Ability::Technician:
			return rational(BOUNDED_CONDITIONAL(base <= 60_bi and move.name != Moves::Hit_Self, 3_bi, 2_bi), 2_bi);
		case Ability::Blaze:
			return rational(BOUNDED_CONDITIONAL(pinch_ability_activates(Type::Fire), 3_bi, 2_bi), 2_bi);
		case Ability::Overgrow:
			return rational(BOUNDED_CONDITIONAL(pinch_ability_activates(Type::Grass), 3_bi, 2_bi), 2_bi);
		case Ability::Swarm:
			return rational(BOUNDED_CONDITIONAL(pinch_ability_activates(Type::Bug), 3_bi, 2_bi), 2_bi);
		case Ability::Torrent:
			return rational(BOUNDED_CONDITIONAL(pinch_ability_activates(Type::Water), 3_bi, 2_bi), 2_bi);
		case Ability::Iron_Fist:
			return rational(BOUNDED_CONDITIONAL(is_boosted_by_iron_fist(move.name), 6_bi, 5_bi), 5_bi);
		case Ability::Reckless:
			return rational(BOUNDED_CONDITIONAL(is_boosted_by_reckless(move.name), 6_bi, 5_bi), 5_bi);
		case Ability::Rivalry:
			return rational(4_bi + multiplier(attacker.gender(), defender.gender()), 4_bi);
		default:
			return rational(1_bi, 1_bi);
	}
}

auto defender_ability_modifier(Type const move_type, Ability const ability) -> rational<bounded::integer<1, 5>, bounded::integer<1, 4>> {
	switch (ability) {
		case Ability::Dry_Skin: return rational(BOUNDED_CONDITIONAL(move_type == Type::Fire, 5_bi, 4_bi), 4_bi);
		case Ability::Heatproof: return rational(1_bi, BOUNDED_CONDITIONAL(move_type == Type::Fire, 2_bi, 1_bi));
		case Ability::Thick_Fat: return rational(1_bi, BOUNDED_CONDITIONAL(move_type == Type::Fire or move_type == Type::Ice, 2_bi, 1_bi));
		default: return rational(1_bi, 1_bi);
	}
}

}	// namespace

template<Generation generation>
auto move_power(Team<generation> const & attacker_team, ExecutedMove<generation> const executed, Team<generation> const & defender_team, Weather const weather) -> MovePower {
	auto const & attacker = attacker_team.pokemon();
	auto const & defender = defender_team.pokemon();
	auto const base = base_power(attacker_team, executed, defender_team, weather);
	return bounded::assume_in_range<MovePower>(bounded::max(1_bi,
		base *
		BOUNDED_CONDITIONAL(doubling(attacker, executed.move.name, defender, weather), 2_bi, 1_bi) *
		item_modifier(attacker, executed.move, weather) *
		BOUNDED_CONDITIONAL(attacker.charge_boosted(executed.move.type), 2_bi, 1_bi) /
		BOUNDED_CONDITIONAL(defender.sport_is_active(executed.move.type), 2_bi, 1_bi) *
		attacker_ability_power_modifier(attacker, executed.move, defender, base) *
		defender_ability_modifier(executed.move.type, defender.ability())
	));
}

#define TECHNICALMACHINE_EXPLICIT_INSTANTIATION(generation) \
	template auto move_power<generation>(Team<generation> const & attacker_team, ExecutedMove<generation> const executed, Team<generation> const & defender_team, Weather const weather) -> MovePower

TECHNICALMACHINE_EXPLICIT_INSTANTIATION(Generation::one);
TECHNICALMACHINE_EXPLICIT_INSTANTIATION(Generation::two);
TECHNICALMACHINE_EXPLICIT_INSTANTIATION(Generation::three);
TECHNICALMACHINE_EXPLICIT_INSTANTIATION(Generation::four);
TECHNICALMACHINE_EXPLICIT_INSTANTIATION(Generation::five);
TECHNICALMACHINE_EXPLICIT_INSTANTIATION(Generation::six);
TECHNICALMACHINE_EXPLICIT_INSTANTIATION(Generation::seven);
TECHNICALMACHINE_EXPLICIT_INSTANTIATION(Generation::eight);

} // namespace technicalmachine
