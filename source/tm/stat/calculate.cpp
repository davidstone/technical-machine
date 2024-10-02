// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

module;

#include <bounded/conditional.hpp>

export module tm.stat.calculate;

import tm.stat.stat_names;

import tm.move.category;
import tm.move.move_name;

import tm.pokemon.any_pokemon;
import tm.pokemon.hp_ratio;
import tm.pokemon.is_type;
import tm.pokemon.species;

import tm.stat.stage;

import tm.status.status;
import tm.status.status_name;

import tm.type.type;

import tm.ability;
import tm.any_team;
import tm.environment;
import tm.generation;
import tm.item;
import tm.rational;
import tm.weather;

import bounded;
import std_module;

namespace technicalmachine {
using namespace bounded::literal;

constexpr auto is_boosted_by_deep_sea_scale(Species const species) {
	return species == Species::Clamperl;
}

constexpr auto is_boosted_by_deep_sea_tooth(Species const species) {
	return species == Species::Clamperl;
}

constexpr auto is_boosted_by_light_ball(Species const species) {
	return species == Species::Pikachu;
}

constexpr auto is_boosted_by_metal_powder(Species const species) {
	return species == Species::Ditto;
}

constexpr auto is_boosted_by_soul_dew(Species const species) {
	switch (species) {
		case Species::Latias:
		case Species::Latios:
			return true;
		default:
			return false;
	}
}

constexpr auto is_boosted_by_thick_club(Species const species) {
	switch (species) {
		case Species::Cubone:
		case Species::Marowak:
			return true;
		default:
			return false;
	}
}

constexpr auto is_boosted_by_guts(Generation const generation, StatusName const status) -> bool {
	switch (status) {
		case StatusName::clear:
			return false;
		case StatusName::freeze:
			return generation <= Generation::four;
		case StatusName::burn:
		case StatusName::paralysis:
		case StatusName::poison:
		case StatusName::toxic:
		case StatusName::sleep:
		case StatusName::rest:
			return true;
	}
}

// TODO: Reduce duplication here vs. in power
template<any_active_pokemon PokemonType>
constexpr auto pinch_ability_activates(Type const ability_type, PokemonType const pokemon, Type const move_type) {
	return generation_from<PokemonType> >= Generation::five and move_type == ability_type and hp_ratio(pokemon) <= rational(1_bi, 3_bi);
}

constexpr auto boosts_special_attack(any_active_pokemon auto const pokemon, Type const move_type, Weather const weather) -> bool {
	auto const ability = pokemon.ability();
	auto pinch_ability = [&](Type const type) {
		return pinch_ability_activates(type, pokemon, move_type);
	};
	switch (ability) {
		case Ability::Solar_Power: return weather == Weather::sun;
		case Ability::Blaze: return pinch_ability(Type::Fire);
		case Ability::Overgrow: return pinch_ability(Type::Grass);
		case Ability::Swarm: return pinch_ability(Type::Bug);
		case Ability::Torrent: return pinch_ability(Type::Water);
		default: return false;
	}
}

constexpr auto boosts_special_defense(Ability const ability, Weather const weather) -> bool {
	return ability == Ability::Flower_Gift and weather == Weather::sun;
}

template<any_active_pokemon PokemonType>
constexpr auto attack_ability_modifier(PokemonType const pokemon, Type const move_type, Weather const weather) {
	constexpr auto denominator = 2_bi;
	auto const numerator = [&]() -> bounded::integer<1, 4> {
		auto const ability = pokemon.ability();
		auto pinch_ability = [&](Type const type) {
			return BOUNDED_CONDITIONAL(pinch_ability_activates(type, pokemon, move_type), 3_bi, denominator);
		};
		switch (ability) {
			case Ability::Blaze: return pinch_ability(Type::Fire);
			case Ability::Overgrow: return pinch_ability(Type::Grass);
			case Ability::Swarm: return pinch_ability(Type::Bug);
			case Ability::Torrent: return pinch_ability(Type::Water);
			case Ability::Flower_Gift: return BOUNDED_CONDITIONAL(
				weather == Weather::sun,
				3_bi,
				denominator
			);
			case Ability::Guts: return BOUNDED_CONDITIONAL(
				is_boosted_by_guts(generation_from<PokemonType>, pokemon.status().name()),
				3_bi,
				denominator
			);
			case Ability::Hustle: return 3_bi;
			case Ability::Huge_Power:
			case Ability::Pure_Power:
				return denominator * 2_bi;
			case Ability::Slow_Start: return BOUNDED_CONDITIONAL(
				pokemon.slow_start_is_active(),
				1_bi,
				denominator
			);
			default: return denominator;
		}
	}();
	return rational(numerator, denominator);
}

constexpr auto defense_ability_modifier(any_active_pokemon auto const pokemon) {
	constexpr auto denominator = 2_bi;
	auto const numerator = BOUNDED_CONDITIONAL(
		boosts_defense(pokemon.ability(), pokemon.status()),
		denominator * 3_bi / 2_bi,
		denominator
	);
	return rational(numerator, denominator);
}

constexpr auto special_attack_ability_modifier(any_active_pokemon auto const pokemon, Type const move_type, Weather const weather) {
	constexpr auto denominator = 2_bi;
	auto const numerator = BOUNDED_CONDITIONAL(
		boosts_special_attack(pokemon, move_type, weather),
		denominator * 3_bi / 2_bi,
		denominator
	);
	return rational(numerator, denominator);
}

constexpr auto special_defense_ability_modifier(any_active_pokemon auto const pokemon, Weather const weather) {
	constexpr auto denominator = 2_bi;
	auto const numerator = BOUNDED_CONDITIONAL(
		boosts_special_defense(pokemon.ability(), weather),
		denominator * 3_bi / 2_bi,
		denominator
	);
	return rational(numerator, denominator);
}

template<SplitSpecialRegularStat stat>
constexpr auto offensive_ability_modifier(any_active_pokemon auto const pokemon, Type const move_type, Weather const weather) {
	if constexpr (stat == SplitSpecialRegularStat::atk) {
		return attack_ability_modifier(pokemon, move_type, weather);
	} else {
		static_assert(stat == SplitSpecialRegularStat::spa);
		return special_attack_ability_modifier(pokemon, move_type, weather);
	}
}

template<SplitSpecialRegularStat stat, any_active_pokemon PokemonType>
constexpr auto item_modifier(PokemonType const pokemon, Environment const environment) {
	constexpr auto generation = generation_from<PokemonType>;
	constexpr auto denominator = 2_bi;
	auto const species = pokemon.species();
	auto const item = pokemon.item(environment);
	auto const numerator = [&] {
		if constexpr (stat == SplitSpecialRegularStat::atk) {
			return [&]() -> bounded::integer<2, 4> {
				switch (item) {
					case Item::Choice_Band:
						return 3_bi;
					case Item::Light_Ball:
						if (generation <= Generation::three) {
							return denominator;
						}
						return BOUNDED_CONDITIONAL(is_boosted_by_light_ball(species), 2_bi * denominator, denominator);
					case Item::Thick_Club:
						return BOUNDED_CONDITIONAL(is_boosted_by_thick_club(species), 2_bi * denominator, denominator);
					default:
						return denominator;
				}
			}();
		} else if constexpr (stat == SplitSpecialRegularStat::def) {
			auto const boosted = item == Item::Metal_Powder and is_boosted_by_metal_powder(species);
			return [&]() -> bounded::integer<2, 4> {
				// TODO: Boost in Generation 3+ only if not transformed
				if (!boosted) {
					return denominator;
				}
				return BOUNDED_CONDITIONAL(generation <= Generation::two, 3_bi, 4_bi);
			}();
		} else if constexpr (stat == SplitSpecialRegularStat::spa) {
			return [&]() -> bounded::integer<2, 4> {
				switch (item) {
					case Item::Soul_Dew:
						return BOUNDED_CONDITIONAL(is_boosted_by_soul_dew(species), 3_bi, denominator);
					case Item::Choice_Specs:
						return 3_bi;
					case Item::Deep_Sea_Tooth:
						return BOUNDED_CONDITIONAL(is_boosted_by_deep_sea_tooth(species), 2_bi * denominator, denominator);
					case Item::Light_Ball:
						return BOUNDED_CONDITIONAL(is_boosted_by_light_ball(species), 2_bi * denominator, denominator);
					default:
						return denominator;
				}
			}();
		} else if constexpr (stat == SplitSpecialRegularStat::spd) {
			return [&]() -> bounded::integer<2, 4> {
				switch (item) {
					case Item::Deep_Sea_Scale:
						return BOUNDED_CONDITIONAL(is_boosted_by_deep_sea_scale(species), 2_bi * denominator, denominator);
					case Item::Metal_Powder:
						return BOUNDED_CONDITIONAL(generation <= Generation::two and is_boosted_by_metal_powder(species), 3_bi, denominator);
					case Item::Soul_Dew:
						return BOUNDED_CONDITIONAL(is_boosted_by_soul_dew(species), 3_bi, denominator);
					default:
						return denominator;
				}
			}();
		}
	}();
	return rational(numerator, denominator);
}

constexpr auto applies_to_physical(SplitSpecialRegularStat const stat) {
	switch (stat) {
		case SplitSpecialRegularStat::atk:
		case SplitSpecialRegularStat::def:
			return true;
		default:
			return false;
	}
}

constexpr auto other_physical_stat(SplitSpecialRegularStat const stat) {
	switch (stat) {
		case SplitSpecialRegularStat::atk: return SplitSpecialRegularStat::def;
		case SplitSpecialRegularStat::def: return SplitSpecialRegularStat::atk;
		default: std::unreachable();
	}
}

constexpr auto determine_initial_stat(SplitSpecialRegularStat const name, any_active_pokemon auto const pokemon) {
	return !applies_to_physical(name) or !pokemon.power_trick_is_active() ?
		pokemon.stat(name) :
		pokemon.stat(other_physical_stat(name));
}

template<SplitSpecialRegularStat stat>
constexpr auto calculate_common_offensive_stat(any_active_pokemon auto const pokemon, Type const move_type, Ability const other_ability, Environment const environment, bool const critical_hit) {
	auto const attack =
		determine_initial_stat(stat, pokemon) *
		modifier<BoostableStat(stat)>(pokemon.stages(), critical_hit) *
		offensive_ability_modifier<stat>(pokemon, move_type, environment.effective_weather(pokemon.ability(), other_ability)) *
		item_modifier<stat>(pokemon, environment);

	return bounded::max(attack, 1_bi);
}

export constexpr auto calculate_attack(any_active_pokemon auto const attacker, Type const move_type, Ability const other_ability, Environment const environment, bool const critical_hit = false) {
	// Cast here because it looks as though the strongest attacker would hold a
	// Light Ball, but because of the restriction on the attacker being Pikachu,
	// it is better to use a Power Trick Shuckle with a Choice Band.
	return bounded::assume_in_range(
		calculate_common_offensive_stat<SplitSpecialRegularStat::atk>(attacker, move_type, other_ability, environment, critical_hit),
		1_bi,
		7368_bi
	);
}


export constexpr auto calculate_special_attack(any_active_pokemon auto const attacker, Type const move_type, Ability const other_ability, Environment const environment, bool const critical_hit = false) {
	// see above comment about Light Ball, except the strongest Special Attack
	// Pokemon is actually a Choice Specs Deoxys-Attack.
	return bounded::assume_in_range(
		calculate_common_offensive_stat<SplitSpecialRegularStat::spa>(attacker, move_type, other_ability, environment, critical_hit),
		1_bi,
		4536_bi
	);
}


export constexpr auto calculate_defense(any_active_pokemon auto const defender, Environment const environment, bool const is_self_ko, bool const critical_hit = false) {
	constexpr auto stat = SplitSpecialRegularStat::def;
	auto const defense =
		determine_initial_stat(stat, defender) *
		modifier<BoostableStat(stat)>(defender.stages(), critical_hit) *
		defense_ability_modifier(defender) *
		item_modifier<stat>(defender, environment);

	// Cast here because it looks as though the strongest defender would hold
	// Metal Powder, but because of the restriction on the attacker being Ditto,
	// it is better to use a Shuckle with no boosting item available.
	return bounded::assume_in_range(
		bounded::max(BOUNDED_CONDITIONAL(is_self_ko, defense / 2_bi, defense), 1_bi),
		1_bi,
		3684_bi
	);
}


template<any_active_pokemon PokemonType>
constexpr auto special_defense_sandstorm_boost(PokemonType const defender, Weather const weather) {
	auto const is_boosted =
		generation_from<PokemonType> >= Generation::four and
		is_type(defender, Type::Rock) and
		weather == Weather::sand;
	return rational(
		BOUNDED_CONDITIONAL(is_boosted, 3_bi, 2_bi),
		2_bi
	);
}

export constexpr auto calculate_special_defense(any_active_pokemon auto const defender, Ability const attacker_ability, Environment const environment, bool const critical_hit = false) {
	constexpr auto stat = SplitSpecialRegularStat::spd;
	auto const weather = environment.effective_weather(defender.ability(), attacker_ability);
	auto const defense =
		defender.stat(stat) *
		modifier<BoostableStat(stat)>(defender.stages(), critical_hit) *
		special_defense_ability_modifier(defender, weather) *
		item_modifier<stat>(defender, environment) *
		special_defense_sandstorm_boost(defender, weather);

	// Cast here because it looks as though the strongest defender would hold
	// Deep Sea Scale, but because of the restriction on the defender being
	// Clamperl, it is better to use a Shuckle with no boosting item available.
	// This also gives more Special Defense than Latias with Soul Dew. It also
	// looks like the best ability would be Flower Gift in the Sun, but this is
	// just as good as Sandstorm's Special Defense boost.
	return bounded::assume_in_range(
		bounded::max(defense, 1_bi),
		1_bi,
		3684_bi
	);
}

} // namespace technicalmachine
