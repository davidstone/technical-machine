// Stat formulas
// Copyright (C) 2013 David Stone
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

#include "stat.hpp"

#include <cstdint>
#include <type_traits>

#include <bounded_integer/array.hpp>

#include "nature.hpp"

#include "../ability.hpp"
#include "../item.hpp"
#include "../rational.hpp"
#include "../status.hpp"
#include "../team.hpp"
#include "../weather.hpp"

#include "../move/moves.hpp"
#include "../move/priority.hpp"

#include "../pokemon/level.hpp"
#include "../pokemon/pokemon.hpp"
#include "../pokemon/species.hpp"

namespace technicalmachine {
namespace {
using namespace bounded_integer::literal;

Stat::base_type get_base(Species name, StatNames stat_name);

}	// namespace

Stat::Stat(Species const species, StatNames const stat_name, EV const set_ev) :
	base(get_base(species, stat_name)),
	ev(set_ev),
	iv(31_bi)
	{
}

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

template<StatNames stat>
Rational item_modifier(Pokemon const & pokemon);
template<>
Rational item_modifier<StatNames::ATK>(Pokemon const & attacker) {
	switch (get_item(attacker).name) {
		case Item::CHOICE_BAND:
			return Rational(3, 2);
		case Item::LIGHT_BALL:
			return is_boosted_by_light_ball(attacker) ? Rational(2) : Rational(1);
		case Item::THICK_CLUB:
			return is_boosted_by_thick_club(attacker) ? Rational(2, 1) : Rational(1);
		default:
			return Rational(1);
	}
}
template<>
Rational item_modifier<StatNames::SPA>(Pokemon const & attacker) {
	switch (get_item(attacker).name) {
		case Item::SOUL_DEW:
			return is_boosted_by_soul_dew(attacker) ? Rational(3, 2) : Rational(1);
		case Item::CHOICE_SPECS:
			return Rational(3, 2);
		case Item::DEEPSEATOOTH:
			return is_boosted_by_deepseatooth(attacker) ? Rational(2) : Rational(1);
		case Item::LIGHT_BALL:
			return is_boosted_by_light_ball(attacker) ? Rational(2) : Rational(1);
		default:
			return Rational(1);
	}
}
template<>
Rational item_modifier<StatNames::DEF>(Pokemon const & defender) {
	return (get_item(defender).name == Item::METAL_POWDER and is_boosted_by_metal_powder(defender)) ?
		Rational(3, 2) :
		Rational(1);
}
template<>
Rational item_modifier<StatNames::SPD>(Pokemon const & defender) {
	switch (get_item(defender).name) {
		case Item::DEEPSEASCALE:
			return is_boosted_by_deepseascale(defender) ? Rational(2) : Rational(1);
		case Item::METAL_POWDER:
			return is_boosted_by_metal_powder(defender) ? Rational(3, 2) : Rational(1);
		case Item::SOUL_DEW:
			return is_boosted_by_soul_dew(defender) ? Rational(3, 2) : Rational(1);
		default:
			return Rational(1);
	}
}
template<>
Rational item_modifier<StatNames::SPE>(Pokemon const & pokemon) {
	switch (get_item(pokemon).name) {
		case Item::QUICK_POWDER:
			return is_boosted_by_quick_powder(pokemon) ? Rational(2) : Rational(1);
		case Item::CHOICE_SCARF:
			return Rational(3, 2);
		case Item::MACHO_BRACE:
		case Item::POWER_ANKLET:
		case Item::POWER_BAND:
		case Item::POWER_BELT:
		case Item::POWER_BRACER:
		case Item::POWER_LENS:
		case Item::POWER_WEIGHT:
			return Rational(1, 2);
		default:
			return Rational(1);
	}
}

Rational special_defense_sandstorm_boost(ActivePokemon const & defender, Weather const & weather);

unsigned paralysis_speed_divisor (Pokemon const & pokemon);
unsigned tailwind_speed_multiplier (Team const & team);

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

template<StatNames stat>
typename std::enable_if<StatTraits<stat>::is_physical, unsigned>::type calculate_initial_stat(ActivePokemon const & pokemon) {
	constexpr auto other = StatTraits<stat>::other;
	return !pokemon.power_trick_is_active() ?
		initial_stat<stat>(get_stat(pokemon, stat), get_level(pokemon), get_nature(pokemon)) :
		initial_stat<other>(get_stat(pokemon, other), get_level(pokemon), get_nature(pokemon));
}
template<StatNames stat>
typename std::enable_if<!StatTraits<stat>::is_physical, unsigned>::type calculate_initial_stat(ActivePokemon const & pokemon) {
	return initial_stat<stat>(get_stat(pokemon, stat), get_level(pokemon), get_nature(pokemon));
}

template<StatNames stat>
unsigned calculate_common_offensive_stat(ActivePokemon const & pokemon, Weather const & weather) {
	auto attack = calculate_initial_stat<stat>(pokemon);
	attack *= pokemon.stage_modifier<stat>(pokemon.critical_hit());

	attack *= Ability::stat_modifier<stat>(pokemon, weather);
	attack *= item_modifier<stat>(pokemon);
	
	return std::max(attack, 1u);
}

}	// namespace

unsigned calculate_attacking_stat(ActivePokemon const & attacker, Weather const & weather) {
	return is_physical(attacker.move()) ?
		calculate_attack(attacker, weather) :
		calculate_special_attack(attacker, weather);
}

unsigned calculate_attack(ActivePokemon const & attacker, Weather const & weather) {
	return calculate_common_offensive_stat<StatNames::ATK>(attacker, weather);
}

