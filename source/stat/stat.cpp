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

Stat::base_type get_base_stat(Species name, Stat::Stats stat_name);

unsigned initial_generic_stat(Stat const & stat, Level level);

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

template<Stat::Stats stat>
Rational item_modifier(Pokemon const & pokemon);
template<>
Rational item_modifier<Stat::ATK>(Pokemon const & attacker) {
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
Rational item_modifier<Stat::SPA>(Pokemon const & attacker) {
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
Rational item_modifier<Stat::DEF>(Pokemon const & defender) {
	return (get_item(defender).name == Item::METAL_POWDER and is_boosted_by_metal_powder(defender)) ?
		Rational(3, 2) :
		Rational(1);
}
template<>
Rational item_modifier<Stat::SPD>(Pokemon const & defender) {
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
Rational item_modifier<Stat::SPE>(Pokemon const & pokemon) {
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

template<Stat::Stats stat>
class StatTraits;

template<>
class StatTraits<Stat::ATK> {
public:
	static constexpr bool is_physical = true;
	static constexpr Stat::Stats other = Stat::DEF;
};
template<>
class StatTraits<Stat::DEF> {
public:
	static constexpr bool is_physical = true;
	static constexpr Stat::Stats other = Stat::ATK;
};
template<>
class StatTraits<Stat::SPA> {
public:
	static constexpr bool is_physical = false;
};
template<>
class StatTraits<Stat::SPD> {
public:
	static constexpr bool is_physical = false;
};
template<>
class StatTraits<Stat::SPE> {
public:
	static constexpr bool is_physical = false;
};

template<Stat::Stats stat>
typename std::enable_if<StatTraits<stat>::is_physical, unsigned>::type
calculate_initial_stat(ActivePokemon const & pokemon) {
	constexpr auto other = StatTraits<stat>::other;
	return !pokemon.power_trick_is_active() ? initial_stat<stat>(pokemon) : initial_stat<other>(pokemon);
}
template<Stat::Stats stat>
typename std::enable_if<!StatTraits<stat>::is_physical, unsigned>::type
calculate_initial_stat(ActivePokemon const & pokemon) {
	return initial_stat<stat>(pokemon);
}

template<Stat::Stats stat>
void calculate_common_offensive_stat(ActivePokemon & pokemon, Weather const & weather) {
	auto attack = calculate_initial_stat<stat>(pokemon);
	attack *= pokemon.stage_modifier<stat>(pokemon.critical_hit());

	attack *= Ability::stat_modifier<stat>(pokemon, weather);
	attack *= item_modifier<stat>(pokemon);
	
	get_stat(pokemon, stat).stat = std::max(attack, 1u);
}

}	// unnamed namespace

Stat::Stat (Species name, Stats stat_name) :
	max (65535),
	base(get_base_stat(name, stat_name)),
	iv(31_bi),
	ev (0)
	{
}

void Stat::calculate_initial_hp(Level const level) {
	max = (base > 1) ? (initial_generic_stat(*this, level) + static_cast<unsigned>(level()) + 5) : 1;
	stat = max;
}

template<Stat::Stats stat>
unsigned initial_stat(Pokemon const & pokemon) {
	return initial_generic_stat(get_stat(pokemon, stat), get_level(pokemon)) * get_nature(pokemon).boost<stat>();
}
template<>
unsigned initial_stat<Stat::HP>(Pokemon const & pokemon) {
	Stat const & hp = get_stat(pokemon, Stat::HP);
	return (hp.base > 1) ? (initial_generic_stat(hp, get_level(pokemon)) + static_cast<unsigned>(get_level(pokemon)()) + 5) : 1;
}
template unsigned initial_stat<Stat::ATK>(Pokemon const & pokemon);
template unsigned initial_stat<Stat::SPA>(Pokemon const & pokemon);
template unsigned initial_stat<Stat::DEF>(Pokemon const & pokemon);
template unsigned initial_stat<Stat::SPD>(Pokemon const & pokemon);
template unsigned initial_stat<Stat::SPE>(Pokemon const & pokemon);

void calculate_attacking_stat (ActivePokemon & attacker, Weather const & weather) {
	if (is_physical(attacker.move()))
		calculate_attack(attacker, weather);
	else
		calculate_special_attack(attacker, weather);
}

void calculate_attack(ActivePokemon & attacker, Weather const & weather) {
	calculate_common_offensive_stat<Stat::ATK>(attacker, weather);
}

void calculate_special_attack (ActivePokemon & attacker, Weather const & weather) {
	calculate_common_offensive_stat<Stat::SPA>(attacker, weather);
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

void calculate_defending_stat (ActivePokemon const & attacker, ActivePokemon & defender, Weather const & weather) {
	if (is_physical(attacker.move())) {
		calculate_defense(defender, weather, attacker.critical_hit(), is_self_KO(attacker.move()));
	}
	else {
		calculate_special_defense(defender, weather, attacker.critical_hit());
	}
}

void calculate_defense (ActivePokemon & defender, Weather const & weather, bool ch, bool is_self_KO) {
	constexpr auto stat = Stat::DEF;
	auto defense = calculate_initial_stat<stat>(defender);

	defense *= defender.stage_modifier<stat>(ch);
	
	defense *= Ability::stat_modifier<stat>(defender, weather);
	defense *= item_modifier<stat>(defender);
	
	if (is_self_KO)
		defense /= 2;

	get_stat(defender, stat).stat = std::max(defense, 1u);
}

void calculate_special_defense (ActivePokemon & defender, Weather const & weather, bool ch) {
	constexpr auto stat = Stat::SPD;
	auto defense = calculate_initial_stat<stat>(defender);
	
	defense *= defender.stage_modifier<stat>(ch);

	defense *= Ability::stat_modifier<stat>(defender, weather);	
	defense *= item_modifier<stat>(defender);
	
	defense *= special_defense_sandstorm_boost(defender, weather);
	
	get_stat(defender, stat).stat = std::max(defense, 1u);
}

void calculate_speed (Team & team, Weather const & weather) {
	constexpr auto stat = Stat::SPE;
	auto & pokemon = team.pokemon();
	auto speed = calculate_initial_stat<stat>(pokemon);
	
	speed *= pokemon.stage_modifier<stat>();

	speed *= Ability::stat_modifier<stat>(pokemon, weather);
	speed *= item_modifier<stat>(pokemon);
	
	speed /= paralysis_speed_divisor (pokemon);
	
	speed *= tailwind_speed_multiplier (team);

	get_stat(pokemon, stat).stat = std::max(speed, 1u);
}

void order (Team & team1, Team & team2, Weather const & weather, Team* & faster, Team* & slower) {
	Priority const priority1(team1.pokemon().move());
	Priority const priority2(team2.pokemon().move());
	if (priority1 == priority2) {
		calculate_speed(team1, weather);
		calculate_speed(team2, weather);
		faster_pokemon (team1, team2, weather, faster, slower);
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

void faster_pokemon (Team & team1, Team & team2, Weather const & weather, Team* & faster, Team* & slower) {
	auto const speed1 = get_stat(team1.pokemon(), Stat::SPE).stat;
	auto const speed2 = get_stat(team2.pokemon(), Stat::SPE).stat;
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
	if (weather.trick_room())
		std::swap (faster, slower);
}

namespace {

unsigned initial_generic_stat(Stat const & stat, Level const level) {
	return (static_cast<unsigned>(2_bi * stat.base + stat.iv) + stat.ev.points()) * static_cast<unsigned>(level()) / 100 + 5;
}

Rational special_defense_sandstorm_boost(ActivePokemon const & defender, Weather const & weather) {
	return (is_type(defender, Type::Rock) and weather.sand()) ? Rational(3, 2) : Rational(1);
}

unsigned paralysis_speed_divisor (Pokemon const & pokemon) {
	return get_status(pokemon).lowers_speed(get_ability(pokemon)) ? 4 : 1;
}

unsigned tailwind_speed_multiplier (Team const & team) {
	return team.screens.tailwind() ? 2 : 1;
}

Stat::base_type get_base_stat(Species name, Stat::Stats stat) {
	static constexpr auto base_stat = bounded_integer::make_array<6>(
	
		// Generation 1
		45_bi,	49_bi,	49_bi,	65_bi,	65_bi,	45_bi,		// Bulbasaur
		60_bi,	62_bi,	63_bi,	80_bi,	80_bi,	60_bi,		// Ivysaur
		80_bi,	82_bi,	83_bi,	100_bi,	100_bi,	80_bi,		// Venusaur
		39_bi,	52_bi,	43_bi,	60_bi,	50_bi,	65_bi,		// Charmander
		58_bi,	64_bi,	58_bi,	80_bi,	65_bi,	80_bi,		// Charmeleon
		78_bi,	84_bi,	78_bi,	109_bi,	85_bi,	100_bi,		// Charizard
		44_bi,	48_bi,	65_bi,	50_bi,	64_bi,	43_bi,		// Squirtle
		59_bi,	63_bi,	80_bi,	65_bi,	80_bi,	58_bi,		// Wartortle
		79_bi,	83_bi,	100_bi,	85_bi,	105_bi,	78_bi,		// Blastoise
		45_bi,	30_bi,	35_bi,	20_bi,	20_bi,	45_bi,		// Caterpie
		50_bi,	20_bi,	55_bi,	25_bi,	25_bi,	30_bi,		// Metapod
		60_bi,	45_bi,	50_bi,	80_bi,	80_bi,	70_bi,		// Butterfree
		40_bi,	35_bi,	30_bi,	20_bi,	20_bi,	50_bi,		// Weedle
		45_bi,	25_bi,	50_bi,	25_bi,	25_bi,	35_bi,		// Kakuna
		65_bi,	80_bi,	40_bi,	45_bi,	80_bi,	75_bi,		// Beedrill
		40_bi,	45_bi,	40_bi,	35_bi,	35_bi,	56_bi,		// Pidgey
		63_bi,	60_bi,	55_bi,	50_bi,	50_bi,	71_bi,		// Pidgeotto
		83_bi,	80_bi,	75_bi,	70_bi,	70_bi,	91_bi,		// Pidgeot
		30_bi,	56_bi,	35_bi,	25_bi,	35_bi,	72_bi,		// Rattata
		55_bi,	81_bi,	60_bi,	50_bi,	70_bi,	97_bi,		// Raticate
		40_bi,	60_bi,	30_bi,	31_bi,	31_bi,	70_bi,		// Spearow
		65_bi,	90_bi,	65_bi,	61_bi,	61_bi,	100_bi,		// Fearow
		35_bi,	60_bi,	44_bi,	40_bi,	54_bi,	55_bi,		// Ekans
		60_bi,	85_bi,	69_bi,	65_bi,	79_bi,	80_bi,		// Arbok
		35_bi,	55_bi,	30_bi,	50_bi,	40_bi,	90_bi,		// Pikachu
		60_bi,	90_bi,	55_bi,	90_bi,	80_bi,	100_bi,		// Raichu
		50_bi,	75_bi,	85_bi,	20_bi,	30_bi,	40_bi,		// Sandshrew
		75_bi,	100_bi,	110_bi,	45_bi,	55_bi,	65_bi,		// Sandslash
		55_bi,	47_bi,	52_bi,	40_bi,	40_bi,	41_bi,		// Nidoran-F
		70_bi,	62_bi,	67_bi,	55_bi,	55_bi,	56_bi,		// Nidorina
		90_bi,	82_bi,	87_bi,	75_bi,	85_bi,	76_bi,		// Nidoqueen
		46_bi,	57_bi,	40_bi,	40_bi,	40_bi,	50_bi,		// Nidoran-M
		61_bi,	72_bi,	57_bi,	55_bi,	55_bi,	65_bi,		// Nidorino
		81_bi,	92_bi,	77_bi,	85_bi,	75_bi,	85_bi,		// Nidoking
		70_bi,	45_bi,	48_bi,	60_bi,	65_bi,	35_bi,		// Clefairy
		95_bi,	70_bi,	73_bi,	85_bi,	90_bi,	60_bi,		// Clefable
		38_bi,	41_bi,	40_bi,	50_bi,	65_bi,	65_bi,		// Vulpix
		73_bi,	76_bi,	75_bi,	81_bi,	100_bi,	100_bi,		// Ninetales
		115_bi,	45_bi,	20_bi,	45_bi,	25_bi,	20_bi,		// Jigglypuff
		140_bi,	70_bi,	45_bi,	75_bi,	50_bi,	45_bi,		// Wigglytuff
		40_bi,	45_bi,	35_bi,	30_bi,	40_bi,	55_bi,		// Zubat
		75_bi,	80_bi,	70_bi,	65_bi,	75_bi,	90_bi,		// Golbat
		45_bi,	50_bi,	55_bi,	75_bi,	65_bi,	30_bi,		// Oddish
		60_bi,	65_bi,	70_bi,	85_bi,	75_bi,	40_bi,		// Gloom
		75_bi,	80_bi,	85_bi,	100_bi,	90_bi,	50_bi,		// Vileplume
		35_bi,	70_bi,	55_bi,	45_bi,	55_bi,	25_bi,		// Paras
		60_bi,	95_bi,	80_bi,	60_bi,	80_bi,	30_bi,		// Parasect
		60_bi,	55_bi,	50_bi,	40_bi,	55_bi,	45_bi,		// Venonat
		70_bi,	65_bi,	60_bi,	90_bi,	75_bi,	90_bi,		// Venomoth
		10_bi,	55_bi,	25_bi,	35_bi,	45_bi,	95_bi,		// Diglett
		35_bi,	80_bi,	50_bi,	50_bi,	70_bi,	120_bi,		// Dugtrio
		40_bi,	45_bi,	35_bi,	40_bi,	40_bi,	90_bi,		// Meowth
		65_bi,	70_bi,	60_bi,	65_bi,	65_bi,	115_bi,		// Persian
		50_bi,	52_bi,	48_bi,	65_bi,	50_bi,	55_bi,		// Psyduck
		80_bi,	82_bi,	78_bi,	95_bi,	80_bi,	85_bi,		// Golduck
		40_bi,	80_bi,	35_bi,	35_bi,	45_bi,	70_bi,		// Mankey
		65_bi,	105_bi,	60_bi,	60_bi,	70_bi,	95_bi,		// Primeape
		55_bi,	70_bi,	45_bi,	70_bi,	50_bi,	60_bi,		// Growlithe
		90_bi,	110_bi,	80_bi,	100_bi,	80_bi,	95_bi,		// Arcanine
		40_bi,	50_bi,	40_bi,	40_bi,	40_bi,	90_bi,		// Poliwag
		65_bi,	65_bi,	65_bi,	50_bi,	50_bi,	90_bi,		// Poliwhirl
		90_bi,	85_bi,	95_bi,	70_bi,	90_bi,	70_bi,		// Poliwrath
		25_bi,	20_bi,	15_bi,	105_bi,	55_bi,	90_bi,		// Abra
		40_bi,	35_bi,	30_bi,	120_bi,	70_bi,	105_bi,		// Kadabra
		55_bi,	50_bi,	45_bi,	135_bi,	85_bi,	120_bi,		// Alakazam
		70_bi,	80_bi,	50_bi,	35_bi,	35_bi,	35_bi,		// Machop
		80_bi,	100_bi,	70_bi,	50_bi,	60_bi,	45_bi,		// Machoke
		90_bi,	130_bi,	80_bi,	65_bi,	85_bi,	55_bi,		// Machamp
		50_bi,	75_bi,	35_bi,	70_bi,	30_bi,	40_bi,		// Bellsprout
		65_bi,	90_bi,	50_bi,	85_bi,	45_bi,	55_bi,		// Weepinbell
		80_bi,	105_bi,	65_bi,	100_bi,	60_bi,	70_bi,		// Victreebel
		40_bi,	40_bi,	35_bi,	50_bi,	100_bi,	70_bi,		// Tentacool
		80_bi,	70_bi,	65_bi,	80_bi,	120_bi,	100_bi,		// Tentacruel
		40_bi,	80_bi,	100_bi,	30_bi,	30_bi,	20_bi,		// Geodude
		55_bi,	95_bi,	115_bi,	45_bi,	45_bi,	35_bi,		// Graveler
		80_bi,	110_bi,	130_bi,	55_bi,	65_bi,	45_bi,		// Golem
		50_bi,	85_bi,	55_bi,	65_bi,	65_bi,	90_bi,		// Ponyta
		65_bi,	100_bi,	70_bi,	80_bi,	80_bi,	105_bi,		// Rapidash
		90_bi,	65_bi,	65_bi,	40_bi,	40_bi,	15_bi,		// Slowpoke
		95_bi,	75_bi,	110_bi,	100_bi,	80_bi,	30_bi,		// Slowbro
		25_bi,	35_bi,	70_bi,	95_bi,	55_bi,	45_bi,		// Magnemite
		50_bi,	60_bi,	95_bi,	120_bi,	70_bi,	70_bi,		// Magneton
		52_bi,	65_bi,	55_bi,	58_bi,	62_bi,	60_bi,		// Farfetch'd
		35_bi,	85_bi,	45_bi,	35_bi,	35_bi,	75_bi,		// Doduo
		60_bi,	110_bi,	70_bi,	60_bi,	60_bi,	100_bi,		// Dodrio
		65_bi,	45_bi,	55_bi,	45_bi,	70_bi,	45_bi,		// Seel
		90_bi,	70_bi,	80_bi,	70_bi,	95_bi,	70_bi,		// Dewgong
		80_bi,	80_bi,	50_bi,	40_bi,	50_bi,	25_bi,		// Grimer
		105_bi,	105_bi,	75_bi,	65_bi,	100_bi,	50_bi,		// Muk
		30_bi,	65_bi,	100_bi,	45_bi,	25_bi,	40_bi,		// Shellder
		50_bi,	95_bi,	180_bi,	85_bi,	45_bi,	70_bi,		// Cloyster
		30_bi,	35_bi,	30_bi,	100_bi,	35_bi,	80_bi,		// Gastly
		45_bi,	50_bi,	45_bi,	115_bi,	55_bi,	95_bi,		// Haunter
		60_bi,	65_bi,	60_bi,	130_bi,	75_bi,	110_bi,		// Gengar
		35_bi,	45_bi,	160_bi,	30_bi,	45_bi,	70_bi,		// Onix
		60_bi,	48_bi,	45_bi,	43_bi,	90_bi,	42_bi,		// Drowzee
		85_bi,	73_bi,	70_bi,	73_bi,	115_bi,	67_bi,		// Hypno
		30_bi,	105_bi,	90_bi,	25_bi,	25_bi,	50_bi,		// Krabby
		55_bi,	130_bi,	115_bi,	50_bi,	50_bi,	75_bi,		// Kingler
		40_bi,	30_bi,	50_bi,	55_bi,	55_bi,	100_bi,		// Voltorb
		60_bi,	50_bi,	70_bi,	80_bi,	80_bi,	140_bi,		// Electrode
		60_bi,	40_bi,	80_bi,	60_bi,	45_bi,	40_bi,		// Exeggcute
		95_bi,	95_bi,	85_bi,	125_bi,	65_bi,	55_bi,		// Exeggutor
		50_bi,	50_bi,	95_bi,	40_bi,	50_bi,	35_bi,		// Cubone
		60_bi,	80_bi,	110_bi,	50_bi,	80_bi,	45_bi,		// Marowak
		50_bi,	120_bi,	53_bi,	35_bi,	110_bi,	87_bi,		// Hitmonlee
		50_bi,	105_bi,	79_bi,	35_bi,	110_bi,	76_bi,		// Hitmonchan
		90_bi,	55_bi,	75_bi,	60_bi,	75_bi,	30_bi,		// Lickitung
		40_bi,	65_bi,	95_bi,	60_bi,	45_bi,	35_bi,		// Koffing
		65_bi,	90_bi,	120_bi,	85_bi,	70_bi,	60_bi,		// Weezing
		80_bi,	85_bi,	95_bi,	30_bi,	30_bi,	25_bi,		// Rhyhorn
		105_bi,	130_bi,	120_bi,	45_bi,	45_bi,	40_bi,		// Rhydon
		250_bi,	5_bi,	5_bi,	35_bi,	105_bi,	50_bi,		// Chansey
		65_bi,	55_bi,	115_bi,	100_bi,	40_bi,	60_bi,		// Tangela
		105_bi,	95_bi,	80_bi,	40_bi,	80_bi,	90_bi,		// Kangaskhan
		30_bi,	40_bi,	70_bi,	70_bi,	25_bi,	60_bi,		// Horsea
		55_bi,	65_bi,	95_bi,	95_bi,	45_bi,	85_bi,		// Seadra
		45_bi,	67_bi,	60_bi,	35_bi,	50_bi,	63_bi,		// Goldeen
		80_bi,	92_bi,	65_bi,	65_bi,	80_bi,	68_bi,		// Seaking
		30_bi,	45_bi,	55_bi,	70_bi,	55_bi,	85_bi,		// Staryu
		60_bi,	75_bi,	85_bi,	100_bi,	85_bi,	115_bi,		// Starmie
		40_bi,	45_bi,	65_bi,	100_bi,	120_bi,	90_bi,		// Mr. Mime
		70_bi,	110_bi,	80_bi,	55_bi,	80_bi,	105_bi,		// Scyther
		65_bi,	50_bi,	35_bi,	115_bi,	95_bi,	95_bi,		// Jynx
		65_bi,	83_bi,	57_bi,	95_bi,	85_bi,	105_bi,		// Electabuzz
		65_bi,	95_bi,	57_bi,	100_bi,	85_bi,	93_bi,		// Magmar
		65_bi,	125_bi,	100_bi,	55_bi,	70_bi,	85_bi,		// Pinsir
		75_bi,	100_bi,	95_bi,	40_bi,	70_bi,	110_bi,		// Tauros
		20_bi,	10_bi,	55_bi,	15_bi,	20_bi,	80_bi,		// Magikarp
		95_bi,	125_bi,	79_bi,	60_bi,	100_bi,	81_bi,		// Gyarados
		130_bi,	85_bi,	80_bi,	85_bi,	95_bi,	60_bi,		// Lapras
		48_bi,	48_bi,	48_bi,	48_bi,	48_bi,	48_bi,		// Ditto
		55_bi,	55_bi,	50_bi,	45_bi,	65_bi,	55_bi,		// Eevee
		130_bi,	65_bi,	60_bi,	110_bi,	95_bi,	65_bi,		// Vaporeon
		65_bi,	65_bi,	60_bi,	110_bi,	95_bi,	130_bi,		// Jolteon
		65_bi,	130_bi,	60_bi,	95_bi,	110_bi,	65_bi,		// Flareon
		65_bi,	60_bi,	70_bi,	85_bi,	75_bi,	40_bi,		// Porygon
		35_bi,	40_bi,	100_bi,	90_bi,	55_bi,	35_bi,		// Omanyte
		70_bi,	60_bi,	125_bi,	115_bi,	70_bi,	55_bi,		// Omastar
		30_bi,	80_bi,	90_bi,	55_bi,	45_bi,	55_bi,		// Kabuto
		60_bi,	115_bi,	105_bi,	65_bi,	70_bi,	80_bi,		// Kabutops
		80_bi,	105_bi,	65_bi,	60_bi,	75_bi,	130_bi,		// Aerodactyl
		160_bi,	110_bi,	65_bi,	65_bi,	110_bi,	30_bi,		// Snorlax
		90_bi,	85_bi,	100_bi,	95_bi,	125_bi,	85_bi,		// Articuno
		90_bi,	90_bi,	85_bi,	125_bi,	90_bi,	100_bi,		// Zapdos
		90_bi,	100_bi,	90_bi,	125_bi,	85_bi,	90_bi,		// Moltres
		41_bi,	64_bi,	45_bi,	50_bi,	50_bi,	50_bi,		// Dratini
		61_bi,	84_bi,	65_bi,	70_bi,	70_bi,	70_bi,		// Dragonair
		91_bi,	134_bi,	95_bi,	100_bi,	100_bi,	80_bi,		// Dragonite
		106_bi,	110_bi,	90_bi,	154_bi,	90_bi,	130_bi,		// Mewtwo
		100_bi,	100_bi,	100_bi,	100_bi,	100_bi,	100_bi,		// Mew
		
		// Generation 2
		45_bi,	49_bi,	65_bi,	49_bi,	65_bi,	45_bi,		// Chikorita
		60_bi,	62_bi,	80_bi,	63_bi,	80_bi,	60_bi,		// Bayleef
		80_bi,	82_bi,	100_bi,	83_bi,	100_bi,	80_bi,		// Meganium
		39_bi,	52_bi,	43_bi,	60_bi,	50_bi,	65_bi,		// Cyndaquil
		58_bi,	64_bi,	58_bi,	80_bi,	65_bi,	80_bi,		// Quilava
		78_bi,	84_bi,	78_bi,	109_bi,	85_bi,	100_bi,		// Typhlosion
		50_bi,	65_bi,	64_bi,	44_bi,	48_bi,	43_bi,		// Totodile
		65_bi,	80_bi,	80_bi,	59_bi,	63_bi,	58_bi,		// Croconaw
		85_bi,	105_bi,	100_bi,	79_bi,	83_bi,	78_bi,		// Feraligatr
		35_bi,	46_bi,	34_bi,	35_bi,	45_bi,	20_bi,		// Sentret
		85_bi,	76_bi,	64_bi,	45_bi,	55_bi,	90_bi,		// Furret
		60_bi,	30_bi,	30_bi,	36_bi,	56_bi,	50_bi,		// Hoothoot
		100_bi,	50_bi,	50_bi,	76_bi,	96_bi,	70_bi,		// Noctowl
		40_bi,	20_bi,	30_bi,	40_bi,	80_bi,	55_bi,		// Ledyba
		55_bi,	35_bi,	50_bi,	55_bi,	110_bi,	85_bi,		// Ledian
		40_bi,	60_bi,	40_bi,	40_bi,	40_bi,	30_bi,		// Spinarak
		70_bi,	90_bi,	70_bi,	60_bi,	60_bi,	40_bi,		// Ariados
		85_bi,	90_bi,	80_bi,	70_bi,	80_bi,	130_bi,		// Crobat
		75_bi,	38_bi,	38_bi,	56_bi,	56_bi,	67_bi,		// Chinchou
		125_bi,	58_bi,	58_bi,	76_bi,	76_bi,	67_bi,		// Lanturn
		20_bi,	40_bi,	15_bi,	35_bi,	35_bi,	60_bi,		// Pichu
		50_bi,	25_bi,	28_bi,	45_bi,	55_bi,	15_bi,		// Cleffa
		90_bi,	30_bi,	15_bi,	40_bi,	20_bi,	15_bi,		// Igglybuff
		35_bi,	20_bi,	65_bi,	40_bi,	65_bi,	20_bi,		// Togepi
		55_bi,	40_bi,	85_bi,	80_bi,	105_bi,	40_bi,		// Togetic
		40_bi,	50_bi,	45_bi,	70_bi,	45_bi,	70_bi,		// Natu
		65_bi,	75_bi,	70_bi,	95_bi,	70_bi,	95_bi,		// Xatu
		55_bi,	40_bi,	40_bi,	65_bi,	45_bi,	35_bi,		// Mareep
		70_bi,	55_bi,	55_bi,	80_bi,	60_bi,	45_bi,		// Flaaffy
		90_bi,	75_bi,	75_bi,	115_bi,	90_bi,	55_bi,		// Ampharos
		75_bi,	80_bi,	85_bi,	90_bi,	100_bi,	50_bi,		// Bellossom
		70_bi,	20_bi,	50_bi,	20_bi,	50_bi,	40_bi,		// Marill
		100_bi,	50_bi,	80_bi,	50_bi,	80_bi,	50_bi,		// Azumarill
		70_bi,	100_bi,	115_bi,	30_bi,	65_bi,	30_bi,		// Sudowoodo
		90_bi,	75_bi,	75_bi,	90_bi,	100_bi,	70_bi,		// Politoed
		35_bi,	35_bi,	40_bi,	35_bi,	55_bi,	50_bi,		// Hoppip
		55_bi,	45_bi,	50_bi,	45_bi,	65_bi,	80_bi,		// Skiploom
		75_bi,	55_bi,	70_bi,	55_bi,	85_bi,	110_bi,		// Jumpluff
		55_bi,	70_bi,	55_bi,	40_bi,	55_bi,	85_bi,		// Aipom
		30_bi,	30_bi,	30_bi,	30_bi,	30_bi,	30_bi,		// Sunkern
		75_bi,	75_bi,	55_bi,	105_bi,	85_bi,	30_bi,		// Sunflora
		65_bi,	65_bi,	45_bi,	75_bi,	45_bi,	95_bi,		// Yanma
		55_bi,	45_bi,	45_bi,	25_bi,	25_bi,	15_bi,		// Wooper
		95_bi,	85_bi,	85_bi,	65_bi,	65_bi,	35_bi,		// Quagsire
		65_bi,	65_bi,	60_bi,	130_bi,	95_bi,	110_bi,		// Espeon
		95_bi,	65_bi,	110_bi,	60_bi,	130_bi,	65_bi,		// Umbreon
		60_bi,	85_bi,	42_bi,	85_bi,	42_bi,	91_bi,		// Murkrow
		95_bi,	75_bi,	80_bi,	100_bi,	110_bi,	30_bi,		// Slowking
		60_bi,	60_bi,	60_bi,	85_bi,	85_bi,	85_bi,		// Misdreavus
		48_bi,	72_bi,	48_bi,	72_bi,	48_bi,	48_bi,		// Unown
		190_bi,	33_bi,	58_bi,	33_bi,	58_bi,	33_bi,		// Wobbuffet
		70_bi,	80_bi,	65_bi,	90_bi,	65_bi,	85_bi,		// Girafarig
		50_bi,	65_bi,	90_bi,	35_bi,	35_bi,	15_bi,		// Pineco
		75_bi,	90_bi,	140_bi,	60_bi,	60_bi,	40_bi,		// Forretress
		100_bi,	70_bi,	70_bi,	65_bi,	65_bi,	45_bi,		// Dunsparce
		65_bi,	75_bi,	105_bi,	35_bi,	65_bi,	85_bi,		// Gligar
		75_bi,	85_bi,	200_bi,	55_bi,	65_bi,	30_bi,		// Steelix
		60_bi,	80_bi,	50_bi,	40_bi,	40_bi,	30_bi,		// Snubbull
		90_bi,	120_bi,	75_bi,	60_bi,	60_bi,	45_bi,		// Granbull
		65_bi,	95_bi,	75_bi,	55_bi,	55_bi,	85_bi,		// Qwilfish
		70_bi,	130_bi,	100_bi,	55_bi,	80_bi,	65_bi,		// Scizor
		20_bi,	10_bi,	230_bi,	10_bi,	230_bi,	5_bi,		// Shuckle
		80_bi,	125_bi,	75_bi,	40_bi,	95_bi,	85_bi,		// Heracross
		55_bi,	95_bi,	55_bi,	35_bi,	75_bi,	115_bi,		// Sneasel
		60_bi,	80_bi,	50_bi,	50_bi,	50_bi,	40_bi,		// Teddiursa
		90_bi,	130_bi,	75_bi,	75_bi,	75_bi,	55_bi,		// Ursaring
		40_bi,	40_bi,	40_bi,	70_bi,	40_bi,	20_bi,		// Slugma
		50_bi,	50_bi,	120_bi,	80_bi,	80_bi,	30_bi,		// Magcargo
		50_bi,	50_bi,	40_bi,	30_bi,	30_bi,	50_bi,		// Swinub
		100_bi,	100_bi,	80_bi,	60_bi,	60_bi,	50_bi,		// Piloswine
		55_bi,	55_bi,	85_bi,	65_bi,	85_bi,	35_bi,		// Corsola
		35_bi,	65_bi,	35_bi,	65_bi,	35_bi,	65_bi,		// Remoraid
		75_bi,	105_bi,	75_bi,	105_bi,	75_bi,	45_bi,		// Octillery
		45_bi,	55_bi,	45_bi,	65_bi,	45_bi,	75_bi,		// Delibird
		65_bi,	40_bi,	70_bi,	80_bi,	140_bi,	70_bi,		// Mantine
		65_bi,	80_bi,	140_bi,	40_bi,	70_bi,	70_bi,		// Skarmory
		45_bi,	60_bi,	30_bi,	80_bi,	50_bi,	65_bi,		// Houndour
		75_bi,	90_bi,	50_bi,	110_bi,	80_bi,	95_bi,		// Houndoom
		75_bi,	95_bi,	95_bi,	95_bi,	95_bi,	85_bi,		// Kingdra
		90_bi,	60_bi,	60_bi,	40_bi,	40_bi,	40_bi,		// Phanpy
		90_bi,	120_bi,	120_bi,	60_bi,	60_bi,	50_bi,		// Donphan
		85_bi,	80_bi,	90_bi,	105_bi,	95_bi,	60_bi,		// Porygon2
		73_bi,	95_bi,	62_bi,	85_bi,	65_bi,	85_bi,		// Stantler
		55_bi,	20_bi,	35_bi,	20_bi,	45_bi,	75_bi,		// Smeargle
		35_bi,	35_bi,	35_bi,	35_bi,	35_bi,	35_bi,		// Tyrogue
		50_bi,	95_bi,	95_bi,	35_bi,	110_bi,	70_bi,		// Hitmontop
		45_bi,	30_bi,	15_bi,	85_bi,	65_bi,	65_bi,		// Smoochum
		45_bi,	63_bi,	37_bi,	65_bi,	55_bi,	95_bi,		// Elekid
		45_bi,	75_bi,	37_bi,	70_bi,	55_bi,	83_bi,		// Magby
		95_bi,	80_bi,	105_bi,	40_bi,	70_bi,	100_bi,		// Miltank
		255_bi,	10_bi,	10_bi,	75_bi,	135_bi,	55_bi,		// Blissey
		90_bi,	85_bi,	75_bi,	115_bi,	100_bi,	115_bi,		// Raikou
		115_bi,	115_bi,	85_bi,	90_bi,	75_bi,	100_bi,		// Entei
		100_bi,	75_bi,	115_bi,	90_bi,	115_bi,	85_bi,		// Suicune
		50_bi,	64_bi,	50_bi,	45_bi,	50_bi,	41_bi,		// Larvitar
		70_bi,	84_bi,	70_bi,	65_bi,	70_bi,	51_bi,		// Pupitar
		100_bi,	134_bi,	110_bi,	95_bi,	100_bi,	61_bi,		// Tyranitar
		106_bi,	90_bi,	130_bi,	90_bi,	154_bi,	110_bi,		// Lugia
		106_bi,	130_bi,	90_bi,	110_bi,	154_bi,	90_bi,		// Ho-Oh
		100_bi,	100_bi,	100_bi,	100_bi,	100_bi,	100_bi,		// Celebi
		
		// Generation 3
		40_bi,	45_bi,	35_bi,	65_bi,	55_bi,	70_bi,		// Treecko
		50_bi,	65_bi,	45_bi,	85_bi,	65_bi,	95_bi,		// Grovyle
		70_bi,	85_bi,	65_bi,	105_bi,	85_bi,	120_bi,		// Sceptile
		45_bi,	60_bi,	40_bi,	70_bi,	50_bi,	45_bi,		// Torchic
		60_bi,	85_bi,	60_bi,	85_bi,	60_bi,	55_bi,		// Combusken
		80_bi,	120_bi,	70_bi,	110_bi,	70_bi,	80_bi,		// Blaziken
		50_bi,	70_bi,	50_bi,	50_bi,	50_bi,	40_bi,		// Mudkip
		70_bi,	85_bi,	70_bi,	60_bi,	70_bi,	50_bi,		// Marshtomp
		100_bi,	110_bi,	90_bi,	85_bi,	90_bi,	60_bi,		// Swampert
		35_bi,	55_bi,	35_bi,	30_bi,	30_bi,	35_bi,		// Poochyena
		70_bi,	90_bi,	70_bi,	60_bi,	60_bi,	70_bi,		// Mightyena
		38_bi,	30_bi,	41_bi,	30_bi,	41_bi,	60_bi,		// Zigzagoon
		78_bi,	70_bi,	61_bi,	50_bi,	61_bi,	100_bi,		// Linoone
		45_bi,	45_bi,	35_bi,	20_bi,	30_bi,	20_bi,		// Wurmple
		50_bi,	35_bi,	55_bi,	25_bi,	25_bi,	15_bi,		// Silcoon
		60_bi,	70_bi,	50_bi,	90_bi,	50_bi,	65_bi,		// Beautifly
		50_bi,	35_bi,	55_bi,	25_bi,	25_bi,	15_bi,		// Cascoon
		60_bi,	50_bi,	70_bi,	50_bi,	90_bi,	65_bi,		// Dustox
		40_bi,	30_bi,	30_bi,	40_bi,	50_bi,	30_bi,		// Lotad
		60_bi,	50_bi,	50_bi,	60_bi,	70_bi,	50_bi,		// Lombre
		80_bi,	70_bi,	70_bi,	90_bi,	100_bi,	70_bi,		// Ludicolo
		40_bi,	40_bi,	50_bi,	30_bi,	30_bi,	30_bi,		// Seedot
		70_bi,	70_bi,	40_bi,	60_bi,	40_bi,	60_bi,		// Nuzleaf
		90_bi,	100_bi,	60_bi,	90_bi,	60_bi,	80_bi,		// Shiftry
		40_bi,	55_bi,	30_bi,	30_bi,	30_bi,	85_bi,		// Taillow
		60_bi,	85_bi,	60_bi,	50_bi,	50_bi,	125_bi,		// Swellow
		40_bi,	30_bi,	30_bi,	55_bi,	30_bi,	85_bi,		// Wingull
		60_bi,	50_bi,	100_bi,	85_bi,	70_bi,	65_bi,		// Pelipper
		28_bi,	25_bi,	25_bi,	45_bi,	35_bi,	40_bi,		// Ralts
		38_bi,	35_bi,	35_bi,	65_bi,	55_bi,	50_bi,		// Kirlia
		68_bi,	65_bi,	65_bi,	125_bi,	115_bi,	80_bi,		// Gardevoir
		40_bi,	30_bi,	32_bi,	50_bi,	52_bi,	65_bi,		// Surskit
		70_bi,	60_bi,	62_bi,	80_bi,	82_bi,	60_bi,		// Masquerain
		60_bi,	40_bi,	60_bi,	40_bi,	60_bi,	35_bi,		// Shroomish
		60_bi,	130_bi,	80_bi,	60_bi,	60_bi,	70_bi,		// Breloom
		60_bi,	60_bi,	60_bi,	35_bi,	35_bi,	30_bi,		// Slakoth
		80_bi,	80_bi,	80_bi,	55_bi,	55_bi,	90_bi,		// Vigoroth
		150_bi,	160_bi,	100_bi,	95_bi,	65_bi,	100_bi,		// Slaking
		31_bi,	45_bi,	90_bi,	30_bi,	30_bi,	40_bi,		// Nincada
		61_bi,	90_bi,	45_bi,	50_bi,	50_bi,	160_bi,		// Ninjask
		1_bi,	90_bi,	45_bi,	30_bi,	30_bi,	40_bi,		// Shedinja
		64_bi,	51_bi,	23_bi,	51_bi,	23_bi,	28_bi,		// Whismur
		84_bi,	71_bi,	43_bi,	71_bi,	43_bi,	48_bi,		// Loudred
		104_bi,	91_bi,	63_bi,	91_bi,	63_bi,	68_bi,		// Exploud
		72_bi,	60_bi,	30_bi,	20_bi,	30_bi,	25_bi,		// Makuhita
		144_bi,	120_bi,	60_bi,	40_bi,	60_bi,	50_bi,		// Hariyama
		50_bi,	20_bi,	40_bi,	20_bi,	40_bi,	20_bi,		// Azurill
		30_bi,	45_bi,	135_bi,	45_bi,	90_bi,	30_bi,		// Nosepass
		50_bi,	45_bi,	45_bi,	35_bi,	35_bi,	50_bi,		// Skitty
		70_bi,	65_bi,	65_bi,	55_bi,	55_bi,	70_bi,		// Delcatty
		50_bi,	75_bi,	75_bi,	65_bi,	65_bi,	50_bi,		// Sableye
		50_bi,	85_bi,	85_bi,	55_bi,	55_bi,	50_bi,		// Mawile
		50_bi,	70_bi,	100_bi,	40_bi,	40_bi,	30_bi,		// Aron
		60_bi,	90_bi,	140_bi,	50_bi,	50_bi,	40_bi,		// Lairon
		70_bi,	110_bi,	180_bi,	60_bi,	60_bi,	50_bi,		// Aggron
		30_bi,	40_bi,	55_bi,	40_bi,	55_bi,	60_bi,		// Meditite
		60_bi,	60_bi,	75_bi,	60_bi,	75_bi,	80_bi,		// Medicham
		40_bi,	45_bi,	40_bi,	65_bi,	40_bi,	65_bi,		// Electrike
		70_bi,	75_bi,	60_bi,	105_bi,	60_bi,	105_bi,		// Manectric
		60_bi,	50_bi,	40_bi,	85_bi,	75_bi,	95_bi,		// Plusle
		60_bi,	40_bi,	50_bi,	75_bi,	85_bi,	95_bi,		// Minun
		65_bi,	73_bi,	55_bi,	47_bi,	75_bi,	85_bi,		// Volbeat
		65_bi,	47_bi,	55_bi,	73_bi,	75_bi,	85_bi,		// Illumise
		50_bi,	60_bi,	45_bi,	100_bi,	80_bi,	65_bi,		// Roselia
		70_bi,	43_bi,	53_bi,	43_bi,	53_bi,	40_bi,		// Gulpin
		100_bi,	73_bi,	83_bi,	73_bi,	83_bi,	55_bi,		// Swalot
		45_bi,	90_bi,	20_bi,	65_bi,	20_bi,	65_bi,		// Carvanha
		70_bi,	120_bi,	40_bi,	95_bi,	40_bi,	95_bi,		// Sharpedo
		130_bi,	70_bi,	35_bi,	70_bi,	35_bi,	60_bi,		// Wailmer
		170_bi,	90_bi,	45_bi,	90_bi,	45_bi,	60_bi,		// Wailord
		60_bi,	60_bi,	40_bi,	65_bi,	45_bi,	35_bi,		// Numel
		70_bi,	100_bi,	70_bi,	105_bi,	75_bi,	40_bi,		// Camerupt
		70_bi,	85_bi,	140_bi,	85_bi,	70_bi,	20_bi,		// Torkoal
		60_bi,	25_bi,	35_bi,	70_bi,	80_bi,	60_bi,		// Spoink
		80_bi,	45_bi,	65_bi,	90_bi,	110_bi,	80_bi,		// Grumpig
		60_bi,	60_bi,	60_bi,	60_bi,	60_bi,	60_bi,		// Spinda
		45_bi,	100_bi,	45_bi,	45_bi,	45_bi,	10_bi,		// Trapinch
		50_bi,	70_bi,	50_bi,	50_bi,	50_bi,	70_bi,		// Vibrava
		80_bi,	100_bi,	80_bi,	80_bi,	80_bi,	100_bi,		// Flygon
		50_bi,	85_bi,	40_bi,	85_bi,	40_bi,	35_bi,		// Cacnea
		70_bi,	115_bi,	60_bi,	115_bi,	60_bi,	55_bi,		// Cacturne
		45_bi,	40_bi,	60_bi,	40_bi,	75_bi,	50_bi,		// Swablu
		75_bi,	70_bi,	90_bi,	70_bi,	105_bi,	80_bi,		// Altaria
		73_bi,	115_bi,	60_bi,	60_bi,	60_bi,	90_bi,		// Zangoose
		73_bi,	100_bi,	60_bi,	100_bi,	60_bi,	65_bi,		// Seviper
		70_bi,	55_bi,	65_bi,	95_bi,	85_bi,	70_bi,		// Lunatone
		70_bi,	95_bi,	85_bi,	55_bi,	65_bi,	70_bi,		// Solrock
		50_bi,	48_bi,	43_bi,	46_bi,	41_bi,	60_bi,		// Barboach
		110_bi,	78_bi,	73_bi,	76_bi,	71_bi,	60_bi,		// Whiscash
		43_bi,	80_bi,	65_bi,	50_bi,	35_bi,	35_bi,		// Corphish
		63_bi,	120_bi,	85_bi,	90_bi,	55_bi,	55_bi,		// Crawdaunt
		40_bi,	40_bi,	55_bi,	40_bi,	70_bi,	55_bi,		// Baltoy
		60_bi,	70_bi,	105_bi,	70_bi,	120_bi,	75_bi,		// Claydol
		66_bi,	41_bi,	77_bi,	61_bi,	87_bi,	23_bi,		// Lileep
		86_bi,	81_bi,	97_bi,	81_bi,	107_bi,	43_bi,		// Cradily
		45_bi,	95_bi,	50_bi,	40_bi,	50_bi,	75_bi,		// Anorith
		75_bi,	125_bi,	100_bi,	70_bi,	80_bi,	45_bi,		// Armaldo
		20_bi,	15_bi,	20_bi,	10_bi,	55_bi,	80_bi,		// Feebas
		95_bi,	60_bi,	79_bi,	100_bi,	125_bi,	81_bi,		// Milotic
		70_bi,	70_bi,	70_bi,	70_bi,	70_bi,	70_bi,		// Castform
		60_bi,	90_bi,	70_bi,	60_bi,	120_bi,	40_bi,		// Kecleon
		44_bi,	75_bi,	35_bi,	63_bi,	33_bi,	45_bi,		// Shuppet
		64_bi,	115_bi,	65_bi,	83_bi,	63_bi,	65_bi,		// Banette
		20_bi,	40_bi,	90_bi,	30_bi,	90_bi,	25_bi,		// Duskull
		40_bi,	70_bi,	130_bi,	60_bi,	130_bi,	25_bi,		// Dusclops
		99_bi,	68_bi,	83_bi,	72_bi,	87_bi,	51_bi,		// Tropius
		65_bi,	50_bi,	70_bi,	95_bi,	80_bi,	65_bi,		// Chimecho
		65_bi,	130_bi,	60_bi,	75_bi,	60_bi,	75_bi,		// Absol
		95_bi,	23_bi,	48_bi,	23_bi,	48_bi,	23_bi,		// Wynaut
		50_bi,	50_bi,	50_bi,	50_bi,	50_bi,	50_bi,		// Snorunt
		80_bi,	80_bi,	80_bi,	80_bi,	80_bi,	80_bi,		// Glalie
		70_bi,	40_bi,	50_bi,	55_bi,	50_bi,	25_bi,		// Spheal
		90_bi,	60_bi,	70_bi,	75_bi,	70_bi,	45_bi,		// Sealeo
		110_bi,	80_bi,	90_bi,	95_bi,	90_bi,	65_bi,		// Walrein
		35_bi,	64_bi,	85_bi,	74_bi,	55_bi,	32_bi,		// Clamperl
		55_bi,	104_bi,	105_bi,	94_bi,	75_bi,	52_bi,		// Huntail
		55_bi,	84_bi,	105_bi,	114_bi,	75_bi,	52_bi,		// Gorebyss
		100_bi,	90_bi,	130_bi,	45_bi,	65_bi,	55_bi,		// Relicanth
		43_bi,	30_bi,	55_bi,	40_bi,	65_bi,	97_bi,		// Luvdisc
		45_bi,	75_bi,	60_bi,	40_bi,	30_bi,	50_bi,		// Bagon
		65_bi,	95_bi,	100_bi,	60_bi,	50_bi,	50_bi,		// Shelgon
		95_bi,	135_bi,	80_bi,	110_bi,	80_bi,	100_bi,		// Salamence
		40_bi,	55_bi,	80_bi,	35_bi,	60_bi,	30_bi,		// Beldum
		60_bi,	75_bi,	100_bi,	55_bi,	80_bi,	50_bi,		// Metang
		80_bi,	135_bi,	130_bi,	95_bi,	90_bi,	70_bi,		// Metagross
		80_bi,	100_bi,	200_bi,	50_bi,	100_bi,	50_bi,		// Regirock
		80_bi,	50_bi,	100_bi,	100_bi,	200_bi,	50_bi,		// Regice
		80_bi,	75_bi,	150_bi,	75_bi,	150_bi,	50_bi,		// Registeel
		80_bi,	80_bi,	90_bi,	110_bi,	130_bi,	110_bi,		// Latias
		80_bi,	90_bi,	80_bi,	130_bi,	110_bi,	110_bi,		// Latios
		100_bi,	100_bi,	90_bi,	150_bi,	140_bi,	90_bi,		// Kyogre
		100_bi,	150_bi,	140_bi,	100_bi,	90_bi,	90_bi,		// Groudon
		105_bi,	150_bi,	90_bi,	150_bi,	90_bi,	95_bi,		// Rayquaza
		100_bi,	100_bi,	100_bi,	100_bi,	100_bi,	100_bi,		// Jirachi
		50_bi,	150_bi,	50_bi,	150_bi,	50_bi,	150_bi,		// Deoxys-Mediocre
		50_bi,	180_bi,	20_bi,	180_bi,	20_bi,	150_bi,		// Deoxys-Attack
		50_bi,	70_bi,	160_bi,	70_bi,	160_bi,	90_bi,		// Deoxys-Defense
		50_bi,	95_bi,	90_bi,	95_bi,	90_bi,	180_bi,		// Deoxys-Speed
		
		// Generation 4
		55_bi,	68_bi,	64_bi,	45_bi,	55_bi,	31_bi,		// Turtwig
		75_bi,	89_bi,	85_bi,	55_bi,	65_bi,	36_bi,		// Grotle
		95_bi,	109_bi,	105_bi,	75_bi,	85_bi,	56_bi,		// Torterra
		44_bi,	58_bi,	44_bi,	58_bi,	44_bi,	61_bi,		// Chimchar
		64_bi,	78_bi,	52_bi,	78_bi,	52_bi,	81_bi,		// Monferno
		76_bi,	104_bi,	71_bi,	104_bi,	71_bi,	108_bi,		// Infernape
		53_bi,	51_bi,	53_bi,	61_bi,	56_bi,	40_bi,		// Piplup
		64_bi,	66_bi,	68_bi,	81_bi,	76_bi,	50_bi,		// Prinplup
		84_bi,	86_bi,	88_bi,	111_bi,	101_bi,	60_bi,		// Empoleon
		40_bi,	55_bi,	30_bi,	30_bi,	30_bi,	60_bi,		// Starly
		55_bi,	75_bi,	50_bi,	40_bi,	40_bi,	80_bi,		// Staravia
		85_bi,	120_bi,	70_bi,	50_bi,	50_bi,	100_bi,		// Staraptor
		59_bi,	45_bi,	40_bi,	35_bi,	40_bi,	31_bi,		// Bidoof
		79_bi,	85_bi,	60_bi,	55_bi,	60_bi,	71_bi,		// Bibarel
		37_bi,	25_bi,	41_bi,	25_bi,	41_bi,	25_bi,		// Kricketot
		77_bi,	85_bi,	51_bi,	55_bi,	51_bi,	65_bi,		// Kricketune
		45_bi,	65_bi,	34_bi,	40_bi,	34_bi,	45_bi,		// Shinx
		60_bi,	85_bi,	49_bi,	60_bi,	49_bi,	60_bi,		// Luxio
		80_bi,	120_bi,	79_bi,	95_bi,	79_bi,	70_bi,		// Luxray
		40_bi,	30_bi,	35_bi,	50_bi,	70_bi,	55_bi,		// Budew
		60_bi,	70_bi,	55_bi,	125_bi,	105_bi,	90_bi,		// Roserade
		67_bi,	125_bi,	40_bi,	30_bi,	30_bi,	58_bi,		// Cranidos
		97_bi,	165_bi,	60_bi,	65_bi,	50_bi,	58_bi,		// Rampardos
		30_bi,	42_bi,	118_bi,	42_bi,	88_bi,	30_bi,		// Shieldon
		60_bi,	52_bi,	168_bi,	47_bi,	138_bi,	30_bi,		// Bastiodon
		40_bi,	29_bi,	45_bi,	29_bi,	45_bi,	36_bi,		// Burmy
		60_bi,	59_bi,	85_bi,	79_bi,	105_bi,	36_bi,		// Wormadam-Plant
		60_bi,	79_bi,	105_bi,	59_bi,	85_bi,	36_bi,		// Wormadam-Sandy
		60_bi,	69_bi,	95_bi,	69_bi,	95_bi,	36_bi,		// Wormadam-Trash
		70_bi,	94_bi,	50_bi,	94_bi,	50_bi,	66_bi,		// Mothim
		30_bi,	30_bi,	42_bi,	30_bi,	42_bi,	70_bi,		// Combee
		70_bi,	80_bi,	102_bi,	80_bi,	102_bi,	40_bi,		// Vespiquen
		60_bi,	45_bi,	70_bi,	45_bi,	90_bi,	95_bi,		// Pachirisu
		55_bi,	65_bi,	35_bi,	60_bi,	30_bi,	85_bi,		// Buizel
		85_bi,	105_bi,	55_bi,	85_bi,	50_bi,	115_bi,		// Floatzel
		45_bi,	35_bi,	45_bi,	62_bi,	53_bi,	35_bi,		// Cherubi
		70_bi,	60_bi,	70_bi,	87_bi,	78_bi,	85_bi,		// Cherrim
		76_bi,	48_bi,	48_bi,	57_bi,	62_bi,	34_bi,		// Shellos
		111_bi,	83_bi,	68_bi,	92_bi,	82_bi,	39_bi,		// Gastrodon
		75_bi,	100_bi,	66_bi,	60_bi,	66_bi,	115_bi,		// Ambipom
		90_bi,	50_bi,	34_bi,	60_bi,	44_bi,	70_bi,		// Drifloon
		150_bi,	80_bi,	44_bi,	90_bi,	54_bi,	80_bi,		// Drifblim
		55_bi,	66_bi,	44_bi,	44_bi,	56_bi,	85_bi,		// Buneary
		65_bi,	76_bi,	84_bi,	54_bi,	96_bi,	105_bi,		// Lopunny
		60_bi,	60_bi,	60_bi,	105_bi,	105_bi,	105_bi,		// Mismagius
		100_bi,	125_bi,	52_bi,	105_bi,	52_bi,	71_bi,		// Honchkrow
		49_bi,	55_bi,	42_bi,	42_bi,	37_bi,	85_bi,		// Glameow
		71_bi,	82_bi,	64_bi,	64_bi,	59_bi,	112_bi,		// Purugly
		45_bi,	30_bi,	50_bi,	65_bi,	50_bi,	45_bi,		// Chingling
		63_bi,	63_bi,	47_bi,	41_bi,	41_bi,	74_bi,		// Stunky
		103_bi,	93_bi,	67_bi,	71_bi,	61_bi,	84_bi,		// Skuntank
		57_bi,	24_bi,	86_bi,	24_bi,	86_bi,	23_bi,		// Bronzor
		67_bi,	89_bi,	116_bi,	79_bi,	116_bi,	33_bi,		// Bronzong
		50_bi,	80_bi,	95_bi,	10_bi,	45_bi,	10_bi,		// Bonsly
		20_bi,	25_bi,	45_bi,	70_bi,	90_bi,	60_bi,		// Mime Jr.
		100_bi,	5_bi,	5_bi,	15_bi,	65_bi,	30_bi,		// Happiny
		76_bi,	65_bi,	45_bi,	92_bi,	42_bi,	91_bi,		// Chatot
		50_bi,	92_bi,	108_bi,	92_bi,	108_bi,	35_bi,		// Spiritomb
		58_bi,	70_bi,	45_bi,	40_bi,	45_bi,	42_bi,		// Gible
		68_bi,	90_bi,	65_bi,	50_bi,	55_bi,	82_bi,		// Gabite
		108_bi,	130_bi,	95_bi,	80_bi,	85_bi,	102_bi,		// Garchomp
		135_bi,	85_bi,	40_bi,	40_bi,	85_bi,	5_bi,		// Munchlax
		40_bi,	70_bi,	40_bi,	35_bi,	40_bi,	60_bi,		// Riolu
		70_bi,	110_bi,	70_bi,	115_bi,	70_bi,	90_bi,		// Lucario
		68_bi,	72_bi,	78_bi,	38_bi,	42_bi,	32_bi,		// Hippopotas
		108_bi,	112_bi,	118_bi,	68_bi,	72_bi,	47_bi,		// Hippowdon
		40_bi,	50_bi,	90_bi,	30_bi,	55_bi,	65_bi,		// Skorupi
		70_bi,	90_bi,	110_bi,	60_bi,	75_bi,	95_bi,		// Drapion
		48_bi,	61_bi,	40_bi,	61_bi,	40_bi,	50_bi,		// Croagunk
		83_bi,	106_bi,	65_bi,	86_bi,	65_bi,	85_bi,		// Toxicroak
		74_bi,	100_bi,	72_bi,	90_bi,	72_bi,	46_bi,		// Carnivine
		49_bi,	49_bi,	56_bi,	49_bi,	61_bi,	66_bi,		// Finneon
		69_bi,	69_bi,	76_bi,	69_bi,	86_bi,	91_bi,		// Lumineon
		45_bi,	20_bi,	50_bi,	60_bi,	120_bi,	50_bi,		// Mantyke
		60_bi,	62_bi,	50_bi,	62_bi,	60_bi,	40_bi,		// Snover
		90_bi,	92_bi,	75_bi,	92_bi,	85_bi,	60_bi,		// Abomasnow
		70_bi,	120_bi,	65_bi,	45_bi,	85_bi,	125_bi,		// Weavile
		70_bi,	70_bi,	115_bi,	130_bi,	90_bi,	60_bi,		// Magnezone
		110_bi,	85_bi,	95_bi,	80_bi,	95_bi,	50_bi,		// Lickilicky
		115_bi,	140_bi,	130_bi,	55_bi,	55_bi,	40_bi,		// Rhyperior
		100_bi,	100_bi,	125_bi,	110_bi,	50_bi,	50_bi,		// Tangrowth
		75_bi,	123_bi,	67_bi,	95_bi,	85_bi,	95_bi,		// Electivire
		75_bi,	95_bi,	67_bi,	125_bi,	95_bi,	83_bi,		// Magmortar
		85_bi,	50_bi,	95_bi,	120_bi,	115_bi,	80_bi,		// Togekiss
		86_bi,	76_bi,	86_bi,	116_bi,	56_bi,	95_bi,		// Yanmega
		65_bi,	110_bi,	130_bi,	60_bi,	65_bi,	95_bi,		// Leafeon
		65_bi,	60_bi,	110_bi,	130_bi,	95_bi,	65_bi,		// Glaceon
		75_bi,	95_bi,	125_bi,	45_bi,	75_bi,	95_bi,		// Gliscor
		110_bi,	130_bi,	80_bi,	70_bi,	60_bi,	80_bi,		// Mamoswine
		85_bi,	80_bi,	70_bi,	135_bi,	75_bi,	90_bi,		// Porygon-Z
		68_bi,	125_bi,	65_bi,	65_bi,	115_bi,	80_bi,		// Gallade
		60_bi,	55_bi,	145_bi,	75_bi,	150_bi,	40_bi,		// Probopass
		45_bi,	100_bi,	135_bi,	65_bi,	135_bi,	45_bi,		// Dusknoir
		70_bi,	80_bi,	70_bi,	80_bi,	70_bi,	110_bi,		// Froslass
		50_bi,	50_bi,	77_bi,	95_bi,	77_bi,	91_bi,		// Rotom
		50_bi,	65_bi,	107_bi,	105_bi,	107_bi,	86_bi,		// Rotom-Heat
		50_bi,	65_bi,	107_bi,	105_bi,	107_bi,	86_bi,		// Rotom-Wash
		50_bi,	65_bi,	107_bi,	105_bi,	107_bi,	86_bi,		// Rotom-Frost
		50_bi,	65_bi,	107_bi,	105_bi,	107_bi,	86_bi,		// Rotom-Fan
		50_bi,	65_bi,	107_bi,	105_bi,	107_bi,	86_bi,		// Rotom-Mow
		75_bi,	75_bi,	130_bi,	75_bi,	130_bi,	95_bi,		// Uxie
		80_bi,	105_bi,	105_bi,	105_bi,	105_bi,	80_bi,		// Mesprit
		75_bi,	125_bi,	70_bi,	125_bi,	70_bi,	115_bi,		// Azelf
		100_bi,	120_bi,	120_bi,	150_bi,	100_bi,	90_bi,		// Dialga
		90_bi,	120_bi,	100_bi,	150_bi,	120_bi,	100_bi,		// Palkia
		91_bi,	90_bi,	106_bi,	130_bi,	106_bi,	77_bi,		// Heatran
		110_bi,	160_bi,	110_bi,	80_bi,	110_bi,	100_bi,		// Regigigas
		150_bi,	100_bi,	120_bi,	100_bi,	120_bi,	90_bi,		// Giratina-Altered
		150_bi,	120_bi,	100_bi,	120_bi,	100_bi,	90_bi,		// Giratina-Origin
		120_bi,	70_bi,	120_bi,	75_bi,	130_bi,	85_bi,		// Cresselia
		80_bi,	80_bi,	80_bi,	80_bi,	80_bi,	80_bi,		// Phione
		100_bi,	100_bi,	100_bi,	100_bi,	100_bi,	100_bi,		// Manaphy
		70_bi,	90_bi,	90_bi,	135_bi,	90_bi,	125_bi,		// Darkrai
		100_bi,	100_bi,	100_bi,	100_bi,	100_bi,	100_bi,		// Shaymin-Land
		100_bi,	103_bi,	75_bi,	120_bi,	75_bi,	127_bi,		// Shaymin-Sky
		120_bi,	120_bi,	120_bi,	120_bi,	120_bi,	120_bi,		// Arceus
		
		// Generation 5
		100_bi,	100_bi,	100_bi,	100_bi,	100_bi,	100_bi,		// Victini
		45_bi,	45_bi,	55_bi,	45_bi,	55_bi,	63_bi,		// Snivy
		60_bi,	60_bi,	75_bi,	60_bi,	75_bi,	83_bi,		// Servine
		75_bi,	75_bi,	95_bi,	75_bi,	95_bi,	113_bi,		// Serperior
		65_bi,	63_bi,	45_bi,	45_bi,	45_bi,	45_bi,		// Tepig
		90_bi,	93_bi,	55_bi,	70_bi,	55_bi,	55_bi,		// Pignite
		110_bi,	123_bi,	65_bi,	100_bi,	65_bi,	65_bi,		// Emboar
		55_bi,	55_bi,	45_bi,	63_bi,	45_bi,	45_bi,		// Oshawott
		75_bi,	75_bi,	60_bi,	83_bi,	60_bi,	60_bi,		// Dewott
		95_bi,	100_bi,	85_bi,	108_bi,	70_bi,	70_bi,		// Samurott
		45_bi,	55_bi,	39_bi,	35_bi,	39_bi,	42_bi,		// Patrat
		60_bi,	85_bi,	69_bi,	60_bi,	69_bi,	77_bi,		// Watchog
		45_bi,	60_bi,	45_bi,	25_bi,	45_bi,	55_bi,		// Lillipup
		65_bi,	80_bi,	65_bi,	35_bi,	65_bi,	60_bi,		// Herdier
		85_bi,	100_bi,	90_bi,	45_bi,	90_bi,	80_bi,		// Stoutland
		41_bi,	50_bi,	37_bi,	50_bi,	37_bi,	66_bi,		// Purrloin
		64_bi,	88_bi,	50_bi,	88_bi,	50_bi,	106_bi,		// Liepard
		50_bi,	53_bi,	48_bi,	53_bi,	48_bi,	64_bi,		// Pansage
		75_bi,	98_bi,	63_bi,	98_bi,	63_bi,	101_bi,		// Simisage
		50_bi,	53_bi,	48_bi,	53_bi,	48_bi,	64_bi,		// Pansear
		75_bi,	98_bi,	63_bi,	98_bi,	63_bi,	101_bi,		// Simisear
		50_bi,	53_bi,	48_bi,	53_bi,	48_bi,	64_bi,		// Panpour
		75_bi,	98_bi,	63_bi,	98_bi,	63_bi,	101_bi,		// Simipour
		76_bi,	25_bi,	45_bi,	67_bi,	55_bi,	24_bi,		// Munna
		116_bi,	55_bi,	85_bi,	107_bi,	95_bi,	29_bi,		// Musharna
		50_bi,	55_bi,	50_bi,	36_bi,	30_bi,	43_bi,		// Pidove
		62_bi,	77_bi,	62_bi,	50_bi,	42_bi,	65_bi,		// Tranquill
		80_bi,	105_bi,	80_bi,	65_bi,	55_bi,	93_bi,		// Unfezant
		45_bi,	60_bi,	32_bi,	50_bi,	32_bi,	76_bi,		// Blitzle
		75_bi,	100_bi,	63_bi,	80_bi,	63_bi,	116_bi,		// Zebstrika
		55_bi,	75_bi,	85_bi,	25_bi,	25_bi,	15_bi,		// Roggenrola
		70_bi,	105_bi,	105_bi,	50_bi,	40_bi,	20_bi,		// Boldore
		85_bi,	135_bi,	130_bi,	60_bi,	70_bi,	25_bi,		// Gigalith
		55_bi,	45_bi,	43_bi,	55_bi,	43_bi,	72_bi,		// Woobat
		67_bi,	57_bi,	55_bi,	77_bi,	55_bi,	114_bi,		// Swoobat
		60_bi,	85_bi,	40_bi,	30_bi,	45_bi,	68_bi,		// Drilbur
		110_bi,	135_bi,	60_bi,	50_bi,	65_bi,	88_bi,		// Excadrill
		103_bi,	60_bi,	86_bi,	60_bi,	86_bi,	50_bi,		// Audino
		75_bi,	80_bi,	55_bi,	25_bi,	35_bi,	35_bi,		// Timburr
		85_bi,	105_bi,	85_bi,	40_bi,	50_bi,	40_bi,		// Gurdurr
		105_bi,	140_bi,	95_bi,	55_bi,	65_bi,	45_bi,		// Conkeldurr
		50_bi,	50_bi,	40_bi,	50_bi,	40_bi,	64_bi,		// Tympole
		75_bi,	65_bi,	55_bi,	65_bi,	55_bi,	69_bi,		// Palpitoad
		105_bi,	85_bi,	75_bi,	85_bi,	75_bi,	74_bi,		// Seismitoad
		120_bi,	100_bi,	85_bi,	30_bi,	85_bi,	45_bi,		// Throh
		75_bi,	125_bi,	75_bi,	30_bi,	75_bi,	85_bi,		// Sawk
		45_bi,	53_bi,	70_bi,	40_bi,	60_bi,	42_bi,		// Sewaddle
		55_bi,	63_bi,	90_bi,	50_bi,	80_bi,	42_bi,		// Swadloon
		75_bi,	103_bi,	80_bi,	70_bi,	70_bi,	92_bi,		// Leavanny
		30_bi,	45_bi,	59_bi,	30_bi,	39_bi,	57_bi,		// Venipede
		40_bi,	55_bi,	99_bi,	40_bi,	79_bi,	47_bi,		// Whirlipede
		60_bi,	90_bi,	89_bi,	55_bi,	69_bi,	112_bi,		// Scolipede
		40_bi,	27_bi,	60_bi,	37_bi,	50_bi,	66_bi,		// Cottonee
		60_bi,	67_bi,	85_bi,	77_bi,	75_bi,	116_bi,		// Whimsicott
		45_bi,	35_bi,	50_bi,	70_bi,	50_bi,	30_bi,		// Petilil
		70_bi,	60_bi,	75_bi,	110_bi,	75_bi,	90_bi,		// Lilligant
		70_bi,	92_bi,	65_bi,	80_bi,	55_bi,	98_bi,		// Basculin-Red
		70_bi,	92_bi,	65_bi,	80_bi,	55_bi,	98_bi,		// Basculin-Blue
		50_bi,	72_bi,	35_bi,	35_bi,	35_bi,	65_bi,		// Sandile
		60_bi,	82_bi,	45_bi,	45_bi,	45_bi,	74_bi,		// Krokorok
		95_bi,	117_bi,	70_bi,	65_bi,	70_bi,	92_bi,		// Krookodile
		70_bi,	90_bi,	45_bi,	15_bi,	45_bi,	50_bi,		// Darumaka
		105_bi,	140_bi,	55_bi,	30_bi,	55_bi,	95_bi,		// Darmanitan
		//105_bi,	30_bi,	105_bi,	140_bi,	105_bi,	55_bi,		// Darmanitan (Zen Mode)
		75_bi,	86_bi,	67_bi,	106_bi,	67_bi,	60_bi,		// Maractus
		50_bi,	65_bi,	85_bi,	35_bi,	35_bi,	55_bi,		// Dwebble
		70_bi,	95_bi,	125_bi,	65_bi,	75_bi,	45_bi,		// Crustle
		50_bi,	75_bi,	70_bi,	35_bi,	70_bi,	48_bi,		// Scraggy
		65_bi,	90_bi,	115_bi,	45_bi,	115_bi,	58_bi,		// Scrafty
		72_bi,	58_bi,	80_bi,	103_bi,	80_bi,	97_bi,		// Sigilyph
		38_bi,	30_bi,	85_bi,	55_bi,	65_bi,	30_bi,		// Yamask
		58_bi,	50_bi,	145_bi,	95_bi,	105_bi,	30_bi,		// Cofagrigus
		54_bi,	78_bi,	103_bi,	53_bi,	45_bi,	22_bi,		// Tirtouga
		74_bi,	108_bi,	133_bi,	83_bi,	65_bi,	32_bi,		// Carracosta
		55_bi,	112_bi,	45_bi,	74_bi,	45_bi,	70_bi,		// Archen
		75_bi,	140_bi,	65_bi,	112_bi,	65_bi,	110_bi,		// Archeops
		50_bi,	50_bi,	62_bi,	40_bi,	62_bi,	65_bi,		// Trubbish
		80_bi,	95_bi,	82_bi,	60_bi,	82_bi,	75_bi,		// Garbodor
		40_bi,	65_bi,	40_bi,	80_bi,	40_bi,	65_bi,		// Zorua
		60_bi,	105_bi,	60_bi,	120_bi,	60_bi,	105_bi,		// Zoroark
		55_bi,	50_bi,	40_bi,	40_bi,	40_bi,	75_bi,		// Minccino
		75_bi,	95_bi,	60_bi,	65_bi,	60_bi,	115_bi,		// Cinccino
		45_bi,	30_bi,	50_bi,	55_bi,	65_bi,	45_bi,		// Gothita
		60_bi,	45_bi,	70_bi,	75_bi,	85_bi,	55_bi,		// Gothorita
		70_bi,	55_bi,	95_bi,	95_bi,	110_bi,	65_bi,		// Gothitelle
		45_bi,	30_bi,	40_bi,	105_bi,	50_bi,	20_bi,		// Solosis
		65_bi,	40_bi,	50_bi,	125_bi,	60_bi,	30_bi,		// Duosion
		110_bi,	65_bi,	75_bi,	125_bi,	85_bi,	30_bi,		// Reuniclus
		62_bi,	44_bi,	50_bi,	44_bi,	50_bi,	55_bi,		// Ducklett
		75_bi,	87_bi,	63_bi,	87_bi,	63_bi,	98_bi,		// Swanna
		36_bi,	50_bi,	50_bi,	65_bi,	60_bi,	44_bi,		// Vanillite
		51_bi,	65_bi,	65_bi,	80_bi,	75_bi,	59_bi,		// Vanillish
		71_bi,	95_bi,	85_bi,	110_bi,	95_bi,	79_bi,		// Vanilluxe
		60_bi,	60_bi,	50_bi,	40_bi,	50_bi,	75_bi,		// Deerling
		80_bi,	100_bi,	70_bi,	60_bi,	70_bi,	95_bi,		// Sawsbuck
		55_bi,	75_bi,	60_bi,	75_bi,	60_bi,	103_bi,		// Emolga
		50_bi,	75_bi,	45_bi,	40_bi,	45_bi,	60_bi,		// Karrablast
		70_bi,	135_bi,	105_bi,	60_bi,	105_bi,	20_bi,		// Escavalier
		69_bi,	55_bi,	45_bi,	55_bi,	55_bi,	15_bi,		// Foongus
		114_bi,	85_bi,	70_bi,	85_bi,	80_bi,	30_bi,		// Amoonguss
		55_bi,	40_bi,	50_bi,	65_bi,	85_bi,	40_bi,		// Frillish
		100_bi,	60_bi,	70_bi,	85_bi,	105_bi,	60_bi,		// Jellicent
		165_bi,	75_bi,	80_bi,	40_bi,	45_bi,	65_bi,		// Alomomola
		50_bi,	47_bi,	50_bi,	57_bi,	50_bi,	65_bi,		// Joltik
		70_bi,	77_bi,	60_bi,	97_bi,	60_bi,	108_bi,		// Galvantula
		44_bi,	50_bi,	91_bi,	24_bi,	86_bi,	10_bi,		// Ferroseed
		74_bi,	94_bi,	131_bi,	54_bi,	116_bi,	20_bi,		// Ferrothorn
		40_bi,	55_bi,	70_bi,	45_bi,	60_bi,	30_bi,		// Klink
		60_bi,	80_bi,	95_bi,	70_bi,	85_bi,	50_bi,		// Klang
		60_bi,	100_bi,	115_bi,	70_bi,	85_bi,	90_bi,		// Klinklang
		35_bi,	55_bi,	40_bi,	45_bi,	40_bi,	60_bi,		// Tynamo
		65_bi,	85_bi,	70_bi,	75_bi,	70_bi,	40_bi,		// Eelektrik
		85_bi,	115_bi,	80_bi,	105_bi,	80_bi,	50_bi,		// Eelektross
		55_bi,	55_bi,	55_bi,	85_bi,	55_bi,	30_bi,		// Elgyem
		75_bi,	75_bi,	75_bi,	125_bi,	95_bi,	40_bi,		// Beheeyem
		50_bi,	30_bi,	55_bi,	65_bi,	55_bi,	20_bi,		// Litwick
		60_bi,	40_bi,	60_bi,	95_bi,	60_bi,	55_bi,		// Lampent
		60_bi,	55_bi,	90_bi,	145_bi,	90_bi,	80_bi,		// Chandelure
		46_bi,	87_bi,	60_bi,	30_bi,	40_bi,	57_bi,		// Axew
		66_bi,	117_bi,	70_bi,	40_bi,	50_bi,	67_bi,		// Fraxure
		76_bi,	147_bi,	90_bi,	60_bi,	70_bi,	97_bi,		// Haxorus
		55_bi,	70_bi,	40_bi,	60_bi,	40_bi,	40_bi,		// Cubchoo
		95_bi,	110_bi,	80_bi,	70_bi,	80_bi,	50_bi,		// Beartic
		70_bi,	50_bi,	30_bi,	95_bi,	135_bi,	105_bi,		// Cryogonal
		50_bi,	40_bi,	85_bi,	40_bi,	65_bi,	25_bi,		// Shelmet
		80_bi,	70_bi,	40_bi,	100_bi,	60_bi,	145_bi,		// Accelgor
		109_bi,	66_bi,	84_bi,	81_bi,	99_bi,	32_bi,		// Stunfisk
		45_bi,	85_bi,	50_bi,	55_bi,	50_bi,	65_bi,		// Mienfoo
		65_bi,	125_bi,	60_bi,	95_bi,	60_bi,	105_bi,		// Mienshao
		77_bi,	120_bi,	90_bi,	60_bi,	90_bi,	48_bi,		// Druddigon
		59_bi,	74_bi,	50_bi,	35_bi,	50_bi,	35_bi,		// Golett
		89_bi,	124_bi,	80_bi,	55_bi,	80_bi,	55_bi,		// Golurk
		45_bi,	85_bi,	70_bi,	40_bi,	40_bi,	60_bi,		// Pawniard
		65_bi,	125_bi,	100_bi,	60_bi,	70_bi,	70_bi,		// Bisharp
		95_bi,	110_bi,	95_bi,	40_bi,	95_bi,	55_bi,		// Bouffalant
		70_bi,	83_bi,	50_bi,	37_bi,	50_bi,	60_bi,		// Rufflet
		100_bi,	123_bi,	75_bi,	57_bi,	75_bi,	80_bi,		// Braviary
		70_bi,	55_bi,	75_bi,	45_bi,	65_bi,	60_bi,		// Vullaby
		110_bi,	65_bi,	105_bi,	55_bi,	95_bi,	80_bi,		// Mandibuzz
		85_bi,	97_bi,	66_bi,	105_bi,	66_bi,	65_bi,		// Heatmor
		58_bi,	109_bi,	112_bi,	48_bi,	48_bi,	109_bi,		// Durant
		52_bi,	65_bi,	50_bi,	45_bi,	50_bi,	38_bi,		// Deino
		72_bi,	85_bi,	70_bi,	65_bi,	70_bi,	58_bi,		// Zweilous
		92_bi,	105_bi,	90_bi,	125_bi,	90_bi,	98_bi,		// Hydreigon
		55_bi,	85_bi,	55_bi,	50_bi,	55_bi,	60_bi,		// Larvesta
		85_bi,	60_bi,	65_bi,	135_bi,	105_bi,	100_bi,		// Volcarona
		91_bi,	90_bi,	129_bi,	90_bi,	72_bi,	108_bi,		// Cobalion
		91_bi,	129_bi,	90_bi,	72_bi,	90_bi,	108_bi,		// Terrakion
		91_bi,	90_bi,	72_bi,	90_bi,	129_bi,	108_bi,		// Virizion
		79_bi,	115_bi,	70_bi,	125_bi,	80_bi,	111_bi,		// Tornadus-Incarnate
		79_bi,	100_bi,	80_bi,	110_bi,	90_bi,	121_bi,		// Tornadus-Therian
		79_bi,	115_bi,	70_bi,	125_bi,	80_bi,	111_bi,		// Thundurus-Incarnate
		79_bi,	105_bi,	70_bi,	145_bi,	80_bi,	101_bi,		// Thundurus-Therian
		100_bi,	120_bi,	100_bi,	150_bi,	120_bi,	90_bi,		// Reshiram
		100_bi,	150_bi,	120_bi,	120_bi,	100_bi,	90_bi,		// Zekrom
		89_bi,	125_bi,	90_bi,	115_bi,	80_bi,	101_bi,		// Landorus-Incarnate
		89_bi,	145_bi,	90_bi,	105_bi,	80_bi,	91_bi,		// Landorus-Therian
		125_bi,	130_bi,	90_bi,	130_bi,	90_bi,	95_bi,		// Kyurem
		125_bi,	170_bi,	100_bi,	120_bi,	90_bi,	95_bi,		// Kyurem-Black
		125_bi,	120_bi,	90_bi,	170_bi,	100_bi,	95_bi,		// Kyurem-White
		91_bi,	72_bi,	90_bi,	129_bi,	90_bi,	108_bi,		// Keldeo
		100_bi,	77_bi,	77_bi,	128_bi,	128_bi,	90_bi,		// Meloetta
		// 100_bi,	128_bi,	90_bi,	77_bi,	77_bi,	128_bi,		// Meloetta (Pirouette form)
		71_bi,	120_bi,	95_bi,	120_bi,	95_bi,	99_bi			// Genesect 
	);
	using value_type = decltype(base_stat)::value_type::value_type;
	static_assert(std::numeric_limits<Stat::base_type>::min() == std::numeric_limits<value_type>::min(), "Incorrect base stat minimum.");
	static_assert(std::numeric_limits<Stat::base_type>::max() == std::numeric_limits<value_type>::max(), "Incorrect base stat maximum.");
	// I add 1 because HP is -1 to allow every other index to be easier.
	return base_stat.at(name).at(stat + 1);
}

}	// unnamed namespace
}	// namespace technicalmachine
