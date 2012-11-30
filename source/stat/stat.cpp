// Stat formulas
// Copyright (C) 2012 David Stone
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

#include "nature.hpp"

#include "../ability.hpp"
#include "../item.hpp"
#include "../rational.hpp"
#include "../status.hpp"
#include "../team.hpp"
#include "../weather.hpp"

#include "../pokemon/pokemon.hpp"

namespace technicalmachine {
namespace {

uint8_t get_base_stat(Species name, Stat::Stats stat_name);

unsigned initial_generic_stat(Stat const & stat, unsigned level);

Rational attack_ability_modifier(Pokemon const & attacker, bool slow_start, Weather const & weather);
Rational attack_item_modifier(Pokemon const & attacker);

Rational special_attack_ability_modifier(Ability const & ability, Weather const & weather);
Rational special_attack_item_modifier(Pokemon const & attacker);

Rational defense_ability_modifier(Pokemon const & pokemon);
Rational defense_item_modifier(Pokemon const & defender);

Rational special_defense_ability_modifier(Ability const & ability, Weather const & weather);
Rational special_defense_item_modifier(Pokemon const & defender);
Rational special_defense_sandstorm_boost(ActivePokemon const & defender, Weather const & weather);

Rational speed_ability_modifier(ActivePokemon const & pokemon, Weather const & weather);
Rational speed_item_modifier(Pokemon const & pokemon);
unsigned paralysis_speed_divisor (Pokemon const & pokemon);
unsigned tailwind_speed_multiplier (Team const & team);

}	// unnamed namespace

Stat::Stat (Species name, Stats stat_name) :
	max (65535),
	base (get_base_stat (name, stat_name)),
	iv (31),
	ev (0)
	{
}

void Stat::calculate_initial_hp (uint8_t const level) {
	max = (base > 1) ? (initial_generic_stat(*this, level) + level + 5) : 1;
	stat = max;
}

template<>
unsigned initial_stat<Stat::HP>(Pokemon const & pokemon) {
	return (pokemon.hp().base > 1) ? (initial_generic_stat(pokemon.hp(), pokemon.level()) + pokemon.level() + 5) : 1;
}
template<>
unsigned initial_stat<Stat::ATK>(Pokemon const & pokemon) {
	return initial_generic_stat(pokemon.atk(), pokemon.level()) * pokemon.nature().boost<Stat::ATK>();
}
template<>
unsigned initial_stat<Stat::SPA>(Pokemon const & pokemon) {
	return initial_generic_stat(pokemon.spa(), pokemon.level()) * pokemon.nature().boost<Stat::SPA>();
}
template<>
unsigned initial_stat<Stat::DEF>(Pokemon const & pokemon) {
	return initial_generic_stat(pokemon.def(), pokemon.level()) * pokemon.nature().boost<Stat::DEF>();
}
template<>
unsigned initial_stat<Stat::SPD>(Pokemon const & pokemon) {
	return initial_generic_stat(pokemon.spd(), pokemon.level()) * pokemon.nature().boost<Stat::SPD>();
}
template<>
unsigned initial_stat<Stat::SPE>(Pokemon const & pokemon) {
	return initial_generic_stat(pokemon.spe(), pokemon.level()) * pokemon.nature().boost<Stat::SPE>();
}

void calculate_attacking_stat (ActivePokemon & attacker, Weather const & weather) {
	if (attacker.move().is_physical())
		calculate_attack(attacker, weather);
	else
		calculate_special_attack(attacker, weather);
}

void calculate_attack(ActivePokemon & attacker, Weather const & weather) {
	attacker.atk().stat = !attacker.power_trick_is_active() ? initial_stat<Stat::ATK>(attacker) : initial_stat<Stat::DEF>(attacker);

	attacker.atk().stat *= attacker.stage_modifier<Stat::ATK>(attacker.critical_hit());

	attacker.atk().stat *= attack_ability_modifier(attacker, attacker.slow_start_is_active(), weather);
	attacker.atk().stat *= attack_item_modifier(attacker);
	
	if (attacker.atk().stat == 0)
		attacker.atk().stat = 1;
}

void calculate_special_attack (ActivePokemon & attacker, Weather const & weather) {
	attacker.spa().stat = initial_stat<Stat::SPA>(attacker);

	attacker.spa().stat *= attacker.stage_modifier<Stat::SPA>(attacker.critical_hit());

	attacker.spa().stat *= special_attack_ability_modifier(attacker.ability(), weather);
	attacker.spa().stat *= special_attack_item_modifier(attacker);

	if (attacker.spa().stat == 0)
		attacker.spa().stat = 1;
}

void calculate_defending_stat (ActivePokemon const & attacker, ActivePokemon & defender, Weather const & weather) {
	if (attacker.move().is_physical())
		calculate_defense(defender, attacker.critical_hit(), attacker.move().is_self_KO());
	else
		calculate_special_defense(defender, weather, attacker.critical_hit());
}

void calculate_defense (ActivePokemon & defender, bool ch, bool is_self_KO) {
	defender.def().stat = !defender.power_trick_is_active() ? initial_stat<Stat::DEF>(defender) : initial_stat<Stat::ATK>(defender);

	defender.def().stat *= defender.stage_modifier<Stat::DEF>(ch);
	
	defender.def().stat *= defense_ability_modifier(defender);
	defender.def().stat *= defense_item_modifier(defender);
	
	if (is_self_KO)
		defender.def().stat /= 2;

	if (defender.def().stat == 0)
		defender.def().stat = 1;
}

void calculate_special_defense (ActivePokemon & defender, Weather const & weather, bool ch) {
	defender.spd().stat = initial_stat<Stat::SPD>(defender);
	
	defender.spd().stat *= defender.stage_modifier<Stat::SPD>(ch);

	defender.spd().stat *= special_defense_ability_modifier(defender.ability(), weather);	
	defender.spd().stat *= special_defense_item_modifier(defender);
	
	defender.spd().stat *= special_defense_sandstorm_boost(defender, weather);
	
	if (defender.spd().stat == 0)
		defender.spd().stat = 1;
}

void calculate_speed (Team & team, Weather const & weather) {
	auto & pokemon = team.pokemon();
	pokemon.spe().stat = initial_stat<Stat::SPE>(pokemon);
	
	pokemon.spe().stat *= pokemon.stage_modifier<Stat::SPE>();

	pokemon.spe().stat *= speed_ability_modifier(pokemon, weather);
	pokemon.spe().stat *= speed_item_modifier(pokemon);
	
	pokemon.spe().stat /= paralysis_speed_divisor (pokemon);
	
	pokemon.spe().stat *= tailwind_speed_multiplier (team);

	if (pokemon.spe().stat == 0)
		pokemon.spe().stat = 1;
}

void order (Team & team1, Team & team2, Weather const & weather, Team* & faster, Team* & slower) {
	if (team1.pokemon().move().priority() == team2.pokemon().move().priority()) {
		calculate_speed(team1, weather);
		calculate_speed(team2, weather);
		faster_pokemon (team1, team2, weather, faster, slower);
	}
	else if (team1.pokemon().move().priority() > team2.pokemon().move().priority()) {
		faster = &team1;
		slower = &team2;
	}
	else {			// if (team2.pokemon().move().priority() > team1.pokemon().move().priority())
		faster = &team2;
		slower = &team1;
	}
}

void faster_pokemon (Team & team1, Team & team2, Weather const & weather, Team* & faster, Team* & slower) {
	if (team1.pokemon().spe().stat > team2.pokemon().spe().stat) {
		faster = &team1;
		slower = &team2;
	}
	else if (team1.pokemon().spe().stat < team2.pokemon().spe().stat) {
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

unsigned initial_generic_stat(Stat const & stat, unsigned level) {
	return (2u * stat.base + stat.iv + stat.ev.points()) * level / 100 + 5;
}

Rational attack_ability_modifier(Pokemon const & attacker, bool slow_start, Weather const & weather) {
	switch (attacker.ability().name) {
		case Ability::FLOWER_GIFT:
			return weather.sun() ? Rational(3, 2) : Rational(1);
		case Ability::GUTS:
			return (!attacker.status().is_clear()) ? Rational(3, 2) : Rational(1);
		case Ability::HUSTLE:
			return Rational(3, 2);
		case Ability::HUGE_POWER:
		case Ability::PURE_POWER:
			return Rational(2);
		case Ability::SLOW_START:
			return slow_start ? Rational(1, 2) : Rational(1);
		default:
			return Rational(1);
	}
}

Rational attack_item_modifier(Pokemon const & attacker) {
	switch (attacker.item().name) {
		case Item::CHOICE_BAND:
			return Rational(3, 2);
		case Item::LIGHT_BALL:
			return attacker.is_boosted_by_light_ball() ? Rational(2) : Rational(1);
		case Item::THICK_CLUB:
			return attacker.is_boosted_by_thick_club() ? Rational(2, 1) : Rational(1);
		default:
			return Rational(1);
	}
}

Rational special_attack_ability_modifier(Ability const & ability, Weather const & weather) {
	return !ability.boosts_special_attack(weather) ? Rational(1) : Rational(3, 2);
}

Rational special_attack_item_modifier(Pokemon const & attacker) {
	switch (attacker.item().name) {
		case Item::SOUL_DEW:
			return attacker.is_boosted_by_soul_dew() ? Rational(3, 2) : Rational(1);
		case Item::CHOICE_SPECS:
			return Rational(3, 2);
		case Item::DEEPSEATOOTH:
			return attacker.is_boosted_by_deepseatooth() ? Rational(2) : Rational(1);
		case Item::LIGHT_BALL:
			return attacker.is_boosted_by_light_ball() ? Rational(2) : Rational(1);
		default:
			return Rational(1);
	}
}

Rational defense_ability_modifier(Pokemon const & defender) {
	return defender.ability().boosts_defense(defender.status()) ? Rational(3, 2) : Rational(1);
}

Rational defense_item_modifier(Pokemon const & defender) {
	return (defender.item().name == Item::METAL_POWDER and defender.is_boosted_by_metal_powder()) ?
		Rational(3, 2) :
		Rational(1);
}

Rational special_defense_ability_modifier(Ability const & ability, Weather const & weather) {
	return ability.boosts_special_defense(weather) ? Rational(3, 2) : Rational(1);
}

Rational special_defense_item_modifier(Pokemon const & defender) {
	switch (defender.item().name) {
		case Item::DEEPSEASCALE:
			return defender.is_boosted_by_deepseascale() ? Rational(2) : Rational(1);
		case Item::METAL_POWDER:
			return defender.is_boosted_by_metal_powder() ? Rational(3, 2) : Rational(1);
		case Item::SOUL_DEW:
			return defender.is_boosted_by_soul_dew() ? Rational(3, 2) : Rational(1);
		default:
			return Rational(1);
	}
}

Rational special_defense_sandstorm_boost(ActivePokemon const & defender, Weather const & weather) {
	return (is_type(defender, Type::Rock) and weather.sand()) ? Rational(3, 2) : Rational(1);
}

Rational speed_ability_modifier(ActivePokemon const & pokemon, Weather const & weather) {
	switch (pokemon.ability().name) {
		case Ability::CHLOROPHYLL:
			return weather.sun() ? Rational(2) : Rational(1);
		case Ability::SWIFT_SWIM:
			return weather.rain() ? Rational(2) : Rational(1);
		case Ability::UNBURDEN:
			return pokemon.item().was_lost() ? Rational(2) : Rational(1);
		case Ability::QUICK_FEET:
			return (!pokemon.status().is_clear()) ? Rational(3, 2) : Rational(1);
		case Ability::SLOW_START:
			return pokemon.slow_start_is_active() ? Rational(1, 2) : Rational(1);
		default:
			return Rational(1);
	}
}

Rational speed_item_modifier(Pokemon const & pokemon) {
	switch (pokemon.item().name) {
		case Item::QUICK_POWDER:
			return pokemon.is_boosted_by_quick_powder() ? Rational(2) : Rational(1);
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

unsigned paralysis_speed_divisor (Pokemon const & pokemon) {
	return pokemon.status().lowers_speed(pokemon.ability()) ? 4 : 1;
}

unsigned tailwind_speed_multiplier (Team const & team) {
	return team.screens.tailwind() ? 2 : 1;
}

uint8_t get_base_stat (Species name, Stat::Stats stat) {
	constexpr static uint8_t base_stat [][6] = {
	
		// Generation 1
		{	45,	49,	49,	65,	65,	45	},		// Bulbasaur
		{	60,	62,	63,	80,	80,	60	},		// Ivysaur
		{	80,	82,	83,	100,	100,	80	},		// Venusaur
		{	39,	52,	43,	60,	50,	65	},		// Charmander
		{	58,	64,	58,	80,	65,	80	},		// Charmeleon
		{	78,	84,	78,	109,	85,	100	},		// Charizard
		{	44,	48,	65,	50,	64,	43	},		// Squirtle
		{	59,	63,	80,	65,	80,	58	},		// Wartortle
		{	79,	83,	100,	85,	105,	78	},		// Blastoise
		{	45,	30,	35,	20,	20,	45	},		// Caterpie
		{	50,	20,	55,	25,	25,	30	},		// Metapod
		{	60,	45,	50,	80,	80,	70	},		// Butterfree
		{	40,	35,	30,	20,	20,	50	},		// Weedle
		{	45,	25,	50,	25,	25,	35	},		// Kakuna
		{	65,	80,	40,	45,	80,	75	},		// Beedrill
		{	40,	45,	40,	35,	35,	56	},		// Pidgey
		{	63,	60,	55,	50,	50,	71	},		// Pidgeotto
		{	83,	80,	75,	70,	70,	91	},		// Pidgeot
		{	30,	56,	35,	25,	35,	72	},		// Rattata
		{	55,	81,	60,	50,	70,	97	},		// Raticate
		{	40,	60,	30,	31,	31,	70	},		// Spearow
		{	65,	90,	65,	61,	61,	100	},		// Fearow
		{	35,	60,	44,	40,	54,	55	},		// Ekans
		{	60,	85,	69,	65,	79,	80	},		// Arbok
		{	35,	55,	30,	50,	40,	90	},		// Pikachu
		{	60,	90,	55,	90,	80,	100	},		// Raichu
		{	50,	75,	85,	20,	30,	40	},		// Sandshrew
		{	75,	100,	110,	45,	55,	65	},		// Sandslash
		{	55,	47,	52,	40,	40,	41	},		// Nidoran-F
		{	70,	62,	67,	55,	55,	56	},		// Nidorina
		{	90,	82,	87,	75,	85,	76	},		// Nidoqueen
		{	46,	57,	40,	40,	40,	50	},		// Nidoran-M
		{	61,	72,	57,	55,	55,	65	},		// Nidorino
		{	81,	92,	77,	85,	75,	85	},		// Nidoking
		{	70,	45,	48,	60,	65,	35	},		// Clefairy
		{	95,	70,	73,	85,	90,	60	},		// Clefable
		{	38,	41,	40,	50,	65,	65	},		// Vulpix
		{	73,	76,	75,	81,	100,	100	},		// Ninetales
		{	115,	45,	20,	45,	25,	20	},		// Jigglypuff
		{	140,	70,	45,	75,	50,	45	},		// Wigglytuff
		{	40,	45,	35,	30,	40,	55	},		// Zubat
		{	75,	80,	70,	65,	75,	90	},		// Golbat
		{	45,	50,	55,	75,	65,	30	},		// Oddish
		{	60,	65,	70,	85,	75,	40	},		// Gloom
		{	75,	80,	85,	100,	90,	50	},		// Vileplume
		{	35,	70,	55,	45,	55,	25	},		// Paras
		{	60,	95,	80,	60,	80,	30	},		// Parasect
		{	60,	55,	50,	40,	55,	45	},		// Venonat
		{	70,	65,	60,	90,	75,	90	},		// Venomoth
		{	10,	55,	25,	35,	45,	95	},		// Diglett
		{	35,	80,	50,	50,	70,	120	},		// Dugtrio
		{	40,	45,	35,	40,	40,	90	},		// Meowth
		{	65,	70,	60,	65,	65,	115	},		// Persian
		{	50,	52,	48,	65,	50,	55	},		// Psyduck
		{	80,	82,	78,	95,	80,	85	},		// Golduck
		{	40,	80,	35,	35,	45,	70	},		// Mankey
		{	65,	105,	60,	60,	70,	95	},		// Primeape
		{	55,	70,	45,	70,	50,	60	},		// Growlithe
		{	90,	110,	80,	100,	80,	95	},		// Arcanine
		{	40,	50,	40,	40,	40,	90	},		// Poliwag
		{	65,	65,	65,	50,	50,	90	},		// Poliwhirl
		{	90,	85,	95,	70,	90,	70	},		// Poliwrath
		{	25,	20,	15,	105,	55,	90	},		// Abra
		{	40,	35,	30,	120,	70,	105	},		// Kadabra
		{	55,	50,	45,	135,	85,	120	},		// Alakazam
		{	70,	80,	50,	35,	35,	35	},		// Machop
		{	80,	100,	70,	50,	60,	45	},		// Machoke
		{	90,	130,	80,	65,	85,	55	},		// Machamp
		{	50,	75,	35,	70,	30,	40	},		// Bellsprout
		{	65,	90,	50,	85,	45,	55	},		// Weepinbell
		{	80,	105,	65,	100,	60,	70	},		// Victreebel
		{	40,	40,	35,	50,	100,	70	},		// Tentacool
		{	80,	70,	65,	80,	120,	100	},		// Tentacruel
		{	40,	80,	100,	30,	30,	20	},		// Geodude
		{	55,	95,	115,	45,	45,	35	},		// Graveler
		{	80,	110,	130,	55,	65,	45	},		// Golem
		{	50,	85,	55,	65,	65,	90	},		// Ponyta
		{	65,	100,	70,	80,	80,	105	},		// Rapidash
		{	90,	65,	65,	40,	40,	15	},		// Slowpoke
		{	95,	75,	110,	100,	80,	30	},		// Slowbro
		{	25,	35,	70,	95,	55,	45	},		// Magnemite
		{	50,	60,	95,	120,	70,	70	},		// Magneton
		{	52,	65,	55,	58,	62,	60	},		// Farfetch'd
		{	35,	85,	45,	35,	35,	75	},		// Doduo
		{	60,	110,	70,	60,	60,	100	},		// Dodrio
		{	65,	45,	55,	45,	70,	45	},		// Seel
		{	90,	70,	80,	70,	95,	70	},		// Dewgong
		{	80,	80,	50,	40,	50,	25	},		// Grimer
		{	105,	105,	75,	65,	100,	50	},		// Muk
		{	30,	65,	100,	45,	25,	40	},		// Shellder
		{	50,	95,	180,	85,	45,	70	},		// Cloyster
		{	30,	35,	30,	100,	35,	80	},		// Gastly
		{	45,	50,	45,	115,	55,	95	},		// Haunter
		{	60,	65,	60,	130,	75,	110	},		// Gengar
		{	35,	45,	160,	30,	45,	70	},		// Onix
		{	60,	48,	45,	43,	90,	42	},		// Drowzee
		{	85,	73,	70,	73,	115,	67	},		// Hypno
		{	30,	105,	90,	25,	25,	50	},		// Krabby
		{	55,	130,	115,	50,	50,	75	},		// Kingler
		{	40,	30,	50,	55,	55,	100	},		// Voltorb
		{	60,	50,	70,	80,	80,	140	},		// Electrode
		{	60,	40,	80,	60,	45,	40	},		// Exeggcute
		{	95,	95,	85,	125,	65,	55	},		// Exeggutor
		{	50,	50,	95,	40,	50,	35	},		// Cubone
		{	60,	80,	110,	50,	80,	45	},		// Marowak
		{	50,	120,	53,	35,	110,	87	},		// Hitmonlee
		{	50,	105,	79,	35,	110,	76	},		// Hitmonchan
		{	90,	55,	75,	60,	75,	30	},		// Lickitung
		{	40,	65,	95,	60,	45,	35	},		// Koffing
		{	65,	90,	120,	85,	70,	60	},		// Weezing
		{	80,	85,	95,	30,	30,	25	},		// Rhyhorn
		{	105,	130,	120,	45,	45,	40	},		// Rhydon
		{	250,	5, 	5, 	35,	105,	50	},		// Chansey
		{	65,	55,	115,	100,	40,	60	},		// Tangela
		{	105,	95,	80,	40,	80,	90	},		// Kangaskhan
		{	30,	40,	70,	70,	25,	60	},		// Horsea
		{	55,	65,	95,	95,	45,	85	},		// Seadra
		{	45,	67,	60,	35,	50,	63	},		// Goldeen
		{	80,	92,	65,	65,	80,	68	},		// Seaking
		{	30,	45,	55,	70,	55,	85	},		// Staryu
		{	60,	75,	85,	100,	85,	115	},		// Starmie
		{	40,	45,	65,	100,	120,	90	},		// Mr. Mime
		{	70,	110,	80,	55,	80,	105	},		// Scyther
		{	65,	50,	35,	115,	95,	95	},		// Jynx
		{	65,	83,	57,	95,	85,	105	},		// Electabuzz
		{	65,	95,	57,	100,	85,	93	},		// Magmar
		{	65,	125,	100,	55,	70,	85	},		// Pinsir
		{	75,	100,	95,	40,	70,	110	},		// Tauros
		{	20,	10,	55,	15,	20,	80	},		// Magikarp
		{	95,	125,	79,	60,	100,	81	},		// Gyarados
		{	130,	85,	80,	85,	95,	60	},		// Lapras
		{	48,	48,	48,	48,	48,	48	},		// Ditto
		{	55,	55,	50,	45,	65,	55	},		// Eevee
		{	130,	65,	60,	110,	95,	65	},		// Vaporeon
		{	65,	65,	60,	110,	95,	130	},		// Jolteon
		{	65,	130,	60,	95,	110,	65	},		// Flareon
		{	65,	60,	70,	85,	75,	40	},		// Porygon
		{	35,	40,	100,	90,	55,	35	},		// Omanyte
		{	70,	60,	125,	115,	70,	55	},		// Omastar
		{	30,	80,	90,	55,	45,	55	},		// Kabuto
		{	60,	115,	105,	65,	70,	80	},		// Kabutops
		{	80,	105,	65,	60,	75,	130	},		// Aerodactyl
		{	160,	110,	65,	65,	110,	30	},		// Snorlax
		{	90,	85,	100,	95,	125,	85	},		// Articuno
		{	90,	90,	85,	125,	90,	100	},		// Zapdos
		{	90,	100,	90,	125,	85,	90	},		// Moltres
		{	41,	64,	45,	50,	50,	50	},		// Dratini
		{	61,	84,	65,	70,	70,	70	},		// Dragonair
		{	91,	134,	95,	100,	100,	80	},		// Dragonite
		{	106,	110,	90,	154,	90,	130	},		// Mewtwo
		{	100,	100,	100,	100,	100,	100	},		// Mew
		
		// Generation 2
		{	45,	49,	65,	49,	65,	45	},		// Chikorita
		{	60,	62,	80,	63,	80,	60	},		// Bayleef
		{	80,	82,	100,	83,	100,	80	},		// Meganium
		{	39,	52,	43,	60,	50,	65	},		// Cyndaquil
		{	58,	64,	58,	80,	65,	80	},		// Quilava
		{	78,	84,	78,	109,	85,	100	},		// Typhlosion
		{	50,	65,	64,	44,	48,	43	},		// Totodile
		{	65,	80,	80,	59,	63,	58	},		// Croconaw
		{	85,	105,	100,	79,	83,	78	},		// Feraligatr
		{	35,	46,	34,	35,	45,	20	},		// Sentret
		{	85,	76,	64,	45,	55,	90	},		// Furret
		{	60,	30,	30,	36,	56,	50	},		// Hoothoot
		{	100,	50,	50,	76,	96,	70	},		// Noctowl
		{	40,	20,	30,	40,	80,	55	},		// Ledyba
		{	55,	35,	50,	55,	110,	85	},		// Ledian
		{	40,	60,	40,	40,	40,	30	},		// Spinarak
		{	70,	90,	70,	60,	60,	40	},		// Ariados
		{	85,	90,	80,	70,	80,	130	},		// Crobat
		{	75,	38,	38,	56,	56,	67	},		// Chinchou
		{	125,	58,	58,	76,	76,	67	},		// Lanturn
		{	20,	40,	15,	35,	35,	60	},		// Pichu
		{	50,	25,	28,	45,	55,	15	},		// Cleffa
		{	90,	30,	15,	40,	20,	15	},		// Igglybuff
		{	35,	20,	65,	40,	65,	20	},		// Togepi
		{	55,	40,	85,	80,	105,	40	},		// Togetic
		{	40,	50,	45,	70,	45,	70	},		// Natu
		{	65,	75,	70,	95,	70,	95	},		// Xatu
		{	55,	40,	40,	65,	45,	35	},		// Mareep
		{	70,	55,	55,	80,	60,	45	},		// Flaaffy
		{	90,	75,	75,	115,	90,	55	},		// Ampharos
		{	75,	80,	85,	90,	100,	50	},		// Bellossom
		{	70,	20,	50,	20,	50,	40	},		// Marill
		{	100,	50,	80,	50,	80,	50	},		// Azumarill
		{	70,	100,	115,	30,	65,	30	},		// Sudowoodo
		{	90,	75,	75,	90,	100,	70	},		// Politoed
		{	35,	35,	40,	35,	55,	50	},		// Hoppip
		{	55,	45,	50,	45,	65,	80	},		// Skiploom
		{	75,	55,	70,	55,	85,	110	},		// Jumpluff
		{	55,	70,	55,	40,	55,	85	},		// Aipom
		{	30,	30,	30,	30,	30,	30	},		// Sunkern
		{	75,	75,	55,	105,	85,	30	},		// Sunflora
		{	65,	65,	45,	75,	45,	95	},		// Yanma
		{	55,	45,	45,	25,	25,	15	},		// Wooper
		{	95,	85,	85,	65,	65,	35	},		// Quagsire
		{	65,	65,	60,	130,	95,	110	},		// Espeon
		{	95,	65,	110,	60,	130,	65	},		// Umbreon
		{	60,	85,	42,	85,	42,	91	},		// Murkrow
		{	95,	75,	80,	100,	110,	30	},		// Slowking
		{	60,	60,	60,	85,	85,	85	},		// Misdreavus
		{	48,	72,	48,	72,	48,	48	},		// Unown
		{	190,	33,	58,	33,	58,	33	},		// Wobbuffet
		{	70,	80,	65,	90,	65,	85	},		// Girafarig
		{	50,	65,	90,	35,	35,	15	},		// Pineco
		{	75,	90,	140,	60,	60,	40	},		// Forretress
		{	100,	70,	70,	65,	65,	45	},		// Dunsparce
		{	65,	75,	105,	35,	65,	85	},		// Gligar
		{	75,	85,	200,	55,	65,	30	},		// Steelix
		{	60,	80,	50,	40,	40,	30	},		// Snubbull
		{	90,	120,	75,	60,	60,	45	},		// Granbull
		{	65,	95,	75,	55,	55,	85	},		// Qwilfish
		{	70,	130,	100,	55,	80,	65	},		// Scizor
		{	20,	10,	230,	10,	230,	5  	},		// Shuckle
		{	80,	125,	75,	40,	95,	85	},		// Heracross
		{	55,	95,	55,	35,	75,	115	},		// Sneasel
		{	60,	80,	50,	50,	50,	40	},		// Teddiursa
		{	90,	130,	75,	75,	75,	55	},		// Ursaring
		{	40,	40,	40,	70,	40,	20	},		// Slugma
		{	50,	50,	120,	80,	80,	30	},		// Magcargo
		{	50,	50,	40,	30,	30,	50	},		// Swinub
		{	100,	100,	80,	60,	60,	50	},		// Piloswine
		{	55,	55,	85,	65,	85,	35	},		// Corsola
		{	35,	65,	35,	65,	35,	65	},		// Remoraid
		{	75,	105,	75,	105,	75,	45	},		// Octillery
		{	45,	55,	45,	65,	45,	75	},		// Delibird
		{	65,	40,	70,	80,	140,	70	},		// Mantine
		{	65,	80,	140,	40,	70,	70	},		// Skarmory
		{	45,	60,	30,	80,	50,	65	},		// Houndour
		{	75,	90,	50,	110,	80,	95	},		// Houndoom
		{	75,	95,	95,	95,	95,	85	},		// Kingdra
		{	90,	60,	60,	40,	40,	40	},		// Phanpy
		{	90,	120,	120,	60,	60,	50	},		// Donphan
		{	85,	80,	90,	105,	95,	60	},		// Porygon2
		{	73,	95,	62,	85,	65,	85	},		// Stantler
		{	55,	20,	35,	20,	45,	75	},		// Smeargle
		{	35,	35,	35,	35,	35,	35	},		// Tyrogue
		{	50,	95,	95,	35,	110,	70	},		// Hitmontop
		{	45,	30,	15,	85,	65,	65	},		// Smoochum
		{	45,	63,	37,	65,	55,	95	},		// Elekid
		{	45,	75,	37,	70,	55,	83	},		// Magby
		{	95,	80,	105,	40,	70,	100	},		// Miltank
		{	255,	10,	10,	75,	135,	55	},		// Blissey
		{	90,	85,	75,	115,	100,	115	},		// Raikou
		{	115,	115,	85,	90,	75,	100	},		// Entei
		{	100,	75,	115,	90,	115,	85	},		// Suicune
		{	50,	64,	50,	45,	50,	41	},		// Larvitar
		{	70,	84,	70,	65,	70,	51	},		// Pupitar
		{	100,	134,	110,	95,	100,	61	},		// Tyranitar
		{	106,	90,	130,	90,	154,	110	},		// Lugia
		{	106,	130,	90,	110,	154,	90	},		// Ho-Oh
		{	100,	100,	100,	100,	100,	100	},		// Celebi
		
		// Generation 3
		{	40,	45,	35,	65,	55,	70	},		// Treecko
		{	50,	65,	45,	85,	65,	95	},		// Grovyle
		{	70,	85,	65,	105,	85,	120	},		// Sceptile
		{	45,	60,	40,	70,	50,	45	},		// Torchic
		{	60,	85,	60,	85,	60,	55	},		// Combusken
		{	80,	120,	70,	110,	70,	80	},		// Blaziken
		{	50,	70,	50,	50,	50,	40	},		// Mudkip
		{	70,	85,	70,	60,	70,	50	},		// Marshtomp
		{	100,	110,	90,	85,	90,	60	},		// Swampert
		{	35,	55,	35,	30,	30,	35	},		// Poochyena
		{	70,	90,	70,	60,	60,	70	},		// Mightyena
		{	38,	30,	41,	30,	41,	60	},		// Zigzagoon
		{	78,	70,	61,	50,	61,	100	},		// Linoone
		{	45,	45,	35,	20,	30,	20	},		// Wurmple
		{	50,	35,	55,	25,	25,	15	},		// Silcoon
		{	60,	70,	50,	90,	50,	65	},		// Beautifly
		{	50,	35,	55,	25,	25,	15	},		// Cascoon
		{	60,	50,	70,	50,	90,	65	},		// Dustox
		{	40,	30,	30,	40,	50,	30	},		// Lotad
		{	60,	50,	50,	60,	70,	50	},		// Lombre
		{	80,	70,	70,	90,	100,	70	},		// Ludicolo
		{	40,	40,	50,	30,	30,	30	},		// Seedot
		{	70,	70,	40,	60,	40,	60	},		// Nuzleaf
		{	90,	100,	60,	90,	60,	80	},		// Shiftry
		{	40,	55,	30,	30,	30,	85	},		// Taillow
		{	60,	85,	60,	50,	50,	125	},		// Swellow
		{	40,	30,	30,	55,	30,	85	},		// Wingull
		{	60,	50,	100,	85,	70,	65	},		// Pelipper
		{	28,	25,	25,	45,	35,	40	},		// Ralts
		{	38,	35,	35,	65,	55,	50	},		// Kirlia
		{	68,	65,	65,	125,	115,	80	},		// Gardevoir
		{	40,	30,	32,	50,	52,	65	},		// Surskit
		{	70,	60,	62,	80,	82,	60	},		// Masquerain
		{	60,	40,	60,	40,	60,	35	},		// Shroomish
		{	60,	130,	80,	60,	60,	70	},		// Breloom
		{	60,	60,	60,	35,	35,	30	},		// Slakoth
		{	80,	80,	80,	55,	55,	90	},		// Vigoroth
		{	150,	160,	100,	95,	65,	100	},		// Slaking
		{	31,	45,	90,	30,	30,	40	},		// Nincada
		{	61,	90,	45,	50,	50,	160	},		// Ninjask
		{	1, 	90,	45,	30,	30,	40	},		// Shedinja
		{	64,	51,	23,	51,	23,	28	},		// Whismur
		{	84,	71,	43,	71,	43,	48	},		// Loudred
		{	104,	91,	63,	91,	63,	68	},		// Exploud
		{	72,	60,	30,	20,	30,	25	},		// Makuhita
		{	144,	120,	60,	40,	60,	50	},		// Hariyama
		{	50,	20,	40,	20,	40,	20	},		// Azurill
		{	30,	45,	135,	45,	90,	30	},		// Nosepass
		{	50,	45,	45,	35,	35,	50	},		// Skitty
		{	70,	65,	65,	55,	55,	70	},		// Delcatty
		{	50,	75,	75,	65,	65,	50	},		// Sableye
		{	50,	85,	85,	55,	55,	50	},		// Mawile
		{	50,	70,	100,	40,	40,	30	},		// Aron
		{	60,	90,	140,	50,	50,	40	},		// Lairon
		{	70,	110,	180,	60,	60,	50	},		// Aggron
		{	30,	40,	55,	40,	55,	60	},		// Meditite
		{	60,	60,	75,	60,	75,	80	},		// Medicham
		{	40,	45,	40,	65,	40,	65	},		// Electrike
		{	70,	75,	60,	105,	60,	105	},		// Manectric
		{	60,	50,	40,	85,	75,	95	},		// Plusle
		{	60,	40,	50,	75,	85,	95	},		// Minun
		{	65,	73,	55,	47,	75,	85	},		// Volbeat
		{	65,	47,	55,	73,	75,	85	},		// Illumise
		{	50,	60,	45,	100,	80,	65	},		// Roselia
		{	70,	43,	53,	43,	53,	40	},		// Gulpin
		{	100,	73,	83,	73,	83,	55	},		// Swalot
		{	45,	90,	20,	65,	20,	65	},		// Carvanha
		{	70,	120,	40,	95,	40,	95	},		// Sharpedo
		{	130,	70,	35,	70,	35,	60	},		// Wailmer
		{	170,	90,	45,	90,	45,	60	},		// Wailord
		{	60,	60,	40,	65,	45,	35	},		// Numel
		{	70,	100,	70,	105,	75,	40	},		// Camerupt
		{	70,	85,	140,	85,	70,	20	},		// Torkoal
		{	60,	25,	35,	70,	80,	60	},		// Spoink
		{	80,	45,	65,	90,	110,	80	},		// Grumpig
		{	60,	60,	60,	60,	60,	60	},		// Spinda
		{	45,	100,	45,	45,	45,	10	},		// Trapinch
		{	50,	70,	50,	50,	50,	70	},		// Vibrava
		{	80,	100,	80,	80,	80,	100	},		// Flygon
		{	50,	85,	40,	85,	40,	35	},		// Cacnea
		{	70,	115,	60,	115,	60,	55	},		// Cacturne
		{	45,	40,	60,	40,	75,	50	},		// Swablu
		{	75,	70,	90,	70,	105,	80	},		// Altaria
		{	73,	115,	60,	60,	60,	90	},		// Zangoose
		{	73,	100,	60,	100,	60,	65	},		// Seviper
		{	70,	55,	65,	95,	85,	70	},		// Lunatone
		{	70,	95,	85,	55,	65,	70	},		// Solrock
		{	50,	48,	43,	46,	41,	60	},		// Barboach
		{	110,	78,	73,	76,	71,	60	},		// Whiscash
		{	43,	80,	65,	50,	35,	35	},		// Corphish
		{	63,	120,	85,	90,	55,	55	},		// Crawdaunt
		{	40,	40,	55,	40,	70,	55	},		// Baltoy
		{	60,	70,	105,	70,	120,	75	},		// Claydol
		{	66,	41,	77,	61,	87,	23	},		// Lileep
		{	86,	81,	97,	81,	107,	43	},		// Cradily
		{	45,	95,	50,	40,	50,	75	},		// Anorith
		{	75,	125,	100,	70,	80,	45	},		// Armaldo
		{	20,	15,	20,	10,	55,	80	},		// Feebas
		{	95,	60,	79,	100,	125,	81	},		// Milotic
		{	70,	70,	70,	70,	70,	70	},		// Castform
		{	60,	90,	70,	60,	120,	40	},		// Kecleon
		{	44,	75,	35,	63,	33,	45	},		// Shuppet
		{	64,	115,	65,	83,	63,	65	},		// Banette
		{	20,	40,	90,	30,	90,	25	},		// Duskull
		{	40,	70,	130,	60,	130,	25	},		// Dusclops
		{	99,	68,	83,	72,	87,	51	},		// Tropius
		{	65,	50,	70,	95,	80,	65	},		// Chimecho
		{	65,	130,	60,	75,	60,	75	},		// Absol
		{	95,	23,	48,	23,	48,	23	},		// Wynaut
		{	50,	50,	50,	50,	50,	50	},		// Snorunt
		{	80,	80,	80,	80,	80,	80	},		// Glalie
		{	70,	40,	50,	55,	50,	25	},		// Spheal
		{	90,	60,	70,	75,	70,	45	},		// Sealeo
		{	110,	80,	90,	95,	90,	65	},		// Walrein
		{	35,	64,	85,	74,	55,	32	},		// Clamperl
		{	55,	104,	105,	94,	75,	52	},		// Huntail
		{	55,	84,	105,	114,	75,	52	},		// Gorebyss
		{	100,	90,	130,	45,	65,	55	},		// Relicanth
		{	43,	30,	55,	40,	65,	97	},		// Luvdisc
		{	45,	75,	60,	40,	30,	50	},		// Bagon
		{	65,	95,	100,	60,	50,	50	},		// Shelgon
		{	95,	135,	80,	110,	80,	100	},		// Salamence
		{	40,	55,	80,	35,	60,	30	},		// Beldum
		{	60,	75,	100,	55,	80,	50	},		// Metang
		{	80,	135,	130,	95,	90,	70	},		// Metagross
		{	80,	100,	200,	50,	100,	50	},		// Regirock
		{	80,	50,	100,	100,	200,	50	},		// Regice
		{	80,	75,	150,	75,	150,	50	},		// Registeel
		{	80,	80,	90,	110,	130,	110	},		// Latias
		{	80,	90,	80,	130,	110,	110	},		// Latios
		{	100,	100,	90,	150,	140,	90	},		// Kyogre
		{	100,	150,	140,	100,	90,	90	},		// Groudon
		{	105,	150,	90,	150,	90,	95	},		// Rayquaza
		{	100,	100,	100,	100,	100,	100	},		// Jirachi
		{	50,	150,	50,	150,	50,	150	},		// Deoxys-Mediocre
		{	50,	180,	20,	180,	20,	150	},		// Deoxys-Attack
		{	50,	70,	160,	70,	160,	90	},		// Deoxys-Defense
		{	50,	95,	90,	95,	90,	180	},		// Deoxys-Speed
		
		// Generation 4
		{	55,	68,	64,	45,	55,	31	},		// Turtwig
		{	75,	89,	85,	55,	65,	36	},		// Grotle
		{	95,	109,	105,	75,	85,	56	},		// Torterra
		{	44,	58,	44,	58,	44,	61	},		// Chimchar
		{	64,	78,	52,	78,	52,	81	},		// Monferno
		{	76,	104,	71,	104,	71,	108	},		// Infernape
		{	53,	51,	53,	61,	56,	40	},		// Piplup
		{	64,	66,	68,	81,	76,	50	},		// Prinplup
		{	84,	86,	88,	111,	101,	60	},		// Empoleon
		{	40,	55,	30,	30,	30,	60	},		// Starly
		{	55,	75,	50,	40,	40,	80	},		// Staravia
		{	85,	120,	70,	50,	50,	100	},		// Staraptor
		{	59,	45,	40,	35,	40,	31	},		// Bidoof
		{	79,	85,	60,	55,	60,	71	},		// Bibarel
		{	37,	25,	41,	25,	41,	25	},		// Kricketot
		{	77,	85,	51,	55,	51,	65	},		// Kricketune
		{	45,	65,	34,	40,	34,	45	},		// Shinx
		{	60,	85,	49,	60,	49,	60	},		// Luxio
		{	80,	120,	79,	95,	79,	70	},		// Luxray
		{	40,	30,	35,	50,	70,	55	},		// Budew
		{	60,	70,	55,	125,	105,	90	},		// Roserade
		{	67,	125,	40,	30,	30,	58	},		// Cranidos
		{	97,	165,	60,	65,	50,	58	},		// Rampardos
		{	30,	42,	118,	42,	88,	30	},		// Shieldon
		{	60,	52,	168,	47,	138,	30	},		// Bastiodon
		{	40,	29,	45,	29,	45,	36	},		// Burmy
		{	60,	59,	85,	79,	105,	36	},		// Wormadam-Plant
		{	60,	79,	105,	59,	85,	36	},		// Wormadam-Sandy
		{	60,	69,	95,	69,	95,	36	},		// Wormadam-Trash
		{	70,	94,	50,	94,	50,	66	},		// Mothim
		{	30,	30,	42,	30,	42,	70	},		// Combee
		{	70,	80,	102,	80,	102,	40	},		// Vespiquen
		{	60,	45,	70,	45,	90,	95	},		// Pachirisu
		{	55,	65,	35,	60,	30,	85	},		// Buizel
		{	85,	105,	55,	85,	50,	115	},		// Floatzel
		{	45,	35,	45,	62,	53,	35	},		// Cherubi
		{	70,	60,	70,	87,	78,	85	},		// Cherrim
		{	76,	48,	48,	57,	62,	34	},		// Shellos
		{	111,	83,	68,	92,	82,	39	},		// Gastrodon
		{	75,	100,	66,	60,	66,	115	},		// Ambipom
		{	90,	50,	34,	60,	44,	70	},		// Drifloon
		{	150,	80,	44,	90,	54,	80	},		// Drifblim
		{	55,	66,	44,	44,	56,	85	},		// Buneary
		{	65,	76,	84,	54,	96,	105	},		// Lopunny
		{	60,	60,	60,	105,	105,	105	},		// Mismagius
		{	100,	125,	52,	105,	52,	71	},		// Honchkrow
		{	49,	55,	42,	42,	37,	85	},		// Glameow
		{	71,	82,	64,	64,	59,	112	},		// Purugly
		{	45,	30,	50,	65,	50,	45	},		// Chingling
		{	63,	63,	47,	41,	41,	74	},		// Stunky
		{	103,	93,	67,	71,	61,	84	},		// Skuntank
		{	57,	24,	86,	24,	86,	23	},		// Bronzor
		{	67,	89,	116,	79,	116,	33	},		// Bronzong
		{	50,	80,	95,	10,	45,	10	},		// Bonsly
		{	20,	25,	45,	70,	90,	60	},		// Mime Jr.
		{	100,	5, 	5, 	15,	65,	30	},		// Happiny
		{	76,	65,	45,	92,	42,	91	},		// Chatot
		{	50,	92,	108,	92,	108,	35	},		// Spiritomb
		{	58,	70,	45,	40,	45,	42	},		// Gible
		{	68,	90,	65,	50,	55,	82	},		// Gabite
		{	108,	130,	95,	80,	85,	102	},		// Garchomp
		{	135,	85,	40,	40,	85,	5  	},		// Munchlax
		{	40,	70,	40,	35,	40,	60	},		// Riolu
		{	70,	110,	70,	115,	70,	90	},		// Lucario
		{	68,	72,	78,	38,	42,	32	},		// Hippopotas
		{	108,	112,	118,	68,	72,	47	},		// Hippowdon
		{	40,	50,	90,	30,	55,	65	},		// Skorupi
		{	70,	90,	110,	60,	75,	95	},		// Drapion
		{	48,	61,	40,	61,	40,	50	},		// Croagunk
		{	83,	106,	65,	86,	65,	85	},		// Toxicroak
		{	74,	100,	72,	90,	72,	46	},		// Carnivine
		{	49,	49,	56,	49,	61,	66	},		// Finneon
		{	69,	69,	76,	69,	86,	91	},		// Lumineon
		{	45,	20,	50,	60,	120,	50	},		// Mantyke
		{	60,	62,	50,	62,	60,	40	},		// Snover
		{	90,	92,	75,	92,	85,	60	},		// Abomasnow
		{	70,	120,	65,	45,	85,	125	},		// Weavile
		{	70,	70,	115,	130,	90,	60	},		// Magnezone
		{	110,	85,	95,	80,	95,	50	},		// Lickilicky
		{	115,	140,	130,	55,	55,	40	},		// Rhyperior
		{	100,	100,	125,	110,	50,	50	},		// Tangrowth
		{	75,	123,	67,	95,	85,	95	},		// Electivire
		{	75,	95,	67,	125,	95,	83	},		// Magmortar
		{	85,	50,	95,	120,	115,	80	},		// Togekiss
		{	86,	76,	86,	116,	56,	95	},		// Yanmega
		{	65,	110,	130,	60,	65,	95	},		// Leafeon
		{	65,	60,	110,	130,	95,	65	},		// Glaceon
		{	75,	95,	125,	45,	75,	95	},		// Gliscor
		{	110,	130,	80,	70,	60,	80	},		// Mamoswine
		{	85,	80,	70,	135,	75,	90	},		// Porygon-Z
		{	68,	125,	65,	65,	115,	80	},		// Gallade
		{	60,	55,	145,	75,	150,	40	},		// Probopass
		{	45,	100,	135,	65,	135,	45	},		// Dusknoir
		{	70,	80,	70,	80,	70,	110	},		// Froslass
		{	50,	50,	77,	95,	77,	91	},		// Rotom
		{	50,	65,	107,	105,	107,	86	},		// Rotom-Heat
		{	50,	65,	107,	105,	107,	86	},		// Rotom-Wash
		{	50,	65,	107,	105,	107,	86	},		// Rotom-Frost
		{	50,	65,	107,	105,	107,	86	},		// Rotom-Fan
		{	50,	65,	107,	105,	107,	86	},		// Rotom-Mow
		{	75,	75,	130,	75,	130,	95	},		// Uxie
		{	80,	105,	105,	105,	105,	80	},		// Mesprit
		{	75,	125,	70,	125,	70,	115	},		// Azelf
		{	100,	120,	120,	150,	100,	90	},		// Dialga
		{	90,	120,	100,	150,	120,	100	},		// Palkia
		{	91,	90,	106,	130,	106,	77	},		// Heatran
		{	110,	160,	110,	80,	110,	100	},		// Regigigas
		{	150,	100,	120,	100,	120,	90	},		// Giratina-Altered
		{	150,	120,	100,	120,	100,	90	},		// Giratina-Origin
		{	120,	70,	120,	75,	130,	85	},		// Cresselia
		{	80,	80,	80,	80,	80,	80	},		// Phione
		{	100,	100,	100,	100,	100,	100	},		// Manaphy
		{	70,	90,	90,	135,	90,	125	},		// Darkrai
		{	100,	100,	100,	100,	100,	100	},		// Shaymin-L
		{	100,	103,	75,	120,	75,	127	},		// Shaymin-S
		{	120,	120,	120,	120,	120,	120	},		// Arceus
		
		// Generation 5
		{	100,	100,	100,	100,	100,	100	},		// Victini
		{	45,	45,	55,	45,	55,	63	},		// Snivy
		{	60,	60,	75,	60,	75,	83	},		// Servine
		{	75,	75,	95,	75,	95,	113	},		// Serperior
		{	65,	63,	45,	45,	45,	45	},		// Tepig
		{	90,	93,	55,	70,	55,	55	},		// Pignite
		{	110,	123,	65,	100,	65,	65	},		// Emboar
		{	55,	55,	45,	63,	45,	45	},		// Oshawott
		{	75,	75,	60,	83,	60,	60	},		// Dewott
		{	95,	100,	85,	108,	70,	70	},		// Samurott
		{	45,	55,	39,	35,	39,	42	},		// Patrat
		{	60,	85,	69,	60,	69,	77	},		// Watchog
		{	45,	60,	45,	25,	45,	55	},		// Lillipup
		{	65,	80,	65,	35,	65,	60	},		// Herdier
		{	85,	100,	90,	45,	90,	80	},		// Stoutland
		{	41,	50,	37,	50,	37,	66	},		// Purrloin
		{	64,	88,	50,	88,	50,	106	},		// Liepard
		{	50,	53,	48,	53,	48,	64	},		// Pansage
		{	75,	98,	63,	98,	63,	101	},		// Simisage
		{	50,	53,	48,	53,	48,	64	},		// Pansear
		{	75,	98,	63,	98,	63,	101	},		// Simisear
		{	50,	53,	48,	53,	48,	64	},		// Panpour
		{	75,	98,	63,	98,	63,	101	},		// Simipour
		{	76,	25,	45,	67,	55,	24	},		// Munna
		{	116,	55,	85,	107,	95,	29	},		// Musharna
		{	50,	55,	50,	36,	30,	43	},		// Pidove
		{	62,	77,	62,	50,	42,	65	},		// Tranquill
		{	80,	105,	80,	65,	55,	93	},		// Unfezant
		{	45,	60,	32,	50,	32,	76	},		// Blitzle
		{	75,	100,	63,	80,	63,	116	},		// Zebstrika
		{	55,	75,	85,	25,	25,	15	},		// Roggenrola
		{	70,	105,	105,	50,	40,	20	},		// Boldore
		{	85,	135,	130,	60,	70,	25	},		// Gigalith
		{	55,	45,	43,	55,	43,	72	},		// Woobat
		{	67,	57,	55,	77,	55,	114	},		// Swoobat
		{	60,	85,	40,	30,	45,	68	},		// Drilbur
		{	110,	135,	60,	50,	65,	88	},		// Excadrill
		{	103,	60,	86,	60,	86,	50	},		// Audino
		{	75,	80,	55,	25,	35,	35	},		// Timburr
		{	85,	105,	85,	40,	50,	40	},		// Gurdurr
		{	105,	140,	95,	55,	65,	45	},		// Conkeldurr
		{	50,	50,	40,	50,	40,	64	},		// Tympole
		{	75,	65,	55,	65,	55,	69	},		// Palpitoad
		{	105,	85,	75,	85,	75,	74	},		// Seismitoad
		{	120,	100,	85,	30,	85,	45	},		// Throh
		{	75,	125,	75,	30,	75,	85	},		// Sawk
		{	45,	53,	70,	40,	60,	42	},		// Sewaddle
		{	55,	63,	90,	50,	80,	42	},		// Swadloon
		{	75,	103,	80,	70,	70,	92	},		// Leavanny
		{	30,	45,	59,	30,	39,	57	},		// Venipede
		{	40,	55,	99,	40,	79,	47	},		// Whirlipede
		{	60,	90,	89,	55,	69,	112	},		// Scolipede
		{	40,	27,	60,	37,	50,	66	},		// Cottonee
		{	60,	67,	85,	77,	75,	116	},		// Whimsicott
		{	45,	35,	50,	70,	50,	30	},		// Petilil
		{	70,	60,	75,	110,	75,	90	},		// Lilligant
		{	70,	92,	65,	80,	55,	98	},		// Basculin-R
		{	70,	92,	65,	80,	55,	98	},		// Basculin-B
		{	50,	72,	35,	35,	35,	65	},		// Sandile
		{	60,	82,	45,	45,	45,	74	},		// Krokorok
		{	95,	117,	70,	65,	70,	92	},		// Krookodile
		{	70,	90,	45,	15,	45,	50	},		// Darumaka
		{	105,	140,	55,	30,	55,	95	},		// Darmanitan
	//	{	105,	30,	105,	140,	105,	55	},		// Darmanitan (Zen Mode)
		{	75,	86,	67,	106,	67,	60	},		// Maractus
		{	50,	65,	85,	35,	35,	55	},		// Dwebble
		{	70,	95,	125,	65,	75,	45	},		// Crustle
		{	50,	75,	70,	35,	70,	48	},		// Scraggy
		{	65,	90,	115,	45,	115,	58	},		// Scrafty
		{	72,	58,	80,	103,	80,	97	},		// Sigilyph
		{	38,	30,	85,	55,	65,	30	},		// Yamask
		{	58,	50,	145,	95,	105,	30	},		// Cofagrigus
		{	54,	78,	103,	53,	45,	22	},		// Tirtouga
		{	74,	108,	133,	83,	65,	32	},		// Carracosta
		{	55,	112,	45,	74,	45,	70	},		// Archen
		{	75,	140,	65,	112,	65,	110	},		// Archeops
		{	50,	50,	62,	40,	62,	65	},		// Trubbish
		{	80,	95,	82,	60,	82,	75	},		// Garbodor
		{	40,	65,	40,	80,	40,	65	},		// Zorua
		{	60,	105,	60,	120,	60,	105	},		// Zoroark
		{	55,	50,	40,	40,	40,	75	},		// Minccino
		{	75,	95,	60,	65,	60,	115	},		// Cinccino
		{	45,	30,	50,	55,	65,	45	},		// Gothita
		{	60,	45,	70,	75,	85,	55	},		// Gothorita
		{	70,	55,	95,	95,	110,	65	},		// Gothitelle
		{	45,	30,	40,	105,	50,	20	},		// Solosis
		{	65,	40,	50,	125,	60,	30	},		// Duosion
		{	110,	65,	75,	125,	85,	30	},		// Reuniclus
		{	62,	44,	50,	44,	50,	55	},		// Ducklett
		{	75,	87,	63,	87,	63,	98	},		// Swanna
		{	36,	50,	50,	65,	60,	44	},		// Vanillite
		{	51,	65,	65,	80,	75,	59	},		// Vanillish
		{	71,	95,	85,	110,	95,	79	},		// Vanilluxe
		{	60,	60,	50,	40,	50,	75	},		// Deerling
		{	80,	100,	70,	60,	70,	95	},		// Sawsbuck
		{	55,	75,	60,	75,	60,	103	},		// Emolga
		{	50,	75,	45,	40,	45,	60	},		// Karrablast
		{	70,	135,	105,	60,	105,	20	},		// Escavalier
		{	69,	55,	45,	55,	55,	15	},		// Foongus
		{	114,	85,	70,	85,	80,	30	},		// Amoonguss
		{	55,	40,	50,	65,	85,	40	},		// Frillish
		{	100,	60,	70,	85,	105,	60	},		// Jellicent
		{	165,	75,	80,	40,	45,	65	},		// Alomomola
		{	50,	47,	50,	57,	50,	65	},		// Joltik
		{	70,	77,	60,	97,	60,	108	},		// Galvantula
		{	44,	50,	91,	24,	86,	10	},		// Ferroseed
		{	74,	94,	131,	54,	116,	20	},		// Ferrothorn
		{	40,	55,	70,	45,	60,	30	},		// Klink
		{	60,	80,	95,	70,	85,	50	},		// Klang
		{	60,	100,	115,	70,	85,	90	},		// Klinklang
		{	35,	55,	40,	45,	40,	60	},		// Tynamo
		{	65,	85,	70,	75,	70,	40	},		// Eelektrik
		{	85,	115,	80,	105,	80,	50	},		// Eelektross
		{	55,	55,	55,	85,	55,	30	},		// Elgyem
		{	75,	75,	75,	125,	95,	40	},		// Beheeyem
		{	50,	30,	55,	65,	55,	20	},		// Litwick
		{	60,	40,	60,	95,	60,	55	},		// Lampent
		{	60,	55,	90,	145,	90,	80	},		// Chandelure
		{	46,	87,	60,	30,	40,	57	},		// Axew
		{	66,	117,	70,	40,	50,	67	},		// Fraxure
		{	76,	147,	90,	60,	70,	97	},		// Haxorus
		{	55,	70,	40,	60,	40,	40	},		// Cubchoo
		{	95,	110,	80,	70,	80,	50	},		// Beartic
		{	70,	50,	30,	95,	135,	105	},		// Cryogonal
		{	50,	40,	85,	40,	65,	25	},		// Shelmet
		{	80,	70,	40,	100,	60,	145	},		// Accelgor
		{	109,	66,	84,	81,	99,	32	},		// Stunfisk
		{	45,	85,	50,	55,	50,	65	},		// Mienfoo
		{	65,	125,	60,	95,	60,	105	},		// Mienshao
		{	77,	120,	90,	60,	90,	48	},		// Druddigon
		{	59,	74,	50,	35,	50,	35	},		// Golett
		{	89,	124,	80,	55,	80,	55	},		// Golurk
		{	45,	85,	70,	40,	40,	60	},		// Pawniard
		{	65,	125,	100,	60,	70,	70	},		// Bisharp
		{	95,	110,	95,	40,	95,	55	},		// Bouffalant
		{	70,	83,	50,	37,	50,	60	},		// Rufflet
		{	100,	123,	75,	57,	75,	80	},		// Braviary
		{	70,	55,	75,	45,	65,	60	},		// Vullaby
		{	110,	65,	105,	55,	95,	80	},		// Mandibuzz
		{	85,	97,	66,	105,	66,	65	},		// Heatmor
		{	58,	109,	112,	48,	48,	109	},		// Durant
		{	52,	65,	50,	45,	50,	38	},		// Deino
		{	72,	85,	70,	65,	70,	58	},		// Zweilous
		{	92,	105,	90,	125,	90,	98	},		// Hydreigon
		{	55,	85,	55,	50,	55,	60	},		// Larvesta
		{	85,	60,	65,	135,	105,	100	},		// Volcarona
		{	91,	90,	129,	90,	72,	108	},		// Cobalion
		{	91,	129,	90,	72,	90,	108	},		// Terrakion
		{	91,	90,	72,	90,	129,	108	},		// Virizion
		{	79,	115,	70,	125,	80,	111	},		// Tornadus-I
		{	79,	100,	80,	110,	90,	121	},		// Tornadus-T
		{	79,	115,	70,	125,	80,	111	},		// Thundurus-I
		{	79,	105,	70,	145,	80,	101	},		// Thundurus-T
		{	100,	120,	100,	150,	120,	90	},		// Reshiram
		{	100,	150,	120,	120,	100,	90	},		// Zekrom
		{	89,	125,	90,	115,	80,	101	},		// Landorus-I
		{	89,	145,	90,	105,	80,	91	},		// Landorus-T
		{	125,	130,	90,	130,	90,	95	},		// Kyurem
		{	125,	170,	100,	120,	90,	95	},		// Kyurem-B
		{	125,	120,	90,	170,	100,	95	},		// Kyurem-W
		{	91,	72,	90,	129,	90,	108	},		// Keldeo
		{	100,	77,	77,	128,	128,	90	},		// Meloetta
	//	{	100,	128,	90,	77,	77,	128	},		// Meloetta (Pirouette form)
		{	71,	120,	95,	120,	95,	99	}		// Genesect 
	};
	// I add 1 because HP is -1 to allow every other index to be easier.
	return base_stat [static_cast<size_t>(name)] [stat + 1];
}

}	// unnamed namespace
}	// namespace technicalmachine