unsigned calculate_special_attack(ActivePokemon const & attacker, Weather const & weather) {
	return calculate_common_offensive_stat<StatNames::SPA>(attacker, weather);
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

unsigned calculate_defending_stat (ActivePokemon const & attacker, ActivePokemon const & defender, Weather const & weather) {
	return is_physical(attacker.move()) ?
		calculate_defense(defender, weather, attacker.critical_hit(), is_self_KO(attacker.move())) :
		calculate_special_defense(defender, weather, attacker.critical_hit());
}

unsigned calculate_defense(ActivePokemon const & defender, Weather const & weather, bool ch, bool is_self_KO) {
	constexpr auto stat = StatNames::DEF;
	auto defense = calculate_initial_stat<stat>(defender);

	defense *= defender.stage_modifier<stat>(ch);
	
	defense *= Ability::stat_modifier<stat>(defender, weather);
	defense *= item_modifier<stat>(defender);
	
	if (is_self_KO) {
		defense /= 2;
	}

	return std::max(defense, 1u);
}

unsigned calculate_special_defense(ActivePokemon const & defender, Weather const & weather, bool ch) {
	constexpr auto stat = StatNames::SPD;
	auto defense = calculate_initial_stat<stat>(defender);
	
	defense *= defender.stage_modifier<stat>(ch);

	defense *= Ability::stat_modifier<stat>(defender, weather);	
	defense *= item_modifier<stat>(defender);
	
	defense *= special_defense_sandstorm_boost(defender, weather);
	
	return std::max(defense, 1u);
}

unsigned calculate_speed(Team const & team, Weather const & weather) {
	constexpr auto stat = StatNames::SPE;
	auto const & pokemon = team.pokemon();
	auto speed = calculate_initial_stat<stat>(pokemon);
	
	speed *= pokemon.stage_modifier<stat>();

	speed *= Ability::stat_modifier<stat>(pokemon, weather);
	speed *= item_modifier<stat>(pokemon);
	
	speed /= paralysis_speed_divisor (pokemon);
	
	speed *= tailwind_speed_multiplier (team);

	return std::max(speed, 1u);
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

namespace {

Rational special_defense_sandstorm_boost(ActivePokemon const & defender, Weather const & weather) {
	return (is_type(defender, Type::Rock) and weather.sand()) ? Rational(3, 2) : Rational(1);
}

unsigned paralysis_speed_divisor (Pokemon const & pokemon) {
	return get_status(pokemon).lowers_speed(get_ability(pokemon)) ? 4 : 1;
}

unsigned tailwind_speed_multiplier (Team const & team) {
	return team.screens.tailwind() ? 2 : 1;
}

Stat::base_type get_base(Species const species, StatNames const stat) {
	static constexpr auto base_stat = bounded_integer::make_array<5>(
		// Generation 1							
		49_bi,	49_bi,	65_bi,	65_bi,	45_bi,		// Bulbasaur	
		62_bi,	63_bi,	80_bi,	80_bi,	60_bi,		// Ivysaur	
		82_bi,	83_bi,	100_bi,	100_bi,	80_bi,		// Venusaur	
		52_bi,	43_bi,	60_bi,	50_bi,	65_bi,		// Charmander	
		64_bi,	58_bi,	80_bi,	65_bi,	80_bi,		// Charmeleon	
		84_bi,	78_bi,	109_bi,	85_bi,	100_bi,		// Charizard	
		48_bi,	65_bi,	50_bi,	64_bi,	43_bi,		// Squirtle	
		63_bi,	80_bi,	65_bi,	80_bi,	58_bi,		// Wartortle	
		83_bi,	100_bi,	85_bi,	105_bi,	78_bi,		// Blastoise	
		30_bi,	35_bi,	20_bi,	20_bi,	45_bi,		// Caterpie	
		20_bi,	55_bi,	25_bi,	25_bi,	30_bi,		// Metapod	
		45_bi,	50_bi,	80_bi,	80_bi,	70_bi,		// Butterfree	
		35_bi,	30_bi,	20_bi,	20_bi,	50_bi,		// Weedle	
		25_bi,	50_bi,	25_bi,	25_bi,	35_bi,		// Kakuna	
		80_bi,	40_bi,	45_bi,	80_bi,	75_bi,		// Beedrill	
		45_bi,	40_bi,	35_bi,	35_bi,	56_bi,		// Pidgey	
		60_bi,	55_bi,	50_bi,	50_bi,	71_bi,		// Pidgeotto	
		80_bi,	75_bi,	70_bi,	70_bi,	91_bi,		// Pidgeot	
		56_bi,	35_bi,	25_bi,	35_bi,	72_bi,		// Rattata	
		81_bi,	60_bi,	50_bi,	70_bi,	97_bi,		// Raticate	
		60_bi,	30_bi,	31_bi,	31_bi,	70_bi,		// Spearow	
		90_bi,	65_bi,	61_bi,	61_bi,	100_bi,		// Fearow	
		60_bi,	44_bi,	40_bi,	54_bi,	55_bi,		// Ekans	
		85_bi,	69_bi,	65_bi,	79_bi,	80_bi,		// Arbok	
		55_bi,	30_bi,	50_bi,	40_bi,	90_bi,		// Pikachu	
		90_bi,	55_bi,	90_bi,	80_bi,	100_bi,		// Raichu	
		75_bi,	85_bi,	20_bi,	30_bi,	40_bi,		// Sandshrew	
		100_bi,	110_bi,	45_bi,	55_bi,	65_bi,		// Sandslash	
		47_bi,	52_bi,	40_bi,	40_bi,	41_bi,		// Nidoran-F	
		62_bi,	67_bi,	55_bi,	55_bi,	56_bi,		// Nidorina	
		82_bi,	87_bi,	75_bi,	85_bi,	76_bi,		// Nidoqueen	
		57_bi,	40_bi,	40_bi,	40_bi,	50_bi,		// Nidoran-M	
		72_bi,	57_bi,	55_bi,	55_bi,	65_bi,		// Nidorino	
		92_bi,	77_bi,	85_bi,	75_bi,	85_bi,		// Nidoking	
		45_bi,	48_bi,	60_bi,	65_bi,	35_bi,		// Clefairy	
		70_bi,	73_bi,	85_bi,	90_bi,	60_bi,		// Clefable	
		41_bi,	40_bi,	50_bi,	65_bi,	65_bi,		// Vulpix	
		76_bi,	75_bi,	81_bi,	100_bi,	100_bi,		// Ninetales	
		45_bi,	20_bi,	45_bi,	25_bi,	20_bi,		// Jigglypuff	
		70_bi,	45_bi,	75_bi,	50_bi,	45_bi,		// Wigglytuff	
		45_bi,	35_bi,	30_bi,	40_bi,	55_bi,		// Zubat	
		80_bi,	70_bi,	65_bi,	75_bi,	90_bi,		// Golbat	
		50_bi,	55_bi,	75_bi,	65_bi,	30_bi,		// Oddish	
		65_bi,	70_bi,	85_bi,	75_bi,	40_bi,		// Gloom	
		80_bi,	85_bi,	100_bi,	90_bi,	50_bi,		// Vileplume	
		70_bi,	55_bi,	45_bi,	55_bi,	25_bi,		// Paras	
		95_bi,	80_bi,	60_bi,	80_bi,	30_bi,		// Parasect	
		55_bi,	50_bi,	40_bi,	55_bi,	45_bi,		// Venonat	
		65_bi,	60_bi,	90_bi,	75_bi,	90_bi,		// Venomoth	
		55_bi,	25_bi,	35_bi,	45_bi,	95_bi,		// Diglett	
		80_bi,	50_bi,	50_bi,	70_bi,	120_bi,		// Dugtrio	
		45_bi,	35_bi,	40_bi,	40_bi,	90_bi,		// Meowth	
		70_bi,	60_bi,	65_bi,	65_bi,	115_bi,		// Persian	
		52_bi,	48_bi,	65_bi,	50_bi,	55_bi,		// Psyduck	
		82_bi,	78_bi,	95_bi,	80_bi,	85_bi,		// Golduck	
		80_bi,	35_bi,	35_bi,	45_bi,	70_bi,		// Mankey	
		105_bi,	60_bi,	60_bi,	70_bi,	95_bi,		// Primeape	
		70_bi,	45_bi,	70_bi,	50_bi,	60_bi,		// Growlithe	
		110_bi,	80_bi,	100_bi,	80_bi,	95_bi,		// Arcanine	
		50_bi,	40_bi,	40_bi,	40_bi,	90_bi,		// Poliwag	
		65_bi,	65_bi,	50_bi,	50_bi,	90_bi,		// Poliwhirl	
		85_bi,	95_bi,	70_bi,	90_bi,	70_bi,		// Poliwrath	
		20_bi,	15_bi,	105_bi,	55_bi,	90_bi,		// Abra	
		35_bi,	30_bi,	120_bi,	70_bi,	105_bi,		// Kadabra	
		50_bi,	45_bi,	135_bi,	85_bi,	120_bi,		// Alakazam	
		80_bi,	50_bi,	35_bi,	35_bi,	35_bi,		// Machop	
		100_bi,	70_bi,	50_bi,	60_bi,	45_bi,		// Machoke	
		130_bi,	80_bi,	65_bi,	85_bi,	55_bi,		// Machamp	
		75_bi,	35_bi,	70_bi,	30_bi,	40_bi,		// Bellsprout	
		90_bi,	50_bi,	85_bi,	45_bi,	55_bi,		// Weepinbell	
		105_bi,	65_bi,	100_bi,	60_bi,	70_bi,		// Victreebel	
		40_bi,	35_bi,	50_bi,	100_bi,	70_bi,		// Tentacool	
		70_bi,	65_bi,	80_bi,	120_bi,	100_bi,		// Tentacruel	
		80_bi,	100_bi,	30_bi,	30_bi,	20_bi,		// Geodude	
		95_bi,	115_bi,	45_bi,	45_bi,	35_bi,		// Graveler	
		110_bi,	130_bi,	55_bi,	65_bi,	45_bi,		// Golem	
		85_bi,	55_bi,	65_bi,	65_bi,	90_bi,		// Ponyta	
		100_bi,	70_bi,	80_bi,	80_bi,	105_bi,		// Rapidash	
		65_bi,	65_bi,	40_bi,	40_bi,	15_bi,		// Slowpoke	
		75_bi,	110_bi,	100_bi,	80_bi,	30_bi,		// Slowbro	
		35_bi,	70_bi,	95_bi,	55_bi,	45_bi,		// Magnemite	
		60_bi,	95_bi,	120_bi,	70_bi,	70_bi,		// Magneton	
		65_bi,	55_bi,	58_bi,	62_bi,	60_bi,		// Farfetch'd	
		85_bi,	45_bi,	35_bi,	35_bi,	75_bi,		// Doduo	
		110_bi,	70_bi,	60_bi,	60_bi,	100_bi,		// Dodrio	
		45_bi,	55_bi,	45_bi,	70_bi,	45_bi,		// Seel	
		70_bi,	80_bi,	70_bi,	95_bi,	70_bi,		// Dewgong	
		80_bi,	50_bi,	40_bi,	50_bi,	25_bi,		// Grimer	
		105_bi,	75_bi,	65_bi,	100_bi,	50_bi,		// Muk	
		65_bi,	100_bi,	45_bi,	25_bi,	40_bi,		// Shellder	
		95_bi,	180_bi,	85_bi,	45_bi,	70_bi,		// Cloyster	
		35_bi,	30_bi,	100_bi,	35_bi,	80_bi,		// Gastly	
		50_bi,	45_bi,	115_bi,	55_bi,	95_bi,		// Haunter	
		65_bi,	60_bi,	130_bi,	75_bi,	110_bi,		// Gengar	
		45_bi,	160_bi,	30_bi,	45_bi,	70_bi,		// Onix	
		48_bi,	45_bi,	43_bi,	90_bi,	42_bi,		// Drowzee	
		73_bi,	70_bi,	73_bi,	115_bi,	67_bi,		// Hypno	
		105_bi,	90_bi,	25_bi,	25_bi,	50_bi,		// Krabby	
		130_bi,	115_bi,	50_bi,	50_bi,	75_bi,		// Kingler	
		30_bi,	50_bi,	55_bi,	55_bi,	100_bi,		// Voltorb	
		50_bi,	70_bi,	80_bi,	80_bi,	140_bi,		// Electrode	
		40_bi,	80_bi,	60_bi,	45_bi,	40_bi,		// Exeggcute	
		95_bi,	85_bi,	125_bi,	65_bi,	55_bi,		// Exeggutor	
		50_bi,	95_bi,	40_bi,	50_bi,	35_bi,		// Cubone	
		80_bi,	110_bi,	50_bi,	80_bi,	45_bi,		// Marowak	
		120_bi,	53_bi,	35_bi,	110_bi,	87_bi,		// Hitmonlee	
		105_bi,	79_bi,	35_bi,	110_bi,	76_bi,		// Hitmonchan	
		55_bi,	75_bi,	60_bi,	75_bi,	30_bi,		// Lickitung	
		65_bi,	95_bi,	60_bi,	45_bi,	35_bi,		// Koffing	
		90_bi,	120_bi,	85_bi,	70_bi,	60_bi,		// Weezing	
		85_bi,	95_bi,	30_bi,	30_bi,	25_bi,		// Rhyhorn	
		130_bi,	120_bi,	45_bi,	45_bi,	40_bi,		// Rhydon	
		5_bi,	5_bi,	35_bi,	105_bi,	50_bi,		// Chansey	
		55_bi,	115_bi,	100_bi,	40_bi,	60_bi,		// Tangela	
		95_bi,	80_bi,	40_bi,	80_bi,	90_bi,		// Kangaskhan	
		40_bi,	70_bi,	70_bi,	25_bi,	60_bi,		// Horsea	
		65_bi,	95_bi,	95_bi,	45_bi,	85_bi,		// Seadra	
		67_bi,	60_bi,	35_bi,	50_bi,	63_bi,		// Goldeen	
		92_bi,	65_bi,	65_bi,	80_bi,	68_bi,		// Seaking	
		45_bi,	55_bi,	70_bi,	55_bi,	85_bi,		// Staryu	
		75_bi,	85_bi,	100_bi,	85_bi,	115_bi,		// Starmie	
		45_bi,	65_bi,	100_bi,	120_bi,	90_bi,		// Mr. Mime	
		110_bi,	80_bi,	55_bi,	80_bi,	105_bi,		// Scyther	
		50_bi,	35_bi,	115_bi,	95_bi,	95_bi,		// Jynx	
		83_bi,	57_bi,	95_bi,	85_bi,	105_bi,		// Electabuzz	
		95_bi,	57_bi,	100_bi,	85_bi,	93_bi,		// Magmar	
		125_bi,	100_bi,	55_bi,	70_bi,	85_bi,		// Pinsir	
		100_bi,	95_bi,	40_bi,	70_bi,	110_bi,		// Tauros	
		10_bi,	55_bi,	15_bi,	20_bi,	80_bi,		// Magikarp	
		125_bi,	79_bi,	60_bi,	100_bi,	81_bi,		// Gyarados	
		85_bi,	80_bi,	85_bi,	95_bi,	60_bi,		// Lapras	
		48_bi,	48_bi,	48_bi,	48_bi,	48_bi,		// Ditto	
		55_bi,	50_bi,	45_bi,	65_bi,	55_bi,		// Eevee	
		65_bi,	60_bi,	110_bi,	95_bi,	65_bi,		// Vaporeon	
		65_bi,	60_bi,	110_bi,	95_bi,	130_bi,		// Jolteon	
		130_bi,	60_bi,	95_bi,	110_bi,	65_bi,		// Flareon	
		60_bi,	70_bi,	85_bi,	75_bi,	40_bi,		// Porygon	
		40_bi,	100_bi,	90_bi,	55_bi,	35_bi,		// Omanyte	
		60_bi,	125_bi,	115_bi,	70_bi,	55_bi,		// Omastar	
		80_bi,	90_bi,	55_bi,	45_bi,	55_bi,		// Kabuto	
		115_bi,	105_bi,	65_bi,	70_bi,	80_bi,		// Kabutops	
		105_bi,	65_bi,	60_bi,	75_bi,	130_bi,		// Aerodactyl	
		110_bi,	65_bi,	65_bi,	110_bi,	30_bi,		// Snorlax	
		85_bi,	100_bi,	95_bi,	125_bi,	85_bi,		// Articuno	
		90_bi,	85_bi,	125_bi,	90_bi,	100_bi,		// Zapdos	
		100_bi,	90_bi,	125_bi,	85_bi,	90_bi,		// Moltres	
		64_bi,	45_bi,	50_bi,	50_bi,	50_bi,		// Dratini	
		84_bi,	65_bi,	70_bi,	70_bi,	70_bi,		// Dragonair	
		134_bi,	95_bi,	100_bi,	100_bi,	80_bi,		// Dragonite	
		110_bi,	90_bi,	154_bi,	90_bi,	130_bi,		// Mewtwo	
		100_bi,	100_bi,	100_bi,	100_bi,	100_bi,		// Mew	
							
		// Generation 2							
		49_bi,	65_bi,	49_bi,	65_bi,	45_bi,		// Chikorita	
		62_bi,	80_bi,	63_bi,	80_bi,	60_bi,		// Bayleef	
		82_bi,	100_bi,	83_bi,	100_bi,	80_bi,		// Meganium	
		52_bi,	43_bi,	60_bi,	50_bi,	65_bi,		// Cyndaquil	
		64_bi,	58_bi,	80_bi,	65_bi,	80_bi,		// Quilava	
		84_bi,	78_bi,	109_bi,	85_bi,	100_bi,		// Typhlosion	
		65_bi,	64_bi,	44_bi,	48_bi,	43_bi,		// Totodile	
		80_bi,	80_bi,	59_bi,	63_bi,	58_bi,		// Croconaw	
		105_bi,	100_bi,	79_bi,	83_bi,	78_bi,		// Feraligatr	
		46_bi,	34_bi,	35_bi,	45_bi,	20_bi,		// Sentret	
		76_bi,	64_bi,	45_bi,	55_bi,	90_bi,		// Furret	
		30_bi,	30_bi,	36_bi,	56_bi,	50_bi,		// Hoothoot	
		50_bi,	50_bi,	76_bi,	96_bi,	70_bi,		// Noctowl	
		20_bi,	30_bi,	40_bi,	80_bi,	55_bi,		// Ledyba	
		35_bi,	50_bi,	55_bi,	110_bi,	85_bi,		// Ledian	
		60_bi,	40_bi,	40_bi,	40_bi,	30_bi,		// Spinarak	
		90_bi,	70_bi,	60_bi,	60_bi,	40_bi,		// Ariados	
		90_bi,	80_bi,	70_bi,	80_bi,	130_bi,		// Crobat	
		38_bi,	38_bi,	56_bi,	56_bi,	67_bi,		// Chinchou	
		58_bi,	58_bi,	76_bi,	76_bi,	67_bi,		// Lanturn	
		40_bi,	15_bi,	35_bi,	35_bi,	60_bi,		// Pichu	
		25_bi,	28_bi,	45_bi,	55_bi,	15_bi,		// Cleffa	
		30_bi,	15_bi,	40_bi,	20_bi,	15_bi,		// Igglybuff	
		20_bi,	65_bi,	40_bi,	65_bi,	20_bi,		// Togepi	
		40_bi,	85_bi,	80_bi,	105_bi,	40_bi,		// Togetic	
		50_bi,	45_bi,	70_bi,	45_bi,	70_bi,		// Natu	
		75_bi,	70_bi,	95_bi,	70_bi,	95_bi,		// Xatu	
		40_bi,	40_bi,	65_bi,	45_bi,	35_bi,		// Mareep	
		55_bi,	55_bi,	80_bi,	60_bi,	45_bi,		// Flaaffy	
		75_bi,	75_bi,	115_bi,	90_bi,	55_bi,		// Ampharos	
		80_bi,	85_bi,	90_bi,	100_bi,	50_bi,		// Bellossom	
		20_bi,	50_bi,	20_bi,	50_bi,	40_bi,		// Marill	
		50_bi,	80_bi,	50_bi,	80_bi,	50_bi,		// Azumarill	
		100_bi,	115_bi,	30_bi,	65_bi,	30_bi,		// Sudowoodo	
		75_bi,	75_bi,	90_bi,	100_bi,	70_bi,		// Politoed	
		35_bi,	40_bi,	35_bi,	55_bi,	50_bi,		// Hoppip	
		45_bi,	50_bi,	45_bi,	65_bi,	80_bi,		// Skiploom	
		55_bi,	70_bi,	55_bi,	85_bi,	110_bi,		// Jumpluff	
		70_bi,	55_bi,	40_bi,	55_bi,	85_bi,		// Aipom	
		30_bi,	30_bi,	30_bi,	30_bi,	30_bi,		// Sunkern	
		75_bi,	55_bi,	105_bi,	85_bi,	30_bi,		// Sunflora	
		65_bi,	45_bi,	75_bi,	45_bi,	95_bi,		// Yanma	
		45_bi,	45_bi,	25_bi,	25_bi,	15_bi,		// Wooper	
		85_bi,	85_bi,	65_bi,	65_bi,	35_bi,		// Quagsire	
		65_bi,	60_bi,	130_bi,	95_bi,	110_bi,		// Espeon	
		65_bi,	110_bi,	60_bi,	130_bi,	65_bi,		// Umbreon	
		85_bi,	42_bi,	85_bi,	42_bi,	91_bi,		// Murkrow	
		75_bi,	80_bi,	100_bi,	110_bi,	30_bi,		// Slowking	
		60_bi,	60_bi,	85_bi,	85_bi,	85_bi,		// Misdreavus	
		72_bi,	48_bi,	72_bi,	48_bi,	48_bi,		// Unown	
		33_bi,	58_bi,	33_bi,	58_bi,	33_bi,		// Wobbuffet	
		80_bi,	65_bi,	90_bi,	65_bi,	85_bi,		// Girafarig	
		65_bi,	90_bi,	35_bi,	35_bi,	15_bi,		// Pineco	
		90_bi,	140_bi,	60_bi,	60_bi,	40_bi,		// Forretress	
		70_bi,	70_bi,	65_bi,	65_bi,	45_bi,		// Dunsparce	
		75_bi,	105_bi,	35_bi,	65_bi,	85_bi,		// Gligar	
		85_bi,	200_bi,	55_bi,	65_bi,	30_bi,		// Steelix	
		80_bi,	50_bi,	40_bi,	40_bi,	30_bi,		// Snubbull	
		120_bi,	75_bi,	60_bi,	60_bi,	45_bi,		// Granbull	
		95_bi,	75_bi,	55_bi,	55_bi,	85_bi,		// Qwilfish	
		130_bi,	100_bi,	55_bi,	80_bi,	65_bi,		// Scizor	
		10_bi,	230_bi,	10_bi,	230_bi,	5_bi,		// Shuckle	
		125_bi,	75_bi,	40_bi,	95_bi,	85_bi,		// Heracross	
		95_bi,	55_bi,	35_bi,	75_bi,	115_bi,		// Sneasel	
		80_bi,	50_bi,	50_bi,	50_bi,	40_bi,		// Teddiursa	
		130_bi,	75_bi,	75_bi,	75_bi,	55_bi,		// Ursaring	
		40_bi,	40_bi,	70_bi,	40_bi,	20_bi,		// Slugma	
		50_bi,	120_bi,	80_bi,	80_bi,	30_bi,		// Magcargo	
		50_bi,	40_bi,	30_bi,	30_bi,	50_bi,		// Swinub	
		100_bi,	80_bi,	60_bi,	60_bi,	50_bi,		// Piloswine	
		55_bi,	85_bi,	65_bi,	85_bi,	35_bi,		// Corsola	
		65_bi,	35_bi,	65_bi,	35_bi,	65_bi,		// Remoraid	
		105_bi,	75_bi,	105_bi,	75_bi,	45_bi,		// Octillery	
		55_bi,	45_bi,	65_bi,	45_bi,	75_bi,		// Delibird	
		40_bi,	70_bi,	80_bi,	140_bi,	70_bi,		// Mantine	
		80_bi,	140_bi,	40_bi,	70_bi,	70_bi,		// Skarmory	
		60_bi,	30_bi,	80_bi,	50_bi,	65_bi,		// Houndour	
		90_bi,	50_bi,	110_bi,	80_bi,	95_bi,		// Houndoom	
		95_bi,	95_bi,	95_bi,	95_bi,	85_bi,		// Kingdra	
		60_bi,	60_bi,	40_bi,	40_bi,	40_bi,		// Phanpy	
		120_bi,	120_bi,	60_bi,	60_bi,	50_bi,		// Donphan	
		80_bi,	90_bi,	105_bi,	95_bi,	60_bi,		// Porygon2	
		95_bi,	62_bi,	85_bi,	65_bi,	85_bi,		// Stantler	
		20_bi,	35_bi,	20_bi,	45_bi,	75_bi,		// Smeargle	
		35_bi,	35_bi,	35_bi,	35_bi,	35_bi,		// Tyrogue	
		95_bi,	95_bi,	35_bi,	110_bi,	70_bi,		// Hitmontop	
		30_bi,	15_bi,	85_bi,	65_bi,	65_bi,		// Smoochum	
		63_bi,	37_bi,	65_bi,	55_bi,	95_bi,		// Elekid	
		75_bi,	37_bi,	70_bi,	55_bi,	83_bi,		// Magby	
		80_bi,	105_bi,	40_bi,	70_bi,	100_bi,		// Miltank	
		10_bi,	10_bi,	75_bi,	135_bi,	55_bi,		// Blissey	
		85_bi,	75_bi,	115_bi,	100_bi,	115_bi,		// Raikou	
		115_bi,	85_bi,	90_bi,	75_bi,	100_bi,		// Entei	
		75_bi,	115_bi,	90_bi,	115_bi,	85_bi,		// Suicune	
		64_bi,	50_bi,	45_bi,	50_bi,	41_bi,		// Larvitar	
		84_bi,	70_bi,	65_bi,	70_bi,	51_bi,		// Pupitar	
		134_bi,	110_bi,	95_bi,	100_bi,	61_bi,		// Tyranitar	
		90_bi,	130_bi,	90_bi,	154_bi,	110_bi,		// Lugia	
		130_bi,	90_bi,	110_bi,	154_bi,	90_bi,		// Ho-Oh	
		100_bi,	100_bi,	100_bi,	100_bi,	100_bi,		// Celebi	
							
		// Generation 3							
		45_bi,	35_bi,	65_bi,	55_bi,	70_bi,		// Treecko	
		65_bi,	45_bi,	85_bi,	65_bi,	95_bi,		// Grovyle	
		85_bi,	65_bi,	105_bi,	85_bi,	120_bi,		// Sceptile	
		60_bi,	40_bi,	70_bi,	50_bi,	45_bi,		// Torchic	
		85_bi,	60_bi,	85_bi,	60_bi,	55_bi,		// Combusken	
		120_bi,	70_bi,	110_bi,	70_bi,	80_bi,		// Blaziken	
		70_bi,	50_bi,	50_bi,	50_bi,	40_bi,		// Mudkip	
		85_bi,	70_bi,	60_bi,	70_bi,	50_bi,		// Marshtomp	
		110_bi,	90_bi,	85_bi,	90_bi,	60_bi,		// Swampert	
		55_bi,	35_bi,	30_bi,	30_bi,	35_bi,		// Poochyena	
		90_bi,	70_bi,	60_bi,	60_bi,	70_bi,		// Mightyena	
		30_bi,	41_bi,	30_bi,	41_bi,	60_bi,		// Zigzagoon	
		70_bi,	61_bi,	50_bi,	61_bi,	100_bi,		// Linoone	
		45_bi,	35_bi,	20_bi,	30_bi,	20_bi,		// Wurmple	
		35_bi,	55_bi,	25_bi,	25_bi,	15_bi,		// Silcoon	
		70_bi,	50_bi,	90_bi,	50_bi,	65_bi,		// Beautifly	
		35_bi,	55_bi,	25_bi,	25_bi,	15_bi,		// Cascoon	
		50_bi,	70_bi,	50_bi,	90_bi,	65_bi,		// Dustox	
		30_bi,	30_bi,	40_bi,	50_bi,	30_bi,		// Lotad	
		50_bi,	50_bi,	60_bi,	70_bi,	50_bi,		// Lombre	
		70_bi,	70_bi,	90_bi,	100_bi,	70_bi,		// Ludicolo	
		40_bi,	50_bi,	30_bi,	30_bi,	30_bi,		// Seedot	
		70_bi,	40_bi,	60_bi,	40_bi,	60_bi,		// Nuzleaf	
		100_bi,	60_bi,	90_bi,	60_bi,	80_bi,		// Shiftry	
		55_bi,	30_bi,	30_bi,	30_bi,	85_bi,		// Taillow	
		85_bi,	60_bi,	50_bi,	50_bi,	125_bi,		// Swellow	
		30_bi,	30_bi,	55_bi,	30_bi,	85_bi,		// Wingull	
		50_bi,	100_bi,	85_bi,	70_bi,	65_bi,		// Pelipper	
		25_bi,	25_bi,	45_bi,	35_bi,	40_bi,		// Ralts	
		35_bi,	35_bi,	65_bi,	55_bi,	50_bi,		// Kirlia	
		65_bi,	65_bi,	125_bi,	115_bi,	80_bi,		// Gardevoir	
		30_bi,	32_bi,	50_bi,	52_bi,	65_bi,		// Surskit	
		60_bi,	62_bi,	80_bi,	82_bi,	60_bi,		// Masquerain	
		40_bi,	60_bi,	40_bi,	60_bi,	35_bi,		// Shroomish	
		130_bi,	80_bi,	60_bi,	60_bi,	70_bi,		// Breloom	
		60_bi,	60_bi,	35_bi,	35_bi,	30_bi,		// Slakoth	
		80_bi,	80_bi,	55_bi,	55_bi,	90_bi,		// Vigoroth	
		160_bi,	100_bi,	95_bi,	65_bi,	100_bi,		// Slaking	
		45_bi,	90_bi,	30_bi,	30_bi,	40_bi,		// Nincada	
		90_bi,	45_bi,	50_bi,	50_bi,	160_bi,		// Ninjask	
		90_bi,	45_bi,	30_bi,	30_bi,	40_bi,		// Shedinja	
		51_bi,	23_bi,	51_bi,	23_bi,	28_bi,		// Whismur	
		71_bi,	43_bi,	71_bi,	43_bi,	48_bi,		// Loudred	
		91_bi,	63_bi,	91_bi,	63_bi,	68_bi,		// Exploud	
		60_bi,	30_bi,	20_bi,	30_bi,	25_bi,		// Makuhita	
		120_bi,	60_bi,	40_bi,	60_bi,	50_bi,		// Hariyama	
		20_bi,	40_bi,	20_bi,	40_bi,	20_bi,		// Azurill	
		45_bi,	135_bi,	45_bi,	90_bi,	30_bi,		// Nosepass	
		45_bi,	45_bi,	35_bi,	35_bi,	50_bi,		// Skitty	
		65_bi,	65_bi,	55_bi,	55_bi,	70_bi,		// Delcatty	
		75_bi,	75_bi,	65_bi,	65_bi,	50_bi,		// Sableye	
		85_bi,	85_bi,	55_bi,	55_bi,	50_bi,		// Mawile	
		70_bi,	100_bi,	40_bi,	40_bi,	30_bi,		// Aron	
		90_bi,	140_bi,	50_bi,	50_bi,	40_bi,		// Lairon	
		110_bi,	180_bi,	60_bi,	60_bi,	50_bi,		// Aggron	
		40_bi,	55_bi,	40_bi,	55_bi,	60_bi,		// Meditite	
		60_bi,	75_bi,	60_bi,	75_bi,	80_bi,		// Medicham	
		45_bi,	40_bi,	65_bi,	40_bi,	65_bi,		// Electrike	
		75_bi,	60_bi,	105_bi,	60_bi,	105_bi,		// Manectric	
		50_bi,	40_bi,	85_bi,	75_bi,	95_bi,		// Plusle	
		40_bi,	50_bi,	75_bi,	85_bi,	95_bi,		// Minun	
		73_bi,	55_bi,	47_bi,	75_bi,	85_bi,		// Volbeat	
		47_bi,	55_bi,	73_bi,	75_bi,	85_bi,		// Illumise	
		60_bi,	45_bi,	100_bi,	80_bi,	65_bi,		// Roselia	
		43_bi,	53_bi,	43_bi,	53_bi,	40_bi,		// Gulpin	
		73_bi,	83_bi,	73_bi,	83_bi,	55_bi,		// Swalot	
		90_bi,	20_bi,	65_bi,	20_bi,	65_bi,		// Carvanha	
		120_bi,	40_bi,	95_bi,	40_bi,	95_bi,		// Sharpedo	
		70_bi,	35_bi,	70_bi,	35_bi,	60_bi,		// Wailmer	
		90_bi,	45_bi,	90_bi,	45_bi,	60_bi,		// Wailord	
		60_bi,	40_bi,	65_bi,	45_bi,	35_bi,		// Numel	
		100_bi,	70_bi,	105_bi,	75_bi,	40_bi,		// Camerupt	
		85_bi,	140_bi,	85_bi,	70_bi,	20_bi,		// Torkoal	
		25_bi,	35_bi,	70_bi,	80_bi,	60_bi,		// Spoink	
		45_bi,	65_bi,	90_bi,	110_bi,	80_bi,		// Grumpig	
		60_bi,	60_bi,	60_bi,	60_bi,	60_bi,		// Spinda	
		100_bi,	45_bi,	45_bi,	45_bi,	10_bi,		// Trapinch	
		70_bi,	50_bi,	50_bi,	50_bi,	70_bi,		// Vibrava	
		100_bi,	80_bi,	80_bi,	80_bi,	100_bi,		// Flygon	
		85_bi,	40_bi,	85_bi,	40_bi,	35_bi,		// Cacnea	
		115_bi,	60_bi,	115_bi,	60_bi,	55_bi,		// Cacturne	
		40_bi,	60_bi,	40_bi,	75_bi,	50_bi,		// Swablu	
		70_bi,	90_bi,	70_bi,	105_bi,	80_bi,		// Altaria	
		115_bi,	60_bi,	60_bi,	60_bi,	90_bi,		// Zangoose	
		100_bi,	60_bi,	100_bi,	60_bi,	65_bi,		// Seviper	
		55_bi,	65_bi,	95_bi,	85_bi,	70_bi,		// Lunatone	
		95_bi,	85_bi,	55_bi,	65_bi,	70_bi,		// Solrock	
		48_bi,	43_bi,	46_bi,	41_bi,	60_bi,		// Barboach	
		78_bi,	73_bi,	76_bi,	71_bi,	60_bi,		// Whiscash	
		80_bi,	65_bi,	50_bi,	35_bi,	35_bi,		// Corphish	
		120_bi,	85_bi,	90_bi,	55_bi,	55_bi,		// Crawdaunt	
		40_bi,	55_bi,	40_bi,	70_bi,	55_bi,		// Baltoy	
		70_bi,	105_bi,	70_bi,	120_bi,	75_bi,		// Claydol	
		41_bi,	77_bi,	61_bi,	87_bi,	23_bi,		// Lileep	
		81_bi,	97_bi,	81_bi,	107_bi,	43_bi,		// Cradily	
		95_bi,	50_bi,	40_bi,	50_bi,	75_bi,		// Anorith	
		125_bi,	100_bi,	70_bi,	80_bi,	45_bi,		// Armaldo	
		15_bi,	20_bi,	10_bi,	55_bi,	80_bi,		// Feebas	
		60_bi,	79_bi,	100_bi,	125_bi,	81_bi,		// Milotic	
		70_bi,	70_bi,	70_bi,	70_bi,	70_bi,		// Castform	
		90_bi,	70_bi,	60_bi,	120_bi,	40_bi,		// Kecleon	
		75_bi,	35_bi,	63_bi,	33_bi,	45_bi,		// Shuppet	
		115_bi,	65_bi,	83_bi,	63_bi,	65_bi,		// Banette	
		40_bi,	90_bi,	30_bi,	90_bi,	25_bi,		// Duskull	
		70_bi,	130_bi,	60_bi,	130_bi,	25_bi,		// Dusclops	
		68_bi,	83_bi,	72_bi,	87_bi,	51_bi,		// Tropius	
		50_bi,	70_bi,	95_bi,	80_bi,	65_bi,		// Chimecho	
		130_bi,	60_bi,	75_bi,	60_bi,	75_bi,		// Absol	
		23_bi,	48_bi,	23_bi,	48_bi,	23_bi,		// Wynaut	
		50_bi,	50_bi,	50_bi,	50_bi,	50_bi,		// Snorunt	
		80_bi,	80_bi,	80_bi,	80_bi,	80_bi,		// Glalie	
		40_bi,	50_bi,	55_bi,	50_bi,	25_bi,		// Spheal	
		60_bi,	70_bi,	75_bi,	70_bi,	45_bi,		// Sealeo	
		80_bi,	90_bi,	95_bi,	90_bi,	65_bi,		// Walrein	
		64_bi,	85_bi,	74_bi,	55_bi,	32_bi,		// Clamperl	
		104_bi,	105_bi,	94_bi,	75_bi,	52_bi,		// Huntail	
		84_bi,	105_bi,	114_bi,	75_bi,	52_bi,		// Gorebyss	
		90_bi,	130_bi,	45_bi,	65_bi,	55_bi,		// Relicanth	
		30_bi,	55_bi,	40_bi,	65_bi,	97_bi,		// Luvdisc	
		75_bi,	60_bi,	40_bi,	30_bi,	50_bi,		// Bagon	
		95_bi,	100_bi,	60_bi,	50_bi,	50_bi,		// Shelgon	
		135_bi,	80_bi,	110_bi,	80_bi,	100_bi,		// Salamence	
		55_bi,	80_bi,	35_bi,	60_bi,	30_bi,		// Beldum	
		75_bi,	100_bi,	55_bi,	80_bi,	50_bi,		// Metang	
		135_bi,	130_bi,	95_bi,	90_bi,	70_bi,		// Metagross	
		100_bi,	200_bi,	50_bi,	100_bi,	50_bi,		// Regirock	
		50_bi,	100_bi,	100_bi,	200_bi,	50_bi,		// Regice	
		75_bi,	150_bi,	75_bi,	150_bi,	50_bi,		// Registeel	
		80_bi,	90_bi,	110_bi,	130_bi,	110_bi,		// Latias	
		90_bi,	80_bi,	130_bi,	110_bi,	110_bi,		// Latios	
		100_bi,	90_bi,	150_bi,	140_bi,	90_bi,		// Kyogre	
		150_bi,	140_bi,	100_bi,	90_bi,	90_bi,		// Groudon	
		150_bi,	90_bi,	150_bi,	90_bi,	95_bi,		// Rayquaza	
		100_bi,	100_bi,	100_bi,	100_bi,	100_bi,		// Jirachi	
		150_bi,	50_bi,	150_bi,	50_bi,	150_bi,		// Deoxys-Mediocre	
		180_bi,	20_bi,	180_bi,	20_bi,	150_bi,		// Deoxys-Attack	
		70_bi,	160_bi,	70_bi,	160_bi,	90_bi,		// Deoxys-Defense	
		95_bi,	90_bi,	95_bi,	90_bi,	180_bi,		// Deoxys-Speed	
							
		// Generation 4							
		68_bi,	64_bi,	45_bi,	55_bi,	31_bi,		// Turtwig	
		89_bi,	85_bi,	55_bi,	65_bi,	36_bi,		// Grotle	
		109_bi,	105_bi,	75_bi,	85_bi,	56_bi,		// Torterra	
		58_bi,	44_bi,	58_bi,	44_bi,	61_bi,		// Chimchar	
		78_bi,	52_bi,	78_bi,	52_bi,	81_bi,		// Monferno	
		104_bi,	71_bi,	104_bi,	71_bi,	108_bi,		// Infernape	
		51_bi,	53_bi,	61_bi,	56_bi,	40_bi,		// Piplup	
		66_bi,	68_bi,	81_bi,	76_bi,	50_bi,		// Prinplup	
		86_bi,	88_bi,	111_bi,	101_bi,	60_bi,		// Empoleon	
		55_bi,	30_bi,	30_bi,	30_bi,	60_bi,		// Starly	
		75_bi,	50_bi,	40_bi,	40_bi,	80_bi,		// Staravia	
		120_bi,	70_bi,	50_bi,	50_bi,	100_bi,		// Staraptor	
		45_bi,	40_bi,	35_bi,	40_bi,	31_bi,		// Bidoof	
		85_bi,	60_bi,	55_bi,	60_bi,	71_bi,		// Bibarel	
		25_bi,	41_bi,	25_bi,	41_bi,	25_bi,		// Kricketot	
		85_bi,	51_bi,	55_bi,	51_bi,	65_bi,		// Kricketune	
		65_bi,	34_bi,	40_bi,	34_bi,	45_bi,		// Shinx	
		85_bi,	49_bi,	60_bi,	49_bi,	60_bi,		// Luxio	
		120_bi,	79_bi,	95_bi,	79_bi,	70_bi,		// Luxray	
		30_bi,	35_bi,	50_bi,	70_bi,	55_bi,		// Budew	
		70_bi,	55_bi,	125_bi,	105_bi,	90_bi,		// Roserade	
		125_bi,	40_bi,	30_bi,	30_bi,	58_bi,		// Cranidos	
		165_bi,	60_bi,	65_bi,	50_bi,	58_bi,		// Rampardos	
		42_bi,	118_bi,	42_bi,	88_bi,	30_bi,		// Shieldon	
		52_bi,	168_bi,	47_bi,	138_bi,	30_bi,		// Bastiodon	
		29_bi,	45_bi,	29_bi,	45_bi,	36_bi,		// Burmy	
		59_bi,	85_bi,	79_bi,	105_bi,	36_bi,		// Wormadam-Plant	
		79_bi,	105_bi,	59_bi,	85_bi,	36_bi,		// Wormadam-Sandy	
		69_bi,	95_bi,	69_bi,	95_bi,	36_bi,		// Wormadam-Trash	
		94_bi,	50_bi,	94_bi,	50_bi,	66_bi,		// Mothim	
		30_bi,	42_bi,	30_bi,	42_bi,	70_bi,		// Combee	
		80_bi,	102_bi,	80_bi,	102_bi,	40_bi,		// Vespiquen	
		45_bi,	70_bi,	45_bi,	90_bi,	95_bi,		// Pachirisu	
		65_bi,	35_bi,	60_bi,	30_bi,	85_bi,		// Buizel	
		105_bi,	55_bi,	85_bi,	50_bi,	115_bi,		// Floatzel	
		35_bi,	45_bi,	62_bi,	53_bi,	35_bi,		// Cherubi	
		60_bi,	70_bi,	87_bi,	78_bi,	85_bi,		// Cherrim	
		48_bi,	48_bi,	57_bi,	62_bi,	34_bi,		// Shellos	
		83_bi,	68_bi,	92_bi,	82_bi,	39_bi,		// Gastrodon	
		100_bi,	66_bi,	60_bi,	66_bi,	115_bi,		// Ambipom	
		50_bi,	34_bi,	60_bi,	44_bi,	70_bi,		// Drifloon	
		80_bi,	44_bi,	90_bi,	54_bi,	80_bi,		// Drifblim	
		66_bi,	44_bi,	44_bi,	56_bi,	85_bi,		// Buneary	
		76_bi,	84_bi,	54_bi,	96_bi,	105_bi,		// Lopunny	
		60_bi,	60_bi,	105_bi,	105_bi,	105_bi,		// Mismagius	
		125_bi,	52_bi,	105_bi,	52_bi,	71_bi,		// Honchkrow	
		55_bi,	42_bi,	42_bi,	37_bi,	85_bi,		// Glameow	
		82_bi,	64_bi,	64_bi,	59_bi,	112_bi,		// Purugly	
		30_bi,	50_bi,	65_bi,	50_bi,	45_bi,		// Chingling	
		63_bi,	47_bi,	41_bi,	41_bi,	74_bi,		// Stunky	
		93_bi,	67_bi,	71_bi,	61_bi,	84_bi,		// Skuntank	
		24_bi,	86_bi,	24_bi,	86_bi,	23_bi,		// Bronzor	
		89_bi,	116_bi,	79_bi,	116_bi,	33_bi,		// Bronzong	
		80_bi,	95_bi,	10_bi,	45_bi,	10_bi,		// Bonsly	
		25_bi,	45_bi,	70_bi,	90_bi,	60_bi,		// Mime Jr.	
		5_bi,	5_bi,	15_bi,	65_bi,	30_bi,		// Happiny	
		65_bi,	45_bi,	92_bi,	42_bi,	91_bi,		// Chatot	
		92_bi,	108_bi,	92_bi,	108_bi,	35_bi,		// Spiritomb	
		70_bi,	45_bi,	40_bi,	45_bi,	42_bi,		// Gible	
		90_bi,	65_bi,	50_bi,	55_bi,	82_bi,		// Gabite	
		130_bi,	95_bi,	80_bi,	85_bi,	102_bi,		// Garchomp	
		85_bi,	40_bi,	40_bi,	85_bi,	5_bi,		// Munchlax	
		70_bi,	40_bi,	35_bi,	40_bi,	60_bi,		// Riolu	
		110_bi,	70_bi,	115_bi,	70_bi,	90_bi,		// Lucario	
		72_bi,	78_bi,	38_bi,	42_bi,	32_bi,		// Hippopotas	
		112_bi,	118_bi,	68_bi,	72_bi,	47_bi,		// Hippowdon	
		50_bi,	90_bi,	30_bi,	55_bi,	65_bi,		// Skorupi	
		90_bi,	110_bi,	60_bi,	75_bi,	95_bi,		// Drapion	
		61_bi,	40_bi,	61_bi,	40_bi,	50_bi,		// Croagunk	
		106_bi,	65_bi,	86_bi,	65_bi,	85_bi,		// Toxicroak	
		100_bi,	72_bi,	90_bi,	72_bi,	46_bi,		// Carnivine	
		49_bi,	56_bi,	49_bi,	61_bi,	66_bi,		// Finneon	
		69_bi,	76_bi,	69_bi,	86_bi,	91_bi,		// Lumineon	
		20_bi,	50_bi,	60_bi,	120_bi,	50_bi,		// Mantyke	
		62_bi,	50_bi,	62_bi,	60_bi,	40_bi,		// Snover	
		92_bi,	75_bi,	92_bi,	85_bi,	60_bi,		// Abomasnow	
		120_bi,	65_bi,	45_bi,	85_bi,	125_bi,		// Weavile	
		70_bi,	115_bi,	130_bi,	90_bi,	60_bi,		// Magnezone	
		85_bi,	95_bi,	80_bi,	95_bi,	50_bi,		// Lickilicky	
		140_bi,	130_bi,	55_bi,	55_bi,	40_bi,		// Rhyperior	
		100_bi,	125_bi,	110_bi,	50_bi,	50_bi,		// Tangrowth	
		123_bi,	67_bi,	95_bi,	85_bi,	95_bi,		// Electivire	
		95_bi,	67_bi,	125_bi,	95_bi,	83_bi,		// Magmortar	
		50_bi,	95_bi,	120_bi,	115_bi,	80_bi,		// Togekiss	
		76_bi,	86_bi,	116_bi,	56_bi,	95_bi,		// Yanmega	
		110_bi,	130_bi,	60_bi,	65_bi,	95_bi,		// Leafeon	
		60_bi,	110_bi,	130_bi,	95_bi,	65_bi,		// Glaceon	
		95_bi,	125_bi,	45_bi,	75_bi,	95_bi,		// Gliscor	
		130_bi,	80_bi,	70_bi,	60_bi,	80_bi,		// Mamoswine	
		80_bi,	70_bi,	135_bi,	75_bi,	90_bi,		// Porygon-Z	
		125_bi,	65_bi,	65_bi,	115_bi,	80_bi,		// Gallade	
		55_bi,	145_bi,	75_bi,	150_bi,	40_bi,		// Probopass	
		100_bi,	135_bi,	65_bi,	135_bi,	45_bi,		// Dusknoir	
		80_bi,	70_bi,	80_bi,	70_bi,	110_bi,		// Froslass	
		50_bi,	77_bi,	95_bi,	77_bi,	91_bi,		// Rotom	
		65_bi,	107_bi,	105_bi,	107_bi,	86_bi,		// Rotom-Heat	
		65_bi,	107_bi,	105_bi,	107_bi,	86_bi,		// Rotom-Wash	
		65_bi,	107_bi,	105_bi,	107_bi,	86_bi,		// Rotom-Frost	
		65_bi,	107_bi,	105_bi,	107_bi,	86_bi,		// Rotom-Fan	
		65_bi,	107_bi,	105_bi,	107_bi,	86_bi,		// Rotom-Mow	
		75_bi,	130_bi,	75_bi,	130_bi,	95_bi,		// Uxie	
		105_bi,	105_bi,	105_bi,	105_bi,	80_bi,		// Mesprit	
		125_bi,	70_bi,	125_bi,	70_bi,	115_bi,		// Azelf	
		120_bi,	120_bi,	150_bi,	100_bi,	90_bi,		// Dialga	
		120_bi,	100_bi,	150_bi,	120_bi,	100_bi,		// Palkia	
		90_bi,	106_bi,	130_bi,	106_bi,	77_bi,		// Heatran	
		160_bi,	110_bi,	80_bi,	110_bi,	100_bi,		// Regigigas	
		100_bi,	120_bi,	100_bi,	120_bi,	90_bi,		// Giratina-Altered	
		120_bi,	100_bi,	120_bi,	100_bi,	90_bi,		// Giratina-Origin	
		70_bi,	120_bi,	75_bi,	130_bi,	85_bi,		// Cresselia	
		80_bi,	80_bi,	80_bi,	80_bi,	80_bi,		// Phione	
		100_bi,	100_bi,	100_bi,	100_bi,	100_bi,		// Manaphy	
		90_bi,	90_bi,	135_bi,	90_bi,	125_bi,		// Darkrai	
		100_bi,	100_bi,	100_bi,	100_bi,	100_bi,		// Shaymin-Land	
		103_bi,	75_bi,	120_bi,	75_bi,	127_bi,		// Shaymin-Sky	
		120_bi,	120_bi,	120_bi,	120_bi,	120_bi,		// Arceus	
							
		// Generation 5							
		100_bi,	100_bi,	100_bi,	100_bi,	100_bi,		// Victini	
		45_bi,	55_bi,	45_bi,	55_bi,	63_bi,		// Snivy	
		60_bi,	75_bi,	60_bi,	75_bi,	83_bi,		// Servine	
		75_bi,	95_bi,	75_bi,	95_bi,	113_bi,		// Serperior	
		63_bi,	45_bi,	45_bi,	45_bi,	45_bi,		// Tepig	
		93_bi,	55_bi,	70_bi,	55_bi,	55_bi,		// Pignite	
		123_bi,	65_bi,	100_bi,	65_bi,	65_bi,		// Emboar	
		55_bi,	45_bi,	63_bi,	45_bi,	45_bi,		// Oshawott	
		75_bi,	60_bi,	83_bi,	60_bi,	60_bi,		// Dewott	
		100_bi,	85_bi,	108_bi,	70_bi,	70_bi,		// Samurott	
		55_bi,	39_bi,	35_bi,	39_bi,	42_bi,		// Patrat	
		85_bi,	69_bi,	60_bi,	69_bi,	77_bi,		// Watchog	
		60_bi,	45_bi,	25_bi,	45_bi,	55_bi,		// Lillipup	
		80_bi,	65_bi,	35_bi,	65_bi,	60_bi,		// Herdier	
		100_bi,	90_bi,	45_bi,	90_bi,	80_bi,		// Stoutland	
		50_bi,	37_bi,	50_bi,	37_bi,	66_bi,		// Purrloin	
		88_bi,	50_bi,	88_bi,	50_bi,	106_bi,		// Liepard	
		53_bi,	48_bi,	53_bi,	48_bi,	64_bi,		// Pansage	
		98_bi,	63_bi,	98_bi,	63_bi,	101_bi,		// Simisage	
		53_bi,	48_bi,	53_bi,	48_bi,	64_bi,		// Pansear	
		98_bi,	63_bi,	98_bi,	63_bi,	101_bi,		// Simisear	
		53_bi,	48_bi,	53_bi,	48_bi,	64_bi,		// Panpour	
		98_bi,	63_bi,	98_bi,	63_bi,	101_bi,		// Simipour	
		25_bi,	45_bi,	67_bi,	55_bi,	24_bi,		// Munna	
		55_bi,	85_bi,	107_bi,	95_bi,	29_bi,		// Musharna	
		55_bi,	50_bi,	36_bi,	30_bi,	43_bi,		// Pidove	
		77_bi,	62_bi,	50_bi,	42_bi,	65_bi,		// Tranquill	
		105_bi,	80_bi,	65_bi,	55_bi,	93_bi,		// Unfezant	
		60_bi,	32_bi,	50_bi,	32_bi,	76_bi,		// Blitzle	
		100_bi,	63_bi,	80_bi,	63_bi,	116_bi,		// Zebstrika	
		75_bi,	85_bi,	25_bi,	25_bi,	15_bi,		// Roggenrola	
		105_bi,	105_bi,	50_bi,	40_bi,	20_bi,		// Boldore	
		135_bi,	130_bi,	60_bi,	70_bi,	25_bi,		// Gigalith	
		45_bi,	43_bi,	55_bi,	43_bi,	72_bi,		// Woobat	
		57_bi,	55_bi,	77_bi,	55_bi,	114_bi,		// Swoobat	
		85_bi,	40_bi,	30_bi,	45_bi,	68_bi,		// Drilbur	
		135_bi,	60_bi,	50_bi,	65_bi,	88_bi,		// Excadrill	
		60_bi,	86_bi,	60_bi,	86_bi,	50_bi,		// Audino	
		80_bi,	55_bi,	25_bi,	35_bi,	35_bi,		// Timburr	
		105_bi,	85_bi,	40_bi,	50_bi,	40_bi,		// Gurdurr	
		140_bi,	95_bi,	55_bi,	65_bi,	45_bi,		// Conkeldurr	
		50_bi,	40_bi,	50_bi,	40_bi,	64_bi,		// Tympole	
		65_bi,	55_bi,	65_bi,	55_bi,	69_bi,		// Palpitoad	
		85_bi,	75_bi,	85_bi,	75_bi,	74_bi,		// Seismitoad	
		100_bi,	85_bi,	30_bi,	85_bi,	45_bi,		// Throh	
		125_bi,	75_bi,	30_bi,	75_bi,	85_bi,		// Sawk	
		53_bi,	70_bi,	40_bi,	60_bi,	42_bi,		// Sewaddle	
		63_bi,	90_bi,	50_bi,	80_bi,	42_bi,		// Swadloon	
		103_bi,	80_bi,	70_bi,	70_bi,	92_bi,		// Leavanny	
		45_bi,	59_bi,	30_bi,	39_bi,	57_bi,		// Venipede	
		55_bi,	99_bi,	40_bi,	79_bi,	47_bi,		// Whirlipede	
		90_bi,	89_bi,	55_bi,	69_bi,	112_bi,		// Scolipede	
		27_bi,	60_bi,	37_bi,	50_bi,	66_bi,		// Cottonee	
		67_bi,	85_bi,	77_bi,	75_bi,	116_bi,		// Whimsicott	
		35_bi,	50_bi,	70_bi,	50_bi,	30_bi,		// Petilil	
		60_bi,	75_bi,	110_bi,	75_bi,	90_bi,		// Lilligant	
		92_bi,	65_bi,	80_bi,	55_bi,	98_bi,		// Basculin-Red	
		92_bi,	65_bi,	80_bi,	55_bi,	98_bi,		// Basculin-Blue	
		72_bi,	35_bi,	35_bi,	35_bi,	65_bi,		// Sandile	
		82_bi,	45_bi,	45_bi,	45_bi,	74_bi,		// Krokorok	
		117_bi,	70_bi,	65_bi,	70_bi,	92_bi,		// Krookodile	
		90_bi,	45_bi,	15_bi,	45_bi,	50_bi,		// Darumaka	
		140_bi,	55_bi,	30_bi,	55_bi,	95_bi,		// Darmanitan	
		// 30_bi,	105_bi,	140_bi,	105_bi,	55_bi,		// Darmanitan (Zen Mode)
		86_bi,	67_bi,	106_bi,	67_bi,	60_bi,		// Maractus	
		65_bi,	85_bi,	35_bi,	35_bi,	55_bi,		// Dwebble	
		95_bi,	125_bi,	65_bi,	75_bi,	45_bi,		// Crustle	
		75_bi,	70_bi,	35_bi,	70_bi,	48_bi,		// Scraggy	
		90_bi,	115_bi,	45_bi,	115_bi,	58_bi,		// Scrafty	
		58_bi,	80_bi,	103_bi,	80_bi,	97_bi,		// Sigilyph	
		30_bi,	85_bi,	55_bi,	65_bi,	30_bi,		// Yamask	
		50_bi,	145_bi,	95_bi,	105_bi,	30_bi,		// Cofagrigus	
		78_bi,	103_bi,	53_bi,	45_bi,	22_bi,		// Tirtouga	
		108_bi,	133_bi,	83_bi,	65_bi,	32_bi,		// Carracosta	
		112_bi,	45_bi,	74_bi,	45_bi,	70_bi,		// Archen	
		140_bi,	65_bi,	112_bi,	65_bi,	110_bi,		// Archeops	
		50_bi,	62_bi,	40_bi,	62_bi,	65_bi,		// Trubbish	
		95_bi,	82_bi,	60_bi,	82_bi,	75_bi,		// Garbodor	
		65_bi,	40_bi,	80_bi,	40_bi,	65_bi,		// Zorua	
		105_bi,	60_bi,	120_bi,	60_bi,	105_bi,		// Zoroark	
		50_bi,	40_bi,	40_bi,	40_bi,	75_bi,		// Minccino	
		95_bi,	60_bi,	65_bi,	60_bi,	115_bi,		// Cinccino	
		30_bi,	50_bi,	55_bi,	65_bi,	45_bi,		// Gothita	
		45_bi,	70_bi,	75_bi,	85_bi,	55_bi,		// Gothorita	
		55_bi,	95_bi,	95_bi,	110_bi,	65_bi,		// Gothitelle	
		30_bi,	40_bi,	105_bi,	50_bi,	20_bi,		// Solosis	
		40_bi,	50_bi,	125_bi,	60_bi,	30_bi,		// Duosion	
		65_bi,	75_bi,	125_bi,	85_bi,	30_bi,		// Reuniclus	
		44_bi,	50_bi,	44_bi,	50_bi,	55_bi,		// Ducklett	
		87_bi,	63_bi,	87_bi,	63_bi,	98_bi,		// Swanna	
		50_bi,	50_bi,	65_bi,	60_bi,	44_bi,		// Vanillite	
		65_bi,	65_bi,	80_bi,	75_bi,	59_bi,		// Vanillish	
		95_bi,	85_bi,	110_bi,	95_bi,	79_bi,		// Vanilluxe	
		60_bi,	50_bi,	40_bi,	50_bi,	75_bi,		// Deerling	
		100_bi,	70_bi,	60_bi,	70_bi,	95_bi,		// Sawsbuck	
		75_bi,	60_bi,	75_bi,	60_bi,	103_bi,		// Emolga	
		75_bi,	45_bi,	40_bi,	45_bi,	60_bi,		// Karrablast	
		135_bi,	105_bi,	60_bi,	105_bi,	20_bi,		// Escavalier	
		55_bi,	45_bi,	55_bi,	55_bi,	15_bi,		// Foongus	
		85_bi,	70_bi,	85_bi,	80_bi,	30_bi,		// Amoonguss	
		40_bi,	50_bi,	65_bi,	85_bi,	40_bi,		// Frillish	
		60_bi,	70_bi,	85_bi,	105_bi,	60_bi,		// Jellicent	
		75_bi,	80_bi,	40_bi,	45_bi,	65_bi,		// Alomomola	
		47_bi,	50_bi,	57_bi,	50_bi,	65_bi,		// Joltik	
		77_bi,	60_bi,	97_bi,	60_bi,	108_bi,		// Galvantula	
		50_bi,	91_bi,	24_bi,	86_bi,	10_bi,		// Ferroseed	
		94_bi,	131_bi,	54_bi,	116_bi,	20_bi,		// Ferrothorn	
		55_bi,	70_bi,	45_bi,	60_bi,	30_bi,		// Klink	
		80_bi,	95_bi,	70_bi,	85_bi,	50_bi,		// Klang	
		100_bi,	115_bi,	70_bi,	85_bi,	90_bi,		// Klinklang	
		55_bi,	40_bi,	45_bi,	40_bi,	60_bi,		// Tynamo	
		85_bi,	70_bi,	75_bi,	70_bi,	40_bi,		// Eelektrik	
		115_bi,	80_bi,	105_bi,	80_bi,	50_bi,		// Eelektross	
		55_bi,	55_bi,	85_bi,	55_bi,	30_bi,		// Elgyem	
		75_bi,	75_bi,	125_bi,	95_bi,	40_bi,		// Beheeyem	
		30_bi,	55_bi,	65_bi,	55_bi,	20_bi,		// Litwick	
		40_bi,	60_bi,	95_bi,	60_bi,	55_bi,		// Lampent	
		55_bi,	90_bi,	145_bi,	90_bi,	80_bi,		// Chandelure	
		87_bi,	60_bi,	30_bi,	40_bi,	57_bi,		// Axew	
		117_bi,	70_bi,	40_bi,	50_bi,	67_bi,		// Fraxure	
		147_bi,	90_bi,	60_bi,	70_bi,	97_bi,		// Haxorus	
		70_bi,	40_bi,	60_bi,	40_bi,	40_bi,		// Cubchoo	
		110_bi,	80_bi,	70_bi,	80_bi,	50_bi,		// Beartic	
		50_bi,	30_bi,	95_bi,	135_bi,	105_bi,		// Cryogonal	
		40_bi,	85_bi,	40_bi,	65_bi,	25_bi,		// Shelmet	
		70_bi,	40_bi,	100_bi,	60_bi,	145_bi,		// Accelgor	
		66_bi,	84_bi,	81_bi,	99_bi,	32_bi,		// Stunfisk	
		85_bi,	50_bi,	55_bi,	50_bi,	65_bi,		// Mienfoo	
		125_bi,	60_bi,	95_bi,	60_bi,	105_bi,		// Mienshao	
		120_bi,	90_bi,	60_bi,	90_bi,	48_bi,		// Druddigon	
		74_bi,	50_bi,	35_bi,	50_bi,	35_bi,		// Golett	
		124_bi,	80_bi,	55_bi,	80_bi,	55_bi,		// Golurk	
		85_bi,	70_bi,	40_bi,	40_bi,	60_bi,		// Pawniard	
		125_bi,	100_bi,	60_bi,	70_bi,	70_bi,		// Bisharp	
		110_bi,	95_bi,	40_bi,	95_bi,	55_bi,		// Bouffalant	
		83_bi,	50_bi,	37_bi,	50_bi,	60_bi,		// Rufflet	
		123_bi,	75_bi,	57_bi,	75_bi,	80_bi,		// Braviary	
		55_bi,	75_bi,	45_bi,	65_bi,	60_bi,		// Vullaby	
		65_bi,	105_bi,	55_bi,	95_bi,	80_bi,		// Mandibuzz	
		97_bi,	66_bi,	105_bi,	66_bi,	65_bi,		// Heatmor	
		109_bi,	112_bi,	48_bi,	48_bi,	109_bi,		// Durant	
		65_bi,	50_bi,	45_bi,	50_bi,	38_bi,		// Deino	
		85_bi,	70_bi,	65_bi,	70_bi,	58_bi,		// Zweilous	
		105_bi,	90_bi,	125_bi,	90_bi,	98_bi,		// Hydreigon	
		85_bi,	55_bi,	50_bi,	55_bi,	60_bi,		// Larvesta	
		60_bi,	65_bi,	135_bi,	105_bi,	100_bi,		// Volcarona	
		90_bi,	129_bi,	90_bi,	72_bi,	108_bi,		// Cobalion	
		129_bi,	90_bi,	72_bi,	90_bi,	108_bi,		// Terrakion	
		90_bi,	72_bi,	90_bi,	129_bi,	108_bi,		// Virizion	
		115_bi,	70_bi,	125_bi,	80_bi,	111_bi,		// Tornadus-Incarnate	
		100_bi,	80_bi,	110_bi,	90_bi,	121_bi,		// Tornadus-Therian	
		115_bi,	70_bi,	125_bi,	80_bi,	111_bi,		// Thundurus-Incarnate	
		105_bi,	70_bi,	145_bi,	80_bi,	101_bi,		// Thundurus-Therian	
		120_bi,	100_bi,	150_bi,	120_bi,	90_bi,		// Reshiram	
		150_bi,	120_bi,	120_bi,	100_bi,	90_bi,		// Zekrom	
		125_bi,	90_bi,	115_bi,	80_bi,	101_bi,		// Landorus-Incarnate	
		145_bi,	90_bi,	105_bi,	80_bi,	91_bi,		// Landorus-Therian	
		130_bi,	90_bi,	130_bi,	90_bi,	95_bi,		// Kyurem	
		170_bi,	100_bi,	120_bi,	90_bi,	95_bi,		// Kyurem-Black	
		120_bi,	90_bi,	170_bi,	100_bi,	95_bi,		// Kyurem-White	
		72_bi,	90_bi,	129_bi,	90_bi,	108_bi,		// Keldeo	
		77_bi,	77_bi,	128_bi,	128_bi,	90_bi,		// Meloetta	
		// 128_bi,	90_bi,	77_bi,	77_bi,	128_bi,		// Meloetta (Pirouette form)
		120_bi,	95_bi,	120_bi,	95_bi,	99_bi			// Genesect 
	);
	return base_stat.at(species).at(stat);
}

}	// unnamed namespace
}	// namespace technicalmachine
