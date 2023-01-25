// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

module;

#include <tm/for_each_generation.hpp>

#include <bounded/conditional.hpp>

export module tm.move.power;

import tm.move.base_power;
import tm.move.category;
import tm.move.executed_move;
import tm.move.known_move;
import tm.move.move_name;

import tm.pokemon.any_pokemon;
import tm.pokemon.hp_ratio;
import tm.pokemon.pokemon;
import tm.pokemon.species;

import tm.stat.calculate;

import tm.status.status;
import tm.status.status_name;

import tm.type.type;

import tm.ability;
import tm.any_team;
import tm.gender;
import tm.generation;
import tm.item;
import tm.rational;
import tm.team;
import tm.weather;

import bounded;

namespace technicalmachine {
using namespace bounded::literal;

constexpr auto boosts_facade(StatusName const status) -> bool {
	switch (status) {
		case StatusName::burn:
		case StatusName::paralysis:
		case StatusName::poison:
		case StatusName::toxic:
			return true;
		case StatusName::clear:
		case StatusName::freeze:
		case StatusName::sleep:
		case StatusName::rest:
			return false;
	}
}

template<any_active_pokemon AttackerPokemon>
auto doubling(AttackerPokemon const attacker, MoveName const move, any_active_pokemon auto const defender, Weather const weather) -> bool {
	// I account for the doubling of the base power for Pursuit in the
	// switching function by simply multiplying the final base power by 2.
	// Regardless of the combination of modifiers, this does not change the
	// final base power. The exception is if the attacker's ally uses Helping
	// Hand, the target uses U-turn, and the attacker uses Pursuit with a
	// slower Pokemon that has Rivalry and a Muscle Band and neither the
	// attacker nor target is genderless. This will cause the base power to be
	// 1 less than it should be.
	constexpr auto generation = generation_from<AttackerPokemon>;
	if (defender.last_used_move().vanish_doubles_power(generation, move))
		return true;
	switch (move) {
		case MoveName::Assurance:
			return defender.damaged();
		case MoveName::Avalanche:
		case MoveName::Revenge:
			return attacker.damaged();
		case MoveName::Behemoth_Bash:
		case MoveName::Behemoth_Blade:
		case MoveName::Dynamax_Cannon:
			return false;
		case MoveName::Body_Slam:
			return generation >= Generation::six and defender.minimized();
		case MoveName::Bolt_Beak:
		case MoveName::Fishious_Rend: {
			auto const moved = defender.last_used_move();
			return !moved.moved_this_turn() or moved.switched_in_this_turn();
		}
		case MoveName::Brine:
			return defender.hp().current() <= defender.hp().max() / 2_bi;
		case MoveName::Facade:
			return boosts_facade(attacker.status().name());
		case MoveName::Ice_Ball:
		case MoveName::Rollout:
			return attacker.defense_curled();
		case MoveName::Payback:
			return defender.last_used_move().moved_this_turn();
		case MoveName::Smelling_Salts:
			return boosts_smellingsalt(defender.status());
		case MoveName::Solar_Beam: {
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
		case MoveName::Steamroller:
		case MoveName::Stomp:
			return defender.minimized();
		case MoveName::Wake_Up_Slap:
			return is_sleeping(defender.status());
		case MoveName::Weather_Ball: {
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
template<any_active_pokemon AttackerPokemon>
auto item_modifier_numerator(AttackerPokemon const attacker, KnownMove const move, Weather const weather) -> ItemModifierNumerator {
	constexpr auto generation = generation_from<AttackerPokemon>;
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

auto item_modifier(any_active_pokemon auto const attacker, KnownMove const move, Weather const weather) {
	return rational(
		item_modifier_numerator(attacker, move, weather),
		item_modifier_denominator
	);
}

bool is_boosted_by_iron_fist(MoveName const move) {
	switch (move) {
		case MoveName::Bullet_Punch:
		case MoveName::Comet_Punch:
		case MoveName::Dizzy_Punch:
		case MoveName::Drain_Punch:
		case MoveName::Dynamic_Punch:
		case MoveName::Fire_Punch:
		case MoveName::Focus_Punch:
		case MoveName::Hammer_Arm:
		case MoveName::Ice_Hammer:
		case MoveName::Ice_Punch:
		case MoveName::Mach_Punch:
		case MoveName::Mega_Punch:
		case MoveName::Meteor_Mash:
		case MoveName::Plasma_Fists:
		case MoveName::Power_Up_Punch:
		case MoveName::Shadow_Punch:
		case MoveName::Sky_Uppercut:
		case MoveName::Thunder_Punch:
			return true;
		default:
			return false;
	}
}

bool is_boosted_by_reckless(MoveName const move) {
	switch (move) {
		case MoveName::Brave_Bird:
		case MoveName::Double_Edge:
		case MoveName::Flare_Blitz:
		case MoveName::Head_Charge:
		case MoveName::Head_Smash:
		case MoveName::High_Jump_Kick:
		case MoveName::Jump_Kick:
		case MoveName::Light_of_Ruin:
		case MoveName::Submission:
		case MoveName::Take_Down:
		case MoveName::Volt_Tackle:
		case MoveName::Wood_Hammer:
		case MoveName::Wild_Charge:
			return true;
		default:
			return false;
	}
}

template<any_active_pokemon AttackerPokemon>
auto attacker_ability_power_modifier(AttackerPokemon const attacker, KnownMove const move, any_active_pokemon auto const defender, BasePower const base) -> rational<bounded::integer<1, 6>, bounded::integer<1, 5>> {
	auto pinch_ability_activates = [&](Type const type) {
		return generation_from<AttackerPokemon> <= Generation::four and move.type == type and hp_ratio(attacker) <= rational(1_bi, 3_bi);
	};
	switch (attacker.ability()) {
		case Ability::Technician:
			return rational(BOUNDED_CONDITIONAL(base <= 60_bi and move.name != MoveName::Hit_Self, 3_bi, 2_bi), 2_bi);
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

export using MovePower = bounded::integer<1, 1440>;

// If a damaging move does not have power (for instance, OHKO moves and
// fixed-damage moves), the behavior of this function is undefined. If
// `executed.move.name` is Hidden Power, `attacker.pokemon().hidden_power()`
// must not be `none`.
export template<any_team UserTeam, any_team DefenderTeam>
auto move_power(UserTeam const & attacker_team, ExecutedMove<UserTeam> const executed, DefenderTeam const & defender_team, Weather const weather) -> MovePower {
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

#define TECHNICALMACHINE_EXPLICIT_INSTANTIATION_IMPL(UserTeam, DefenderTeam) \
	template auto move_power(UserTeam const & attacker_team, ExecutedMove<UserTeam> const executed, DefenderTeam const & defender_team, Weather const weather) -> MovePower

#define TECHNICALMACHINE_EXPLICIT_INSTANTIATION(generation) \
	TECHNICALMACHINE_EXPLICIT_INSTANTIATION_IMPL(Team<generation>, Team<generation>); \
	TECHNICALMACHINE_EXPLICIT_INSTANTIATION_IMPL(KnownTeam<generation>, KnownTeam<generation>); \
	TECHNICALMACHINE_EXPLICIT_INSTANTIATION_IMPL(KnownTeam<generation>, SeenTeam<generation>); \
	TECHNICALMACHINE_EXPLICIT_INSTANTIATION_IMPL(SeenTeam<generation>, KnownTeam<generation>); \
	TECHNICALMACHINE_EXPLICIT_INSTANTIATION_IMPL(SeenTeam<generation>, SeenTeam<generation>)

TECHNICALMACHINE_FOR_EACH_GENERATION(TECHNICALMACHINE_EXPLICIT_INSTANTIATION);

} // namespace technicalmachine
