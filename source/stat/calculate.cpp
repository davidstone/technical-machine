// Calculate a Pokemon's current stat
// Copyright (C) 2014 David Stone
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

#include "calculate.hpp"

#include <utility>

#include "nature.hpp"
#include "stat_names.hpp"

#include "../ability.hpp"
#include "../item.hpp"
#include "../rational.hpp"
#include "../status.hpp"
#include "../team.hpp"
#include "../weather.hpp"

#include "../move/moves.hpp"
#include "../move/priority.hpp"

#include "../pokemon/active_pokemon.hpp"
#include "../pokemon/level.hpp"
#include "../pokemon/pokemon.hpp"
#include "../pokemon/species.hpp"

namespace technicalmachine {
namespace {

bool is_boosted_by_deepseascale(Species const species) {
	return species == Species::Clamperl;
}

bool is_boosted_by_deepseatooth(Species const species) {
	return species == Species::Clamperl;
}

bool is_boosted_by_light_ball(Species const species) {
	return species == Species::Pikachu;
}

bool is_boosted_by_metal_powder(Species const species) {
	return species == Species::Ditto;
}

bool is_boosted_by_quick_powder(Species const species) {
	return species == Species::Ditto;
}

bool is_boosted_by_soul_dew(Species const species) {
	switch (species) {
		case Species::Latias:
		case Species::Latios:
			return true;
		default:
			return false;
	}
}

bool is_boosted_by_thick_club(Species const species) {
	switch (species) {
		case Species::Cubone:
		case Species::Marowak:
			return true;
		default:
			return false;
	}
}

#define CONDITIONAL(b, t, f) BOUNDED_INTEGER_CONDITIONAL(b, t, f)

constexpr auto ability_denominator = 2_bi;

template<StatNames stat>
struct AbilityNumerator;

template<>
struct AbilityNumerator<StatNames::ATK> {
	auto operator()(ActivePokemon const & attacker, Weather const & weather) -> bounded_integer::native_integer<1, 4> {
		switch (get_ability(attacker).name()) {
			case Ability::Flower_Gift:
				return CONDITIONAL(weather.sun(), 3_bi, ability_denominator);
			case Ability::Guts:
				return CONDITIONAL(!get_status(attacker).is_clear(), 3_bi, ability_denominator);
			case Ability::Hustle:
				return 3_bi;
			case Ability::Huge_Power:
			case Ability::Pure_Power:
				return ability_denominator * 2_bi;
			case Ability::Slow_Start:
				return CONDITIONAL(attacker.slow_start_is_active(), 1_bi, ability_denominator);
			default:
				return ability_denominator;
		}
	}
};
template<>
struct AbilityNumerator<StatNames::SPA> {
	auto operator()(ActivePokemon const & pokemon, Weather const & weather) {
		return CONDITIONAL(get_ability(pokemon).boosts_special_attack(weather), 3_bi, ability_denominator);
	}
};
template<>
struct AbilityNumerator<StatNames::DEF> {
	auto operator()(ActivePokemon const & defender, Weather const &) {
		return CONDITIONAL(get_ability(defender).boosts_defense(get_status(defender)), 3_bi, ability_denominator);
	}
};
template<>
struct AbilityNumerator<StatNames::SPD> {
	auto operator()(ActivePokemon const & pokemon, Weather const & weather) {
		return CONDITIONAL(get_ability(pokemon).boosts_special_defense(weather), 3_bi, ability_denominator);
	}
};
template<>
struct AbilityNumerator<StatNames::SPE> {
	auto operator()(ActivePokemon const & pokemon, Weather const & weather) -> bounded_integer::native_integer<1, 4> {
		switch (get_ability(pokemon).name()) {
			case Ability::Chlorophyll:
				return CONDITIONAL(weather.sun(), ability_denominator * 2_bi, ability_denominator);
			case Ability::Swift_Swim:
				return CONDITIONAL(weather.rain(), ability_denominator * 2_bi, ability_denominator);
			case Ability::Unburden:
				return CONDITIONAL(get_item(pokemon).was_lost(), ability_denominator * 2_bi, ability_denominator);
			case Ability::Quick_Feet:
				return CONDITIONAL(!get_status(pokemon).is_clear(), 3_bi, ability_denominator);
			case Ability::Slow_Start:
				return CONDITIONAL(pokemon.slow_start_is_active(), 1_bi, ability_denominator);
			default:
				return ability_denominator;
		}
	}
};

template<StatNames stat>
auto ability_modifier(ActivePokemon const & pokemon, Weather const & weather) {
	return make_bounded_rational(AbilityNumerator<stat>{}(pokemon, weather), ability_denominator);
}


constexpr auto item_denominator = 2_bi;

template<StatNames>
struct ItemNumerator;

template<>
struct ItemNumerator<StatNames::ATK> {
	auto operator()(Pokemon const & attacker) -> bounded_integer::native_integer<2, 4> {
		switch (get_item(attacker).name) {
			case Item::CHOICE_BAND:
				return 3_bi;
			case Item::LIGHT_BALL:
				return CONDITIONAL(is_boosted_by_light_ball(attacker), 2_bi * item_denominator, item_denominator);
			case Item::THICK_CLUB:
				return CONDITIONAL(is_boosted_by_thick_club(attacker), 2_bi * item_denominator, item_denominator);
			default:
				return item_denominator;
		}
	}
};
template<>
struct ItemNumerator<StatNames::SPA> {
	auto operator()(Pokemon const & attacker) -> bounded_integer::native_integer<2, 4> {
		switch (get_item(attacker).name) {
			case Item::SOUL_DEW:
				return CONDITIONAL(is_boosted_by_soul_dew(attacker), 3_bi, item_denominator);
			case Item::CHOICE_SPECS:
				return 3_bi;
			case Item::DEEPSEATOOTH:
				return CONDITIONAL(is_boosted_by_deepseatooth(attacker), 2_bi * item_denominator, item_denominator);
			case Item::LIGHT_BALL:
				return CONDITIONAL(is_boosted_by_light_ball(attacker), 2_bi * item_denominator, item_denominator);
			default:
				return item_denominator;
		}
	}
};
template<>
struct ItemNumerator<StatNames::DEF> {
	auto operator()(Pokemon const & defender) -> bounded_integer::native_integer<2, 3> {
		return CONDITIONAL(
			get_item(defender).name == Item::METAL_POWDER and is_boosted_by_metal_powder(defender),
			3_bi,
			item_denominator
		);
	}
};
template<>
struct ItemNumerator<StatNames::SPD> {
	auto operator()(Pokemon const & defender) -> bounded_integer::native_integer<2, 4> {
		switch (get_item(defender).name) {
			case Item::DEEPSEASCALE:
				return CONDITIONAL(is_boosted_by_deepseascale(defender), 2_bi * item_denominator, item_denominator);
			case Item::METAL_POWDER:
				return CONDITIONAL(is_boosted_by_metal_powder(defender), 3_bi, item_denominator);
			case Item::SOUL_DEW:
				return CONDITIONAL(is_boosted_by_soul_dew(defender), 3_bi, item_denominator);
			default:
				return item_denominator;
		}
	}
};
template<>
struct ItemNumerator<StatNames::SPE> {
	auto operator()(Pokemon const & pokemon) -> bounded_integer::native_integer<1, 4> {
		switch (get_item(pokemon).name) {
			case Item::QUICK_POWDER:
				return CONDITIONAL(is_boosted_by_quick_powder(pokemon), 2_bi * item_denominator, item_denominator);
			case Item::CHOICE_SCARF:
				return 3_bi;
			case Item::MACHO_BRACE:
			case Item::POWER_ANKLET:
			case Item::POWER_BAND:
			case Item::POWER_BELT:
			case Item::POWER_BRACER:
			case Item::POWER_LENS:
			case Item::POWER_WEIGHT:
				return 1_bi;
			default:
				return item_denominator;
		}
	}
};

template<StatNames stat>
auto item_modifier(Pokemon const & pokemon) {
	return make_bounded_rational(ItemNumerator<stat>{}(pokemon), item_denominator);
}

template<StatNames stat>
class StatTraits;

template<>
class StatTraits<StatNames::ATK> {
public:
	static constexpr bool is_physical = true;
	static constexpr StatNames other = StatNames::DEF;
};
template<>
class StatTraits<StatNames::DEF> {
public:
	static constexpr bool is_physical = true;
	static constexpr StatNames other = StatNames::ATK;
};
template<>
class StatTraits<StatNames::SPA> {
public:
	static constexpr bool is_physical = false;
};
template<>
class StatTraits<StatNames::SPD> {
public:
	static constexpr bool is_physical = false;
};
template<>
class StatTraits<StatNames::SPE> {
public:
	static constexpr bool is_physical = false;
};

template<StatNames stat, enable_if_t<StatTraits<stat>::is_physical> = clang_dummy>
auto calculate_initial_stat(ActivePokemon const & pokemon) {
	constexpr auto other = StatTraits<stat>::other;
	auto const level = get_level(pokemon);
	auto const nature = get_nature(pokemon);
	return !pokemon.power_trick_is_active() ?
		initial_stat<stat>(get_stat(pokemon, stat), level, nature) :
		initial_stat<other>(get_stat(pokemon, other), level, nature);
}
template<StatNames stat, enable_if_t<!StatTraits<stat>::is_physical> = clang_dummy>
auto calculate_initial_stat(ActivePokemon const & pokemon) {
	return initial_stat<stat>(get_stat(pokemon, stat), get_level(pokemon), get_nature(pokemon));
}

template<StatNames stat>
auto calculate_common_offensive_stat(ActivePokemon const & pokemon, Weather const & weather) {
	auto const attack = calculate_initial_stat<stat>(pokemon) *
		modifier<stat>(pokemon.stage(), pokemon.critical_hit()) *
		ability_modifier<stat>(pokemon, weather) *
		item_modifier<stat>(pokemon);
	
	return bounded_integer::max(attack, 1_bi);
}

}	// namespace

std::common_type<attack_type, special_attack_type>::type calculate_attacking_stat(ActivePokemon const & attacker, Weather const & weather) {
	return is_physical(attacker.move()) ?
		calculate_attack(attacker, weather) :
		calculate_special_attack(attacker, weather);
}

attack_type calculate_attack(ActivePokemon const & attacker, Weather const & weather) {
	// static_cast here because it looks as though the strongest attacker would
	// hold a Light Ball, but because of the restriction on the attacker being
	// Pikachu, it is better to use a Power Trick Shuckle with a Choice Band.
	return static_cast<attack_type>(calculate_common_offensive_stat<StatNames::ATK>(attacker, weather));
}

special_attack_type calculate_special_attack(ActivePokemon const & attacker, Weather const & weather) {
	// see above comment about Light Ball, except the strongest Special Attack
	// Pokemon is actually a Choice Specs Deoxys-Attack.
	return static_cast<special_attack_type>(calculate_common_offensive_stat<StatNames::SPA>(attacker, weather));
}

namespace {

bool is_self_KO(Moves const move) {
	switch (move) {
		case Moves::Explosion:
		case Moves::Selfdestruct:
			return true;
		default:
			return false;
	}
}

}	// namespace

std::common_type<defense_type, special_defense_type>::type calculate_defending_stat(ActivePokemon const & attacker, ActivePokemon const & defender, Weather const & weather) {
	return is_physical(attacker.move()) ?
		calculate_defense(defender, weather, attacker.critical_hit(), is_self_KO(attacker.move())) :
		calculate_special_defense(defender, weather, attacker.critical_hit());
}

defense_type calculate_defense(ActivePokemon const & defender, Weather const & weather, bool ch, bool is_self_KO) {
	constexpr auto stat = StatNames::DEF;
	auto const defense = calculate_initial_stat<stat>(defender) *
		modifier<stat>(defender.stage(), ch) *
		ability_modifier<stat>(defender, weather) *
		item_modifier<stat>(defender);
	
	// static_cast here because it looks as though the strongest defender would
	// hold Metal Powder, but because of the restriction on the attacker being
	// Ditto, it is better to use a Shuckle with no boosting item available.
	return static_cast<defense_type>(bounded_integer::max(CONDITIONAL(is_self_KO, defense / 2_bi, defense), 1_bi));
}

namespace {

auto special_defense_sandstorm_boost(ActivePokemon const & defender, Weather const & weather) {
	return make_bounded_rational(CONDITIONAL(is_type(defender, Type::Rock) and weather.sand(), 3_bi, 2_bi), 2_bi);
}

}	// namespace

special_defense_type calculate_special_defense(ActivePokemon const & defender, Weather const & weather, bool ch) {
	constexpr auto stat = StatNames::SPD;
	auto const defense = calculate_initial_stat<stat>(defender) *	
		modifier<stat>(defender.stage(), ch) *
		ability_modifier<stat>(defender, weather) *
		item_modifier<stat>(defender) *
		special_defense_sandstorm_boost(defender, weather);
	
	// static_cast here because it looks as though the strongest defender would
	// hold DeepSeaScale, but because of the restriction on the defender being
	// Clamperl, it is better to use a Shuckle with no boosting item available.
	// This also gives more Special Defense than Latias with Soul Dew. It also
	// looks like the best ability would be Flower Gift in the Sun, but this is
	// just as good as Sandstorm's Special Defense boost.
	return static_cast<special_defense_type>(bounded_integer::max(defense, 1_bi));
}

namespace {

auto paralysis_speed_divisor(Pokemon const & pokemon) {
	return CONDITIONAL(get_status(pokemon).lowers_speed(get_ability(pokemon)), 4_bi, 1_bi);
}

auto tailwind_speed_multiplier(Team const & team) {
	return CONDITIONAL(team.screens.tailwind(), 2_bi, 1_bi);
}

}	// namespace

speed_type calculate_speed(Team const & team, Weather const & weather) {
	constexpr auto stat = StatNames::SPE;
	auto const & pokemon = team.pokemon();
	auto const speed = calculate_initial_stat<stat>(pokemon) *
		modifier<stat>(pokemon.stage()) *
		ability_modifier<stat>(pokemon, weather) *
		item_modifier<stat>(pokemon) /
		paralysis_speed_divisor (pokemon) *
		tailwind_speed_multiplier (team);

	// static_cast here because it looks as though the fastest Pokemon would
	// hold Quick Powder, but because of the restriction on the Pokemon being
	// Ditto, it is better to use a Deoxys-Speed with Choice Scarf.
	return static_cast<speed_type>(bounded_integer::max(speed, 1_bi));
}

void order(Team & team1, Team & team2, Weather const & weather, Team* & faster, Team* & slower) {
	Priority const priority1(team1.pokemon().move());
	Priority const priority2(team2.pokemon().move());
	if (priority1 == priority2) {
		faster_pokemon(team1, team2, weather, faster, slower);
	}
	else if (priority1 > priority2) {
		faster = &team1;
		slower = &team2;
	}
	else {	// if (priority1 < priority2)
		faster = &team2;
		slower = &team1;
	}
}

void faster_pokemon(Team & team1, Team & team2, Weather const & weather, Team* & faster, Team* & slower) {
	auto const speed1 = calculate_speed(team1, weather);
	auto const speed2 = calculate_speed(team2, weather);
	if (speed1 > speed2) {
		faster = &team1;
		slower = &team2;
	}
	else if (speed1 < speed2) {
		faster = &team2;
		slower = &team1;
	}
	else {				// All things are equal
		faster = nullptr;
		slower = nullptr;
	}
	if (weather.trick_room()) {
		std::swap(faster, slower);
	}
}

}	// namespace technicalmachine
