// Calculate a Pokemon's current stat
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

#include <tm/stat/calculate.hpp>

#include <tm/stat/nature.hpp>
#include <tm/stat/stat_names.hpp>

#include <tm/ability.hpp>
#include <tm/item.hpp>
#include <tm/rational.hpp>
#include <tm/status.hpp>
#include <tm/team.hpp>
#include <tm/weather.hpp>

#include <tm/move/category.hpp>
#include <tm/move/moves.hpp>
#include <tm/move/priority.hpp>

#include <tm/pokemon/active_pokemon.hpp>
#include <tm/pokemon/level.hpp>
#include <tm/pokemon/pokemon.hpp>
#include <tm/pokemon/species.hpp>

#include <bounded/assert.hpp>

#include <utility>

namespace technicalmachine {
namespace {

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

constexpr auto is_boosted_by_quick_powder(Species const species) {
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

bool is_boosted_by_guts(Generation const generation, Statuses const status) {
	switch (status) {
		case Statuses::clear:
			return false;
		case Statuses::freeze:
			return generation <= Generation::four;
		case Statuses::burn:
		case Statuses::paralysis:
		case Statuses::poison:
		case Statuses::toxic:
		case Statuses::sleep:
		case Statuses::rest:
			return true;
	}
}

// TODO: Reduce duplication here vs. in power
auto pinch_ability_activates(Generation const generation, Type const ability_type, ActivePokemon const pokemon, Moves const move) {
	return generation >= Generation::five and get_type(generation, move, get_hidden_power(pokemon).type()) == ability_type and hp_ratio(pokemon) <= rational(1_bi, 3_bi);
}

bool boosts_special_attack(Generation const generation, ActivePokemon const pokemon, Moves const move, Ability const other_ability, Weather const weather) {
	auto const ability = pokemon.ability();
	auto pinch_ability = [&](Type const type) {
		return pinch_ability_activates(generation, type, pokemon, move);
	};
	switch (ability) {
		case Ability::Solar_Power: return weather.sun(weather_is_blocked_by_ability(ability, other_ability));
		case Ability::Blaze: return pinch_ability(Type::Fire);
		case Ability::Overgrow: return pinch_ability(Type::Grass);
		case Ability::Swarm: return pinch_ability(Type::Bug);
		case Ability::Torrent: return pinch_ability(Type::Water);
		default: return false;
	}
}

bool boosts_special_defense(Ability const ability, Ability const other_ability, Weather const weather) {
	return ability == Ability::Flower_Gift and weather.sun(weather_is_blocked_by_ability(ability, other_ability));
}


template<StatNames stat>
auto ability_modifier(Generation const generation, ActivePokemon const pokemon, Moves const move, Ability const other_ability, Weather const weather) {
	constexpr auto denominator = 2_bi;
	auto const numerator = [&]{
		auto const ability = pokemon.ability();
		if constexpr (stat == StatNames::ATK) {
			return [&]() -> bounded::integer<1, 4> {
				auto pinch_ability = [&](Type const type) {
					return BOUNDED_CONDITIONAL(pinch_ability_activates(generation, type, pokemon, move), 3_bi, denominator);
				};
				switch (ability) {
				case Ability::Blaze: return pinch_ability(Type::Fire);
				case Ability::Overgrow: return pinch_ability(Type::Grass);
				case Ability::Swarm: return pinch_ability(Type::Bug);
				case Ability::Torrent: return pinch_ability(Type::Water);
				case Ability::Flower_Gift: return BOUNDED_CONDITIONAL(weather.sun(weather_is_blocked_by_ability(ability, other_ability)), 3_bi, denominator);
				case Ability::Guts: return BOUNDED_CONDITIONAL(is_boosted_by_guts(generation, get_status(pokemon).name()), 3_bi, denominator);
				case Ability::Hustle: return 3_bi;
				case Ability::Huge_Power:
				case Ability::Pure_Power:
					return denominator * 2_bi;
				case Ability::Slow_Start: return BOUNDED_CONDITIONAL(pokemon.slow_start_is_active(), 1_bi, denominator);
				default: return denominator;
				}
			}();
		} else if constexpr (stat == StatNames::DEF) {
			return BOUNDED_CONDITIONAL(boosts_defense(ability, get_status(pokemon)), 3_bi, denominator);
		} else if constexpr (stat == StatNames::SPA) {
			return BOUNDED_CONDITIONAL(boosts_special_attack(generation, pokemon, move, other_ability, weather), 3_bi, denominator);
		} else if constexpr (stat == StatNames::SPD) {
			return BOUNDED_CONDITIONAL(boosts_special_defense(ability, other_ability, weather), 3_bi, denominator);
		} else if constexpr (stat == StatNames::SPE) {
			return [&]() -> bounded::integer<1, 4> {
				switch (ability) {
				case Ability::Chlorophyll: return BOUNDED_CONDITIONAL(weather.sun(weather_is_blocked_by_ability(ability, other_ability)), denominator * 2_bi, denominator);
				case Ability::Swift_Swim: return BOUNDED_CONDITIONAL(weather.rain(weather_is_blocked_by_ability(ability, other_ability)), denominator * 2_bi, denominator);
				case Ability::Unburden: return BOUNDED_CONDITIONAL(pokemon.is_unburdened(), denominator * 2_bi, denominator);
				case Ability::Quick_Feet: return BOUNDED_CONDITIONAL(!is_clear(get_status(pokemon)), 3_bi, denominator);
				case Ability::Slow_Start: return BOUNDED_CONDITIONAL(pokemon.slow_start_is_active(), 1_bi, denominator);
				default: return denominator;
				}
			}();
		}
	}();
	return rational(numerator, denominator);
}


template<StatNames stat>
auto item_modifier(Generation const generation, ActivePokemon const pokemon, Weather const weather) {
	constexpr auto denominator = 2_bi;
	auto const species [[maybe_unused]] = get_species(pokemon);
	auto const item = pokemon.item(generation, weather);
	auto const numerator = [&]{
		if constexpr (stat == StatNames::ATK) {
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
		} else if constexpr (stat == StatNames::DEF) {
			auto const boosted = item == Item::Metal_Powder and is_boosted_by_metal_powder(species);
			return [&]() -> bounded::integer<2, 4> {
				// TODO: Boost in Generation 3+ only if not transformed
				if (!boosted) {
					return denominator;
				}
				return BOUNDED_CONDITIONAL(generation <= Generation::two, 3_bi, 4_bi);
			}();
		} else if constexpr (stat == StatNames::SPA) {
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
		} else if constexpr (stat == StatNames::SPD) {
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
		} else if constexpr (stat == StatNames::SPE) {
			return [&]() -> bounded::integer<1, 4> {
				switch (item) {
				case Item::Quick_Powder:
					return BOUNDED_CONDITIONAL(is_boosted_by_quick_powder(species), 2_bi * denominator, denominator);
				case Item::Choice_Scarf:
					return 3_bi;
				case Item::Iron_Ball:
				case Item::Macho_Brace:
				case Item::Power_Anklet:
				case Item::Power_Band:
				case Item::Power_Belt:
				case Item::Power_Bracer:
				case Item::Power_Lens:
				case Item::Power_Weight:
					return 1_bi;
				default:
					return denominator;
				}
			}();
		}
	}();
	return rational(numerator, denominator);
}


constexpr auto is_physical(StatNames const stat) {
	switch (stat) {
	case StatNames::ATK:
	case StatNames::DEF:
		return true;
	default:
		return false;
	}
}

constexpr auto other_physical_stat(StatNames const stat) {
	switch (stat) {
		case StatNames::ATK: return StatNames::DEF;
		case StatNames::DEF: return StatNames::ATK;
		default: bounded::unreachable();
	}
}

auto calculate_initial_stat(StatNames const stat, ActivePokemon const pokemon) {
	auto const level = get_level(pokemon);
	auto const nature = get_nature(pokemon);
	return !is_physical(stat) or !pokemon.power_trick_is_active() ?
		initial_stat(stat, get_stat(pokemon, stat), level, nature) :
		initial_stat(other_physical_stat(stat), get_stat(pokemon, other_physical_stat(stat)), level, nature);
}

template<StatNames stat>
auto calculate_common_offensive_stat(Generation const generation, ActivePokemon const pokemon, Moves const move, Ability const other_ability, Weather const weather, bool const critical_hit) {
	auto const attack = calculate_initial_stat(stat, pokemon) *
		modifier<stat>(pokemon.stage(), critical_hit) *
		ability_modifier<stat>(generation, pokemon, move, other_ability, weather) *
		item_modifier<stat>(generation, pokemon, weather);
	
	return bounded::max(attack, 1_bi);
}

}	// namespace

auto calculate_attack(Generation const generation, ActivePokemon const attacker, Moves const move, Ability const other_ability, Weather const weather, bool const critical_hit) -> attack_type {
	// static_cast here because it looks as though the strongest attacker would
	// hold a Light Ball, but because of the restriction on the attacker being
	// Pikachu, it is better to use a Power Trick Shuckle with a Choice Band.
	return static_cast<attack_type>(calculate_common_offensive_stat<StatNames::ATK>(generation, attacker, move, other_ability, weather, critical_hit));
}

auto calculate_special_attack(Generation const generation, ActivePokemon const attacker, Moves const move, Ability const other_ability, Weather const weather, bool const critical_hit) -> special_attack_type {
	// see above comment about Light Ball, except the strongest Special Attack
	// Pokemon is actually a Choice Specs Deoxys-Attack.
	return static_cast<special_attack_type>(calculate_common_offensive_stat<StatNames::SPA>(generation, attacker, move, other_ability, weather, critical_hit));
}

namespace {

constexpr auto is_self_KO(Moves const move) {
	switch (move) {
		case Moves::Explosion:
		case Moves::Self_Destruct:
			return true;
		default:
			return false;
	}
}

}	// namespace

auto calculate_defense(Generation const generation, ActivePokemon const defender, Moves const move, Ability const other_ability, Weather const weather, bool const critical_hit) -> defense_type {
	constexpr auto stat = StatNames::DEF;
	auto const defense = calculate_initial_stat(stat, defender) *
		modifier<stat>(defender.stage(), critical_hit) *
		ability_modifier<stat>(generation, defender, move, other_ability, weather) *
		item_modifier<stat>(generation, defender, weather);
	
	// static_cast here because it looks as though the strongest defender would
	// hold Metal Powder, but because of the restriction on the attacker being
	// Ditto, it is better to use a Shuckle with no boosting item available.
	return static_cast<defense_type>(bounded::max(BOUNDED_CONDITIONAL(is_self_KO(move), defense / 2_bi, defense), 1_bi));
}

namespace {

auto special_defense_sandstorm_boost(Generation const generation, ActivePokemon const defender, Ability const attacker_ability, Weather const weather) {
	auto const blocks_weather = weather_is_blocked_by_ability(defender.ability(), attacker_ability);
	return rational(BOUNDED_CONDITIONAL(is_type(defender, Type::Rock) and weather.sand(blocks_weather) and generation >= Generation::four, 3_bi, 2_bi), 2_bi);
}

}	// namespace

auto calculate_special_defense(Generation const generation, ActivePokemon const defender, Ability const attacker_ability, Weather const weather, bool const critical_hit) -> special_defense_type {
	constexpr auto stat = StatNames::SPD;
	auto const defense = calculate_initial_stat(stat, defender) *	
		modifier<stat>(defender.stage(), critical_hit) *
		ability_modifier<stat>(generation, defender, Moves(), attacker_ability, weather) *
		item_modifier<stat>(generation, defender, weather) *
		special_defense_sandstorm_boost(generation, defender, attacker_ability, weather);
	
	// static_cast here because it looks as though the strongest defender would
	// hold Deep Sea Scale, but because of the restriction on the defender being
	// Clamperl, it is better to use a Shuckle with no boosting item available.
	// This also gives more Special Defense than Latias with Soul Dew. It also
	// looks like the best ability would be Flower Gift in the Sun, but this is
	// just as good as Sandstorm's Special Defense boost.
	return static_cast<special_defense_type>(bounded::max(defense, 1_bi));
}

namespace {

auto paralysis_speed_divisor(ActivePokemon const pokemon) {
	return BOUNDED_CONDITIONAL(lowers_speed(get_status(pokemon), pokemon.ability()), 4_bi, 1_bi);
}

auto tailwind_speed_multiplier(Team const & team) {
	return BOUNDED_CONDITIONAL(team.screens.tailwind(), 2_bi, 1_bi);
}

}	// namespace

auto calculate_speed(Generation const generation, Team const & team, Ability const other_ability, Weather const weather) -> speed_type {
	constexpr auto stat = StatNames::SPE;
	auto const & pokemon = team.pokemon();
	auto const speed = calculate_initial_stat(stat, pokemon) *
		modifier<stat>(pokemon.stage()) *
		ability_modifier<stat>(generation, pokemon, Moves(), other_ability, weather) *
		item_modifier<stat>(generation, pokemon, weather) /
		paralysis_speed_divisor (pokemon) *
		tailwind_speed_multiplier (team);

	// static_cast here because it looks as though the fastest Pokemon would
	// hold Quick Powder, but because of the restriction on the Pokemon being
	// Ditto, it is better to use a Deoxys-Speed with Choice Scarf.
	return static_cast<speed_type>(bounded::max(speed, 1_bi));
}

auto Faster::before_trick_room(Generation const generation, Team const & team1, Team const & team2, Weather const weather) -> Faster {
	auto const speed1 = calculate_speed(generation, team1, team2.pokemon().ability(), weather);
	auto const speed2 = calculate_speed(generation, team2, team1.pokemon().ability(), weather);

	return
		speed1 > speed2 ? Faster(team1, team2) :
		speed1 < speed2 ? Faster(team2, team1) :
		Faster();
}

Faster::Faster(Generation const generation, Team const & team1, Team const & team2, Weather const weather) {
	auto result = before_trick_room(generation, team1, team2, weather);
	if (!result) {
		return;
	}
	// TODO: Handle Full_Incense, Lagging_Tail, and Stall
	bounded::insert(m_teams, weather.trick_room() ? pair(result->second, result->first) : pair(result->first, result->second));
}

Order::Order(Generation const generation, Team const & team1, Moves const move1, Team const & team2, Moves const move2, Weather const weather):
	Order([&]{
		auto const priority1 = Priority(generation, move1);
		auto const priority2 = Priority(generation, move2);
		
		auto const lhs = Element{team1, move1};
		auto const rhs = Element{team2, move2};
		
		auto const lhs_first = Order(lhs, rhs);
		auto const rhs_first = Order(rhs, lhs);

		if (priority1 > priority2) {
			return lhs_first;
		} else if (priority1 < priority2) {
			return rhs_first;
		} else if (auto const ordered = Faster(generation, team1, team2, weather)) {
			return (std::addressof(ordered->first) == std::addressof(team1)) ? lhs_first : rhs_first;
		} else {
			return Order();
		}
	}())
{
}

}	// namespace technicalmachine
