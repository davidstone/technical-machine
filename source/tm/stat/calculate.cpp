// Calculate a Pokemon's current stat
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

#include <tm/stat/calculate.hpp>

#include <utility>

#include <tm/stat/nature.hpp>
#include <tm/stat/stat_names.hpp>

#include <tm/ability.hpp>
#include <tm/item.hpp>
#include <tm/rational.hpp>
#include <tm/status.hpp>
#include <tm/team.hpp>
#include <tm/weather.hpp>

#include <tm/move/moves.hpp>
#include <tm/move/priority.hpp>

#include <tm/pokemon/active_pokemon.hpp>
#include <tm/pokemon/level.hpp>
#include <tm/pokemon/pokemon.hpp>
#include <tm/pokemon/species.hpp>

namespace technicalmachine {
namespace {

constexpr auto is_boosted_by_deepseascale(Species const species) {
	return species == Species::Clamperl;
}

constexpr auto is_boosted_by_deepseatooth(Species const species) {
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

constexpr auto ability_denominator = 2_bi;

template<StatNames stat>
struct AbilityNumerator;

template<>
struct AbilityNumerator<StatNames::ATK> {
	auto operator()(ActivePokemon const attacker, Weather const weather) -> bounded::integer<1, 4> {
		switch (get_ability(attacker)) {
			case Ability::Flower_Gift:
				return BOUNDED_CONDITIONAL(weather.sun(), 3_bi, ability_denominator);
			case Ability::Guts:
				return BOUNDED_CONDITIONAL(!is_clear(get_status(attacker)), 3_bi, ability_denominator);
			case Ability::Hustle:
				return 3_bi;
			case Ability::Huge_Power:
			case Ability::Pure_Power:
				return ability_denominator * 2_bi;
			case Ability::Slow_Start:
				return BOUNDED_CONDITIONAL(slow_start_is_active(attacker), 1_bi, ability_denominator);
			default:
				return ability_denominator;
		}
	}
};
template<>
struct AbilityNumerator<StatNames::SPA> {
	auto operator()(ActivePokemon const pokemon, Weather const weather) {
		return BOUNDED_CONDITIONAL(boosts_special_attack(get_ability(pokemon), weather), 3_bi, ability_denominator);
	}
};
template<>
struct AbilityNumerator<StatNames::DEF> {
	auto operator()(ActivePokemon const defender, Weather const) {
		return BOUNDED_CONDITIONAL(boosts_defense(get_ability(defender), get_status(defender)), 3_bi, ability_denominator);
	}
};
template<>
struct AbilityNumerator<StatNames::SPD> {
	auto operator()(ActivePokemon const pokemon, Weather const weather) {
		return BOUNDED_CONDITIONAL(boosts_special_defense(get_ability(pokemon), weather), 3_bi, ability_denominator);
	}
};
template<>
struct AbilityNumerator<StatNames::SPE> {
	auto operator()(ActivePokemon const pokemon, Weather const weather) -> bounded::integer<1, 4> {
		switch (get_ability(pokemon)) {
			case Ability::Chlorophyll:
				return BOUNDED_CONDITIONAL(weather.sun(), ability_denominator * 2_bi, ability_denominator);
			case Ability::Swift_Swim:
				return BOUNDED_CONDITIONAL(weather.rain(), ability_denominator * 2_bi, ability_denominator);
			case Ability::Unburden:
				return BOUNDED_CONDITIONAL(was_lost(get_item(pokemon)), ability_denominator * 2_bi, ability_denominator);
			case Ability::Quick_Feet:
				return BOUNDED_CONDITIONAL(!is_clear(get_status(pokemon)), 3_bi, ability_denominator);
			case Ability::Slow_Start:
				return BOUNDED_CONDITIONAL(slow_start_is_active(pokemon), 1_bi, ability_denominator);
			default:
				return ability_denominator;
		}
	}
};

template<StatNames stat>
auto ability_modifier(ActivePokemon const pokemon, Weather const weather) {
	return rational(AbilityNumerator<stat>{}(pokemon, weather), ability_denominator);
}


constexpr auto item_denominator = 2_bi;

template<StatNames>
struct ItemNumerator;

template<>
struct ItemNumerator<StatNames::ATK> {
	auto operator()(Pokemon const & attacker) -> bounded::integer<2, 4> {
		switch (get_item(attacker)) {
			case Item::Choice_Band:
				return 3_bi;
			case Item::Light_Ball:
				return BOUNDED_CONDITIONAL(is_boosted_by_light_ball(get_species(attacker)), 2_bi * item_denominator, item_denominator);
			case Item::Thick_Club:
				return BOUNDED_CONDITIONAL(is_boosted_by_thick_club(get_species(attacker)), 2_bi * item_denominator, item_denominator);
			default:
				return item_denominator;
		}
	}
};
template<>
struct ItemNumerator<StatNames::SPA> {
	auto operator()(Pokemon const & attacker) -> bounded::integer<2, 4> {
		switch (get_item(attacker)) {
			case Item::Soul_Dew:
				return BOUNDED_CONDITIONAL(is_boosted_by_soul_dew(get_species(attacker)), 3_bi, item_denominator);
			case Item::Choice_Specs:
				return 3_bi;
			case Item::DeepSeaTooth:
				return BOUNDED_CONDITIONAL(is_boosted_by_deepseatooth(get_species(attacker)), 2_bi * item_denominator, item_denominator);
			case Item::Light_Ball:
				return BOUNDED_CONDITIONAL(is_boosted_by_light_ball(get_species(attacker)), 2_bi * item_denominator, item_denominator);
			default:
				return item_denominator;
		}
	}
};
template<>
struct ItemNumerator<StatNames::DEF> {
	auto operator()(Pokemon const & defender) -> bounded::integer<2, 3> {
		return BOUNDED_CONDITIONAL(
			get_item(defender) == Item::Metal_Powder and is_boosted_by_metal_powder(get_species(defender)),
			3_bi,
			item_denominator
		);
	}
};
template<>
struct ItemNumerator<StatNames::SPD> {
	auto operator()(Pokemon const & defender) -> bounded::integer<2, 4> {
		switch (get_item(defender)) {
			case Item::DeepSeaScale:
				return BOUNDED_CONDITIONAL(is_boosted_by_deepseascale(get_species(defender)), 2_bi * item_denominator, item_denominator);
			case Item::Metal_Powder:
				return BOUNDED_CONDITIONAL(is_boosted_by_metal_powder(get_species(defender)), 3_bi, item_denominator);
			case Item::Soul_Dew:
				return BOUNDED_CONDITIONAL(is_boosted_by_soul_dew(get_species(defender)), 3_bi, item_denominator);
			default:
				return item_denominator;
		}
	}
};
template<>
struct ItemNumerator<StatNames::SPE> {
	auto operator()(Pokemon const & pokemon) -> bounded::integer<1, 4> {
		switch (get_item(pokemon)) {
			case Item::Quick_Powder:
				return BOUNDED_CONDITIONAL(is_boosted_by_quick_powder(get_species(pokemon)), 2_bi * item_denominator, item_denominator);
			case Item::Choice_Scarf:
				return 3_bi;
			case Item::Macho_Brace:
			case Item::Power_Anklet:
			case Item::Power_Band:
			case Item::Power_Belt:
			case Item::Power_Bracer:
			case Item::Power_Lens:
			case Item::Power_Weight:
				return 1_bi;
			default:
				return item_denominator;
		}
	}
};

template<StatNames stat>
auto item_modifier(Pokemon const & pokemon) {
	return rational(ItemNumerator<stat>{}(pokemon), item_denominator);
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
	case StatNames::ATK:
		return StatNames::DEF;
	case StatNames::DEF:
		return StatNames::ATK;
	default:
		assert(false); __builtin_unreachable();
	}
}

auto calculate_initial_stat(StatNames const stat, ActivePokemon const pokemon) {
	auto const level = get_level(pokemon);
	auto const nature = get_nature(pokemon);
	return !is_physical(stat) or !power_trick_is_active(pokemon) ?
		initial_stat(stat, get_stat(pokemon, stat), level, nature) :
		initial_stat(other_physical_stat(stat), get_stat(pokemon, other_physical_stat(stat)), level, nature);
}

template<StatNames stat>
auto calculate_common_offensive_stat(ActivePokemon const pokemon, Weather const weather, bool const critical_hit) {
	auto const attack = calculate_initial_stat(stat, pokemon) *
		modifier<stat>(stage(pokemon), critical_hit) *
		ability_modifier<stat>(pokemon, weather) *
		item_modifier<stat>(pokemon);
	
	return bounded::max(attack, 1_bi);
}

}	// namespace

auto calculate_attacking_stat(ActivePokemon const attacker, Moves const move, Weather const weather, bool const critical_hit) -> std::common_type_t<attack_type, special_attack_type> {
	return is_physical(move) ?
		calculate_attack(attacker, weather, critical_hit) :
		calculate_special_attack(attacker, weather, critical_hit);
}

auto calculate_attack(ActivePokemon const attacker, Weather const weather, bool const critical_hit) -> attack_type {
	// static_cast here because it looks as though the strongest attacker would
	// hold a Light Ball, but because of the restriction on the attacker being
	// Pikachu, it is better to use a Power Trick Shuckle with a Choice Band.
	return static_cast<attack_type>(calculate_common_offensive_stat<StatNames::ATK>(attacker, weather, critical_hit));
}

auto calculate_special_attack(ActivePokemon const attacker, Weather const weather, bool const critical_hit) -> special_attack_type {
	// see above comment about Light Ball, except the strongest Special Attack
	// Pokemon is actually a Choice Specs Deoxys-Attack.
	return static_cast<special_attack_type>(calculate_common_offensive_stat<StatNames::SPA>(attacker, weather, critical_hit));
}

namespace {

auto is_self_KO(Moves const move) {
	switch (move) {
		case Moves::Explosion:
		case Moves::Selfdestruct:
			return true;
		default:
			return false;
	}
}

}	// namespace

auto calculate_defending_stat(Moves const move, ActivePokemon const defender, Weather const weather, bool const critical_hit) -> std::common_type_t<defense_type, special_defense_type> {
	return is_physical(move) ?
		calculate_defense(defender, weather, critical_hit, is_self_KO(move)) :
		calculate_special_defense(defender, weather, critical_hit);
}

auto calculate_defense(ActivePokemon const defender, Weather const weather, bool const critical_hit, bool is_self_KO) -> defense_type {
	constexpr auto stat = StatNames::DEF;
	auto const defense = calculate_initial_stat(stat, defender) *
		modifier<stat>(stage(defender), critical_hit) *
		ability_modifier<stat>(defender, weather) *
		item_modifier<stat>(defender);
	
	// static_cast here because it looks as though the strongest defender would
	// hold Metal Powder, but because of the restriction on the attacker being
	// Ditto, it is better to use a Shuckle with no boosting item available.
	return static_cast<defense_type>(bounded::max(BOUNDED_CONDITIONAL(is_self_KO, defense / 2_bi, defense), 1_bi));
}

namespace {

auto special_defense_sandstorm_boost(ActivePokemon const defender, Weather const weather) {
	return rational(BOUNDED_CONDITIONAL(is_type(defender, Type::Rock, is_roosting(defender)) and weather.sand(), 3_bi, 2_bi), 2_bi);
}

}	// namespace

auto calculate_special_defense(ActivePokemon const defender, Weather const weather, bool const critical_hit) -> special_defense_type {
	constexpr auto stat = StatNames::SPD;
	auto const defense = calculate_initial_stat(stat, defender) *	
		modifier<stat>(stage(defender), critical_hit) *
		ability_modifier<stat>(defender, weather) *
		item_modifier<stat>(defender) *
		special_defense_sandstorm_boost(defender, weather);
	
	// static_cast here because it looks as though the strongest defender would
	// hold DeepSeaScale, but because of the restriction on the defender being
	// Clamperl, it is better to use a Shuckle with no boosting item available.
	// This also gives more Special Defense than Latias with Soul Dew. It also
	// looks like the best ability would be Flower Gift in the Sun, but this is
	// just as good as Sandstorm's Special Defense boost.
	return static_cast<special_defense_type>(bounded::max(defense, 1_bi));
}

namespace {

auto paralysis_speed_divisor(Pokemon const & pokemon) {
	return BOUNDED_CONDITIONAL(lowers_speed(get_status(pokemon), get_ability(pokemon)), 4_bi, 1_bi);
}

auto tailwind_speed_multiplier(Team const & team) {
	return BOUNDED_CONDITIONAL(team.screens.tailwind(), 2_bi, 1_bi);
}

}	// namespace

auto calculate_speed(Team const & team, Weather const weather) -> speed_type {
	constexpr auto stat = StatNames::SPE;
	auto const & pokemon = team.pokemon();
	auto const speed = calculate_initial_stat(stat, pokemon) *
		modifier<stat>(stage(pokemon)) *
		ability_modifier<stat>(pokemon, weather) *
		item_modifier<stat>(pokemon) /
		paralysis_speed_divisor (pokemon) *
		tailwind_speed_multiplier (team);

	// static_cast here because it looks as though the fastest Pokemon would
	// hold Quick Powder, but because of the restriction on the Pokemon being
	// Ditto, it is better to use a Deoxys-Speed with Choice Scarf.
	return static_cast<speed_type>(bounded::max(speed, 1_bi));
}

auto order(Team const & team1, Moves const move1, Team const & team2, Moves const move2, Weather const weather) -> Order {
	auto const priority1 = Priority(move1);
	auto const priority2 = Priority(move2);
	
	auto const lhs = OrderElement{team1, move1};
	auto const rhs = OrderElement{team2, move2};
	
	auto const lhs_first = Order(bounded::in_place, lhs, rhs);
	auto const rhs_first = Order(bounded::in_place, rhs, lhs);

	if (priority1 > priority2) {
		return lhs_first;
	} else if (priority1 < priority2) {
		return rhs_first;
	} else if (auto const ordered = faster_pokemon(team1, team2, weather)) {
		return (std::addressof(ordered->first) == std::addressof(team1)) ? lhs_first : rhs_first;
	} else {
		return bounded::none;
	}
}

namespace {

auto faster_pokemon_before_trick_room(Team const & team1, Team const & team2, Weather const weather) -> Faster {
	auto const speed1 = calculate_speed(team1, weather);
	auto const speed2 = calculate_speed(team2, weather);

	if (speed1 > speed2) {
		return Faster(bounded::in_place, team1, team2);
	} else if (speed1 < speed2) {
		return Faster(bounded::in_place, team2, team1);
	} else {
		return bounded::none;
	}
}

}	// namespace

auto faster_pokemon(Team const & team1, Team const & team2, Weather const weather) -> Faster {
	auto result = faster_pokemon_before_trick_room(team1, team2, weather);
	if (!result) {
		return bounded::none;
	}
	if (weather.trick_room()) {
		return Faster(bounded::in_place, result->second, result->first);
	}
	return result;
}

}	// namespace technicalmachine
