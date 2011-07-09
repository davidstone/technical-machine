// Stat formulas
// Copyright 2011 David Stone
//
// This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public License
// as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "ability.h"
#include "item.h"
#include "move.h"
#include "pokemon.h"
#include "stat.h"
#include "status.h"
#include "team.h"
#include "weather.h"

namespace technicalmachine {

Stat::Stat (species name, int level, stats stat) :
	base (base_stat [name] [stat]),
	iv (31),
	ev (21),		// Adds up to 126 points, temporary until I add in EV prediction
	stage (0),
	max (-1) {
	}

int hitpoints (Pokemon &member) {
	if (1 == member.hp.base)
		return 1;
	return (2 * member.hp.base + member.hp.iv + member.hp.ev) * member.level / 100 + member.level + 10;
}

void attack (Team &attacker, Weather const &weather) {
	if (attacker.pokemon->move->physical) {
		attacker.pokemon->atk.stat = (2 * attacker.pokemon->atk.base + attacker.pokemon->atk.iv + attacker.pokemon->atk.ev) * attacker.pokemon->level / 100 + 5;
		if (ADAMANT == attacker.pokemon->nature or BRAVE == attacker.pokemon->nature or LONELY == attacker.pokemon->nature or NAUGHTY == attacker.pokemon->nature)
			attacker.pokemon->atk.stat = attacker.pokemon->atk.stat * 11 / 10;
		else if (BOLD == attacker.pokemon->nature or CALM == attacker.pokemon->nature or MODEST == attacker.pokemon->nature or TIMID == attacker.pokemon->nature)
			attacker.pokemon->atk.stat = attacker.pokemon->atk.stat * 9 / 10;

		if (attacker.pokemon->atk.stage >= 0) // >= is better than == to reduce the frequency of checking for a CH
			attacker.pokemon->atk.stat = attacker.pokemon->atk.stat * (2 + attacker.pokemon->atk.stage) / 2;
		else {
			if (!attacker.ch)
				attacker.pokemon->atk.stat = attacker.pokemon->atk.stat * 2 / (2 - attacker.pokemon->atk.stage);
		}

		if (attacker.slow_start != 0)
			attacker.pokemon->atk.stat /= 2;
		else if ((FLOWER_GIFT == attacker.pokemon->ability and 0 != weather.sun) or (GUTS == attacker.pokemon->ability and NO_STATUS != attacker.pokemon->status) or (HUSTLE == attacker.pokemon->ability))
			attacker.pokemon->atk.stat = attacker.pokemon->atk.stat * 3 / 2;
		else if (PURE_POWER == attacker.pokemon->ability)
			attacker.pokemon->atk.stat *= 2;
	
		if (CHOICE_BAND == attacker.pokemon->item)
			attacker.pokemon->atk.stat = attacker.pokemon->atk.stat * 3 / 2;
		else if ((LIGHT_BALL == attacker.pokemon->item and PIKACHU == attacker.pokemon->name) or (THICK_CLUB == attacker.pokemon->item and (CUBONE == attacker.pokemon->name or MAROWAK == attacker.pokemon->name)))
			attacker.pokemon->atk.stat *= 2;
		
		if (attacker.pokemon->atk.stat == 0)
			attacker.pokemon->atk.stat = 1;
	}
	else {
		attacker.pokemon->spa.stat = (2 * attacker.pokemon->spa.base + attacker.pokemon->spa.iv + attacker.pokemon->spa.ev) * attacker.pokemon->level / 100 + 5;
		if (MILD == attacker.pokemon->nature or MODEST == attacker.pokemon->nature or QUIET == attacker.pokemon->nature or RASH == attacker.pokemon->nature)
			attacker.pokemon->spa.stat = attacker.pokemon->spa.stat * 11 / 10;
		else if (ADAMANT == attacker.pokemon->nature or CAREFUL == attacker.pokemon->nature or IMPISH == attacker.pokemon->nature or JOLLY == attacker.pokemon->nature)
			attacker.pokemon->spa.stat = attacker.pokemon->spa.stat * 9 / 10;

		if (attacker.pokemon->spa.stage >= 0)	// >= is better than == to reduce the frequency of checking for a CH
			attacker.pokemon->spa.stat = attacker.pokemon->spa.stat * (2 + attacker.pokemon->spa.stage) / 2;
		else {
			if (!attacker.ch)
				attacker.pokemon->spa.stat = attacker.pokemon->spa.stat * 2 / (2 - attacker.pokemon->spa.stage);
		}

		if (SOLAR_POWER == attacker.pokemon->ability and 0 != weather.sun)
			attacker.pokemon->spa.stat = attacker.pokemon->spa.stat * 3 / 2;

		if ((CHOICE_SPECS == attacker.pokemon->item) or (SOUL_DEW == attacker.pokemon->item and (LATIAS == attacker.pokemon->name or LATIOS == attacker.pokemon->name)))
			attacker.pokemon->spa.stat = attacker.pokemon->spa.stat * 3 / 2;
		else if ((DEEPSEATOOTH == attacker.pokemon->item and CLAMPERL == attacker.pokemon->name) or (LIGHT_BALL == attacker.pokemon->item and PIKACHU == attacker.pokemon->name))
			attacker.pokemon->spa.stat *= 2;

		if (attacker.pokemon->spa.stat == 0)
			attacker.pokemon->spa.stat = 1;
	}
}

void defense (Team const &attacker, Team &defender, Weather const &weather) {
	if (attacker.pokemon->move->physical) {
		defender.pokemon->def.stat = (2 * defender.pokemon->def.base + defender.pokemon->def.iv + defender.pokemon->def.ev) * defender.pokemon->level / 100 + 5;
		if (BOLD == defender.pokemon->nature or IMPISH == defender.pokemon->nature or LAX == defender.pokemon->nature or RELAXED == defender.pokemon->nature)
			defender.pokemon->def.stat = defender.pokemon->def.stat * 11 / 10;
		else if (GENTLE == defender.pokemon->nature or HASTY == defender.pokemon->nature or LONELY == defender.pokemon->nature or MILD == defender.pokemon->nature)
			defender.pokemon->def.stat = defender.pokemon->def.stat * 9 / 10;

		if (defender.pokemon->def.stage > 0) {	// > is better than >= to reduce the frequency of checking for a CH
			if (!attacker.ch)
				defender.pokemon->def.stat = defender.pokemon->def.stat * (2 + defender.pokemon->def.stage) / 2;
		}
		else
			defender.pokemon->def.stat = defender.pokemon->def.stat * 2 / (2 - defender.pokemon->def.stage);

		if (MARVEL_SCALE == defender.pokemon->ability and NO_STATUS != defender.pokemon->status)
			defender.pokemon->def.stat = defender.pokemon->def.stat * 3 / 2;
		
		if (METAL_POWDER == defender.pokemon->item and DITTO == defender.pokemon->name)
			defender.pokemon->def.stat = defender.pokemon->def.stat * 3 / 2;
		
		if (EXPLOSION == attacker.pokemon->move->name or SELFDESTRUCT == attacker.pokemon->move->name)
			defender.pokemon->def.stat /= 2;
	
		if (0 == defender.pokemon->def.stat)
			defender.pokemon->def.stat = 1;
	}
	else {
		defender.pokemon->spd.stat = (2 * defender.pokemon->spd.base + defender.pokemon->spd.iv + defender.pokemon->spd.ev) * defender.pokemon->level / 100 + 5;
		if (CALM == defender.pokemon->nature or CAREFUL == defender.pokemon->nature or GENTLE == defender.pokemon->nature or SASSY == defender.pokemon->nature)
			defender.pokemon->spd.stat = defender.pokemon->spd.stat * 11 / 10;
		else if (LAX == defender.pokemon->nature or NAIVE == defender.pokemon->nature or NAUGHTY == defender.pokemon->nature or RASH == defender.pokemon->nature)
			defender.pokemon->spd.stat = defender.pokemon->spd.stat * 9 / 10;

		if (defender.pokemon->spd.stage > 0) {	// > is better than >= to reduce the frequency of checking for a CH
			if (!attacker.ch)
				defender.pokemon->spd.stat = defender.pokemon->spd.stat * (2 + defender.pokemon->spd.stage) / 2;
		}
		else
			defender.pokemon->spd.stat = defender.pokemon->spd.stat * 2 / (2 - defender.pokemon->spd.stage);

		if (FLOWER_GIFT == defender.pokemon->ability and 0 != weather.sun)
			defender.pokemon->spd.stat = defender.pokemon->spd.stat * 3 / 2;
		
		if (DEEPSEASCALE == defender.pokemon->item and CLAMPERL == defender.pokemon->name)
			defender.pokemon->spd.stat *= 2;
		else if ((METAL_POWDER == defender.pokemon->item and DITTO == defender.pokemon->name) or (SOUL_DEW == defender.pokemon->item and (LATIAS == defender.pokemon->name or LATIOS == defender.pokemon->name)))
			defender.pokemon->spd.stat = defender.pokemon->spd.stat * 3 / 2;
		
		if (istype (defender, ROCK) and 0 != weather.sand)
			defender.pokemon->spd.stat = defender.pokemon->spd.stat * 3 / 2;
		
		if (0 == defender.pokemon->spd.stat)
			defender.pokemon->spd.stat = 1;
	}
}

void speed (Team &team, Weather const &weather) {
	team.pokemon->spe.stat = (2 * team.pokemon->spe.base + team.pokemon->spe.iv + team.pokemon->spe.ev) * team.pokemon->level / 100 + 5;

	if (HASTY == team.pokemon->nature or JOLLY == team.pokemon->nature or NAIVE == team.pokemon->nature or TIMID == team.pokemon->nature)
		team.pokemon->spe.stat = team.pokemon->spe.stat * 11 / 10;
	else if (BRAVE == team.pokemon->nature or QUIET == team.pokemon->nature or RELAXED == team.pokemon->nature or SASSY == team.pokemon->nature)
		team.pokemon->spe.stat = team.pokemon->spe.stat * 9 / 10;
	
	if (team.pokemon->spe.stage >= 0)
		team.pokemon->spe.stat = team.pokemon->spe.stat * (2 + team.pokemon->spe.stage) / 2;
	else
		team.pokemon->spe.stat = team.pokemon->spe.stat * 2 / (2 - team.pokemon->spe.stage);

	if ((CHLOROPHYLL == team.pokemon->ability and 0 != weather.sun) or (SWIFT_SWIM == team.pokemon->ability and 0 != weather.rain) or (UNBURDEN == team.pokemon->ability and false))
		team.pokemon->spe.stat *= 2;
	else if (QUICK_FEET == team.pokemon->ability and NO_STATUS != team.pokemon->status)
		team.pokemon->spe.stat = team.pokemon->spe.stat * 3 / 2;
	else if (0 != team.slow_start)
		team.pokemon->spe.stat /= 2;
		
	if (QUICK_POWDER == team.pokemon->item and DITTO == team.pokemon->name)
		team.pokemon->spe.stat *= 2;
	else if (CHOICE_SCARF == team.pokemon->item)
		team.pokemon->spe.stat = team.pokemon->spe.stat * 3 / 2;
	else if (MACHO_BRACE == team.pokemon->item or POWER_ITEMS == team.pokemon->item)
		team.pokemon->spe.stat /= 2;
		
	if (PARALYSIS == team.pokemon->status and QUICK_FEET != team.pokemon->ability)
		team.pokemon->spe.stat /= 4;
	
	if (team.tailwind)
		team.pokemon->spe.stat *= 2;

	if (team.pokemon->spe.stat == 0)
		team.pokemon->spe.stat = 1;
}

void order (Team &team1, Team &team2, Weather const &weather, Team* &faster, Team* &slower) {
	if (team1.pokemon->move->priority == team2.pokemon->move->priority) {
		speed (team1, weather);
		speed (team2, weather);
		faster_pokemon (team1, team2, weather, faster, slower);
	}
	else if (team1.pokemon->move->priority > team2.pokemon->move->priority) {
		faster = &team1;
		slower = &team2;
	}
	else {				// (team2.pokemon->move->priority > team1.pokemon->move->priority)
		faster = &team2;
		slower = &team1;
	}
}

void faster_pokemon (Team &team1, Team &team2, Weather const &weather, Team* &faster, Team* &slower) {
	if (team1.pokemon->spe.stat > team2.pokemon->spe.stat) {
		faster = &team1;
		slower = &team2;
	}
	else if (team2.pokemon->spe.stat > team1.pokemon->spe.stat) {
		faster = &team2;
		slower = &team1;
	}
	else {				// All things are equal
		faster = NULL;
		slower = NULL;
	}
	if (weather.trick_room != 0)
		std::swap (faster, slower);
}

void chance_to_hit (Team &user, Team const &target, Weather const &weather) {
	if (user.pokemon->move->accuracy == -1)
		user.chance_to_hit = 100;
	else {
		user.chance_to_hit = user.pokemon->move->accuracy;
		if (user.accuracy >= 0)
			user.chance_to_hit = user.chance_to_hit * (3 + user.accuracy) / 3;
		else
			user.chance_to_hit = user.chance_to_hit * 3 / (3 - user.accuracy);
		if (target.evasion <= 0)
			user.chance_to_hit = user.chance_to_hit * (3 - target.evasion) / 3;
		else
			user.chance_to_hit = user.chance_to_hit * 3 / (3 + target.evasion);
		if (user.pokemon->item == WIDE_LENS)
			user.chance_to_hit = user.chance_to_hit * 11 / 10;
		else if (user.pokemon->item == ZOOM_LENS and target.moved)
			user.chance_to_hit = user.chance_to_hit * 6 / 5;
		if (target.pokemon->item == BRIGHTPOWDER)
			user.chance_to_hit = user.chance_to_hit * 9 / 10;
		else if (target.pokemon->item == LAX_INCENSE)
			user.chance_to_hit = user.chance_to_hit * 19 / 20;
		if (user.pokemon->ability == COMPOUNDEYES)
			user.chance_to_hit = user.chance_to_hit * 13 / 10;
		else if (user.pokemon->ability == HUSTLE and user.pokemon->move->physical)
			user.chance_to_hit = user.chance_to_hit * 4 / 5;
		if ((target.pokemon->ability == SAND_VEIL and weather.sand != 0)
				or (target.pokemon->ability == SNOW_CLOAK and weather.hail != 0)
				or (target.pokemon->ability == TANGLED_FEET and target.confused != 0))
			user.chance_to_hit = user.chance_to_hit * 4 / 5;
		if (weather.gravity != 0)
			user.chance_to_hit = user.chance_to_hit * 5 / 3;
		if (user.chance_to_hit > 100)
			user.chance_to_hit = 100;
	}
	user.chance_to_hit = user.chance_to_hit;
}

void Stat::boost (int n) {
	stage += n;
	if (stage > 6)
		stage = 6;
	else if (stage < -6)
		stage = -6;
}

unsigned char const Stat::base_stat [][6] = {
	{ 90, 92, 75, 92, 85, 60 },			// Abomasnow
	{ 25, 20, 15, 105, 55, 90 },		// Abra	
	{ 65, 130, 60, 75, 60, 75 },		// Absol	
	{ 80, 105, 65, 60, 75, 130 },		// Aerodactyl	
	{ 70, 110, 180, 60, 60, 50 },		// Aggron	
	{ 55, 70, 55, 40, 55, 85 },			// Aipom
	{ 55, 50, 45, 135, 85, 120 },		// Alakazam	
	{ 75, 70, 90, 70, 105, 80 },		// Altaria	
	{ 75, 100, 66, 60, 66, 115 },		// Ambipom	
	{ 90, 75, 75, 115, 90, 55 },		// Ampharos	
	{ 45, 95, 50, 40, 50, 75 },			// Anorith
	{ 60, 85, 69, 65, 79, 80 },			// Arbok
	{ 90, 110, 80, 100, 80, 95 },		// Arcanine	
	{ 120, 120, 120, 120, 120, 120 },	// Arceus		
	{ 70, 90, 70, 60, 60, 40 },			// Ariados
	{ 75, 125, 100, 70, 80, 45 },		// Armaldo	
	{ 50, 70, 100, 40, 40, 30 },		// Aron	
	{ 90, 85, 100, 95, 125, 85 },		// Articuno	
	{ 75, 125, 70, 125, 70, 115 },		// Azelf	
	{ 100, 50, 80, 50, 80, 50 },		// Azumarill	
	{ 50, 20, 40, 20, 40, 20 },			// Azurill
	{ 45, 75, 60, 40, 30, 50 },			// Bagon
	{ 40, 40, 55, 40, 70, 55 },			// Baltoy
	{ 64, 115, 65, 83, 63, 65 },		// Banette	
	{ 50, 48, 43, 46, 41, 60 },			// Barboach
	{ 60, 52, 168, 47, 138, 30 },		// Bastiodon	
	{ 60, 62, 80, 63, 80, 60 },			// Bayleef
	{ 60, 70, 50, 90, 50, 65 },			// Beautifly
	{ 65, 80, 40, 45, 80, 75 },			// Beedrill
	{ 40, 55, 80, 35, 60, 30 },			// Beldum
	{ 75, 80, 85, 90, 100, 50 },		// Bellossom	
	{ 50, 75, 35, 70, 30, 40 },			// Bellsprout
	{ 79, 85, 60, 55, 60, 71 },			// Bibarel
	{ 59, 45, 40, 35, 40, 31 },			// Bidoof
	{ 79, 83, 100, 85, 105, 78 },		// Blastoise	
	{ 80, 120, 70, 110, 70, 80 },		// Blaziken	
	{ 255, 10, 10, 75, 135, 55 },		// Blissey	
	{ 50, 80, 95, 10, 45, 10 },			// Bonsly
	{ 60, 130, 80, 60, 60, 70 },		// Breloom	
	{ 67, 89, 116, 79, 116, 33 },		// Bronzong	
	{ 57, 24, 86, 24, 86, 23 },			// Bronzor
	{ 40, 30, 35, 50, 70, 55 },			// Budew
	{ 55, 65, 35, 60, 30, 85 },			// Buizel
	{ 45, 49, 49, 65, 65, 45 },			// Bulbasaur
	{ 55, 66, 44, 44, 56, 85 },			// Buneary
	{ 40, 29, 45, 29, 45, 36 },			// Burmy
	{ 60, 45, 50, 80, 80, 70 },			// Butterfree
	{ 50, 85, 40, 85, 40, 35 },			// Cacnea
	{ 70, 115, 60, 115, 60, 55 },		// Cacturne	
	{ 70, 100, 70, 105, 75, 40 },		// Camerupt	
	{ 74, 100, 72, 90, 72, 46 },		// Carnivine	
	{ 45, 90, 20, 65, 20, 65 },			// Carvanha
	{ 50, 35, 55, 25, 25, 15 },			// Cascoon
	{ 70, 70, 70, 70, 70, 70 },			// Castform
	{ 45, 30, 35, 20, 20, 45 },			// Caterpie
	{ 100, 100, 100, 100, 100, 100 },	// Celebi		
	{ 250, 5, 5, 35, 105, 50 },			// Chansey
	{ 78, 84, 78, 109, 85, 100 },		// Charizard	
	{ 39, 52, 43, 60, 50, 65 },			// Charmander
	{ 58, 64, 58, 80, 65, 80 },			// Charmeleon
	{ 76, 65, 45, 92, 42, 91 },			// Chatot
	{ 70, 60, 70, 87, 78, 85 },			// Cherrim
	{ 45, 35, 45, 62, 53, 35 },			// Cherubi
	{ 45, 49, 65, 49, 65, 45 },			// Chikorita
	{ 44, 58, 44, 58, 44, 61 },			// Chimchar
	{ 65, 50, 70, 95, 80, 65 },			// Chimecho
	{ 75, 38, 38, 56, 56, 67 },			// Chinchou
	{ 45, 30, 50, 65, 50, 45 },			// Chingling
	{ 35, 64, 85, 74, 55, 32 },			// Clamperl
	{ 60, 70, 105, 70, 120, 75 },		// Claydol	
	{ 95, 70, 73, 85, 90, 60 },			// Clefable
	{ 70, 45, 48, 60, 65, 35 },			// Clefairy
	{ 50, 25, 28, 45, 55, 15 },			// Cleffa
	{ 50, 95, 180, 85, 45, 70 },		// Cloyster	
	{ 30, 30, 42, 30, 42, 70 },			// Combee
	{ 60, 85, 60, 85, 60, 55 },			// Combusken
	{ 43, 80, 65, 50, 35, 35 },			// Corphish
	{ 55, 55, 85, 65, 85, 35 },			// Corsola
	{ 86, 81, 97, 81, 107, 43 },		// Cradily	
	{ 67, 125, 40, 30, 30, 58 },		// Cranidos	
	{ 63, 120, 85, 90, 55, 55 },		// Crawdaunt	
	{ 120, 70, 120, 75, 130, 85 },		// Cresselia	
	{ 48, 61, 40, 61, 40, 50 },			// Croagunk
	{ 85, 90, 80, 70, 80, 130 },		// Crobat	
	{ 65, 80, 80, 59, 63, 58 },			// Croconaw
	{ 50, 50, 95, 40, 50, 35 },			// Cubone
	{ 39, 52, 43, 60, 50, 65 },			// Cyndaquil
	{ 70, 90, 90, 135, 90, 125 },		// Darkrai	
	{ 70, 65, 65, 55, 55, 70 },			// Delcatty
	{ 45, 55, 45, 65, 45, 75 },			// Delibird
	{ 50, 180, 20, 180, 20, 150 },		// Deoxys-A	
	{ 50, 70, 160, 70, 160, 90 },		// Deoxys-D	
	{ 50, 150, 50, 150, 50, 150 },		// Deoxys-M	
	{ 50, 95, 90, 95, 90, 180 },		// Deoxys-S	
	{ 90, 70, 80, 70, 95, 70 },			// Dewgong
	{ 100, 120, 120, 150, 100, 90 },	// Dialga		
	{ 10, 55, 25, 35, 45, 95 },			// Diglett
	{ 48, 48, 48, 48, 48, 48 },			// Ditto
	{ 60, 110, 70, 60, 60, 100 },		// Dodrio	
	{ 35, 85, 45, 35, 35, 75 },			// Doduo
	{ 90, 120, 120, 60, 60, 50 },		// Donphan	
	{ 61, 84, 65, 70, 70, 70 },			// Dragonair
	{ 91, 134, 95, 100, 100, 80 },		// Dragonite	
	{ 70, 90, 110, 60, 75, 95 },		// Drapion	
	{ 41, 64, 45, 50, 50, 50 },			// Dratini
	{ 150, 80, 44, 90, 54, 80 },		// Drifblim	
	{ 90, 50, 34, 60, 44, 70 },			// Drifloon
	{ 60, 48, 45, 43, 90, 42 },			// Drowzee
	{ 35, 80, 50, 50, 70, 120 },		// Dugtrio	
	{ 100, 70, 70, 65, 65, 45 },		// Dunsparce	
	{ 40, 70, 130, 60, 130, 25 },		// Dusclops	
	{ 45, 100, 135, 65, 135, 45 },		// Dusknoir	
	{ 20, 40, 90, 30, 90, 25 },			// Duskull
	{ 60, 50, 70, 50, 90, 65 },			// Dustox
	{ 55, 55, 50, 45, 65, 55 },			// Eevee
	{ 35, 60, 44, 40, 54, 55 },			// Ekans
	{ 65, 83, 57, 95, 85, 105 },		// Electabuzz	
	{ 75, 123, 67, 95, 85, 95 },		// Electivire	
	{ 40, 45, 40, 65, 40, 65 },			// Electrike
	{ 60, 50, 70, 80, 80, 140 },		// Electrode	
	{ 45, 63, 37, 65, 55, 95 },			// Elekid
	{ 84, 86, 88, 111, 101, 60 },		// Empoleon	
	{ 115, 115, 85, 90, 75, 100 },		// Entei	
	{ 65, 65, 60, 130, 95, 110 },		// Espeon	
	{ 60, 40, 80, 60, 45, 40 },			// Exeggcute
	{ 95, 95, 85, 125, 65, 55 },		// Exeggutor	
	{ 104, 91, 63, 91, 63, 68 },		// Exploud	
	{ 52, 65, 55, 58, 62, 60 },			// Farfetch'd
	{ 65, 90, 65, 61, 61, 100 },		// Fearow	
	{ 20, 15, 20, 10, 55, 80 },			// Feebas
	{ 85, 105, 100, 79, 83, 78 },		// Feraligatr	
	{ 49, 49, 56, 49, 61, 66 },			// Finneon
	{ 70, 55, 55, 80, 60, 45 },			// Flaaffy
	{ 65, 130, 60, 95, 110, 65 },		// Flareon	
	{ 85, 105, 55, 85, 50, 115 },		// Floatzel	
	{ 80, 100, 80, 80, 80, 100 },		// Flygon	
	{ 75, 90, 140, 60, 60, 40 },		// Forretress	
	{ 70, 80, 70, 80, 70, 110 },		// Froslass	
	{ 85, 76, 64, 45, 55, 90 },			// Furret
	{ 68, 90, 65, 50, 55, 82 },			// Gabite
	{ 68, 125, 65, 65, 115, 80 },		// Gallade	
	{ 108, 130, 95, 80, 85, 102 },		// Garchomp	
	{ 68, 65, 65, 125, 115, 80 },		// Gardevoir	
	{ 30, 35, 30, 100, 35, 80 },		// Gastly	
	{ 111, 83, 68, 92, 82, 39 },		// Gastrodon	
	{ 60, 65, 60, 130, 75, 110 },		// Gengar	
	{ 40, 80, 100, 30, 30, 20 },		// Geodude	
	{ 58, 70, 45, 40, 45, 42 },			// Gible
	{ 70, 80, 65, 90, 65, 85 },			// Girafarig
	{ 150, 100, 120, 100, 120, 90 },	// Giratina-A		
	{ 150, 120, 100, 120, 100, 90 },	// Giratina-O		
	{ 65, 60, 110, 130, 95, 65 },		// Glaceon	
	{ 80, 80, 80, 80, 80, 80 },			// Glalie
	{ 49, 55, 42, 42, 37, 85 },			// Glameow
	{ 65, 75, 105, 35, 65, 85 },		// Gligar	
	{ 75, 95, 125, 45, 75, 95 },		// Gliscor	
	{ 60, 65, 70, 85, 75, 40 },			// Gloom
	{ 75, 80, 70, 65, 75, 90 },			// Golbat
	{ 45, 67, 60, 35, 50, 63 },			// Goldeen
	{ 80, 82, 78, 95, 80, 85 },			// Golduck
	{ 80, 110, 130, 55, 65, 45 },		// Golem	
	{ 55, 84, 105, 114, 75, 52 },		// Gorebyss	
	{ 90, 120, 75, 60, 60, 45 },		// Granbull	
	{ 55, 95, 115, 45, 45, 35 },		// Graveler	
	{ 80, 80, 50, 40, 50, 25 },			// Grimer
	{ 75, 89, 85, 55, 65, 36 },			// Grotle
	{ 100, 150, 140, 100, 90, 90 },		// Groudon	
	{ 50, 65, 45, 85, 65, 95 },			// Grovyle
	{ 55, 70, 45, 70, 50, 60 },			// Growlithe
	{ 80, 45, 65, 90, 110, 80 },		// Grumpig	
	{ 70, 43, 53, 43, 53, 40 },			// Gulpin
	{ 95, 125, 79, 60, 100, 81 },		// Gyarados	
	{ 100, 5, 5, 15, 65, 30 },			// Happiny
	{ 144, 120, 60, 40, 60, 50 },		// Hariyama	
	{ 45, 50, 45, 115, 55, 95 },		// Haunter	
	{ 91, 90, 106, 130, 106, 77 },		// Heatran	
	{ 80, 125, 75, 40, 95, 85 },		// Heracross	
	{ 68, 72, 78, 38, 42, 32 },			// Hippopotas
	{ 108, 112, 118, 68, 72, 47 },		// Hippowdon	
	{ 50, 105, 79, 35, 110, 76 },		// Hitmonchan	
	{ 50, 120, 53, 35, 110, 87 },		// Hitmonlee	
	{ 50, 95, 95, 35, 110, 70 },		// Hitmontop	
	{ 106, 130, 90, 110, 154, 90 },		// Ho-Oh	
	{ 100, 125, 52, 105, 52, 71 },		// Honchkrow	
	{ 60, 30, 30, 36, 56, 50 },			// Hoothoot
	{ 35, 35, 40, 35, 55, 50 },			// Hoppip
	{ 30, 40, 70, 70, 25, 60 },			// Horsea
	{ 75, 90, 50, 110, 80, 95 },		// Houndoom	
	{ 45, 60, 30, 80, 50, 65 },			// Houndour
	{ 55, 104, 105, 94, 75, 52 },		// Huntail	
	{ 85, 73, 70, 73, 115, 67 },		// Hypno	
	{ 90, 30, 15, 40, 20, 15 },			// Igglybuff
	{ 65, 47, 55, 73, 75, 85 },			// Illumise
	{ 76, 104, 71, 104, 71, 108 },		// Infernape	
	{ 60, 62, 63, 80, 80, 60 },			// Ivysaur
	{ 115, 45, 20, 45, 25, 20 },		// Jigglypuff	
	{ 100, 100, 100, 100, 100, 100 },	// Jirachi		
	{ 65, 65, 60, 110, 95, 130 },		// Jolteon	
	{ 75, 55, 70, 55, 85, 110 },		// Jumpluff	
	{ 65, 50, 35, 115, 95, 95 },		// Jynx	
	{ 30, 80, 90, 55, 45, 55 },			// Kabuto
	{ 60, 115, 105, 65, 70, 80 },		// Kabutops	
	{ 40, 35, 30, 120, 70, 105 },		// Kadabra	
	{ 45, 25, 50, 25, 25, 35 },			// Kakuna
	{ 105, 95, 80, 40, 80, 90 },		// Kangaskhan	
	{ 60, 90, 70, 60, 120, 40 },		// Kecleon	
	{ 75, 95, 95, 95, 95, 85 },			// Kingdra
	{ 55, 130, 115, 50, 50, 75 },		// Kingler	
	{ 38, 35, 35, 65, 55, 50 },			// Kirlia
	{ 40, 65, 95, 60, 45, 35 },			// Koffing
	{ 30, 105, 90, 25, 25, 50 },		// Krabby	
	{ 37, 25, 41, 25, 41, 25 },			// Kricketot
	{ 77, 85, 51, 55, 51, 65 },			// Kricketune
	{ 100, 100, 90, 150, 140, 90 },		// Kyogre	
	{ 60, 90, 140, 50, 50, 40 },		// Lairon	
	{ 125, 58, 58, 76, 76, 67 },		// Lanturn	
	{ 130, 85, 80, 85, 95, 60 },		// Lapras	
	{ 50, 64, 50, 45, 50, 41 },			// Larvitar
	{ 80, 80, 90, 110, 130, 110 },		// Latias	
	{ 80, 90, 80, 130, 110, 110 },		// Latios	
	{ 65, 110, 130, 60, 65, 95 },		// Leafeon	
	{ 55, 35, 50, 55, 110, 85 },		// Ledian	
	{ 40, 20, 30, 40, 80, 55 },			// Ledyba
	{ 110, 85, 95, 80, 95, 50 },		// Lickilicky	
	{ 90, 55, 75, 60, 75, 30 },			// Lickitung
	{ 66, 41, 77, 61, 87, 23 },			// Lileep
	{ 78, 70, 61, 50, 61, 100 },		// Linoone	
	{ 60, 50, 50, 60, 70, 50 },			// Lombre
	{ 65, 76, 84, 54, 96, 105 },		// Lopunny	
	{ 40, 30, 30, 40, 50, 30 },			// Lotad
	{ 84, 71, 43, 71, 43, 48 },			// Loudred
	{ 70, 110, 70, 115, 70, 90 },		// Lucario	
	{ 80, 70, 70, 90, 100, 70 },		// Ludicolo	
	{ 106, 90, 130, 90, 154, 110 },		// Lugia	
	{ 69, 69, 76, 69, 86, 91 },			// Lumineon
	{ 70, 55, 65, 95, 85, 70 },			// Lunatone
	{ 43, 30, 55, 40, 65, 97 },			// Luvdisc
	{ 60, 85, 49, 60, 49, 60 },			// Luxio
	{ 80, 120, 79, 95, 79, 70 },		// Luxray	
	{ 90, 130, 80, 65, 85, 55 },		// Machamp	
	{ 80, 100, 70, 50, 60, 45 },		// Machoke	
	{ 70, 80, 50, 35, 35, 35 },			// Machop
	{ 45, 75, 37, 70, 55, 83 },			// Magby
	{ 50, 50, 120, 80, 80, 30 },		// Magcargo	
	{ 20, 10, 55, 15, 20, 80 },			// Magikarp
	{ 65, 95, 57, 100, 85, 93 },		// Magmar	
	{ 75, 95, 67, 125, 95, 83 },		// Magmortar	
	{ 25, 35, 70, 95, 55, 45 },			// Magnemite
	{ 50, 60, 95, 120, 70, 70 },		// Magneton	
	{ 70, 70, 115, 130, 90, 60 },		// Magnezone	
	{ 72, 60, 30, 20, 30, 25 },			// Makuhita
	{ 110, 130, 80, 70, 60, 80 },		// Mamoswine	
	{ 100, 100, 100, 100, 100, 100 },	// Manaphy		
	{ 70, 75, 60, 105, 60, 105 },		// Manectric	
	{ 40, 80, 35, 35, 45, 70 },			// Mankey
	{ 65, 40, 70, 80, 140, 70 },		// Mantine	
	{ 45, 20, 50, 60, 120, 50 },		// Mantyke	
	{ 55, 40, 40, 65, 45, 35 },			// Mareep
	{ 70, 20, 50, 20, 50, 40 },			// Marill
	{ 60, 80, 110, 50, 80, 45 },		// Marowak	
	{ 70, 85, 70, 60, 70, 50 },			// Marshtomp
	{ 70, 60, 62, 80, 82, 60 },			// Masquerain
	{ 50, 85, 85, 55, 55, 50 },			// Mawile
	{ 60, 60, 75, 60, 75, 80 },			// Medicham
	{ 30, 40, 55, 40, 55, 60 },			// Meditite
	{ 80, 82, 100, 83, 100, 80 },		// Meganium	
	{ 40, 45, 35, 40, 40, 90 },			// Meowth
	{ 80, 105, 105, 105, 105, 80 },		// Mesprit	
	{ 80, 135, 130, 95, 90, 70 },		// Metagross	
	{ 60, 75, 100, 55, 80, 50 },		// Metang	
	{ 50, 20, 55, 25, 25, 30 },			// Metapod
	{ 100, 100, 100, 100, 100, 100 },	// Mew		
	{ 106, 110, 90, 154, 90, 130 },		// Mewtwo	
	{ 70, 90, 70, 60, 60, 70 },			// Mightyena
	{ 95, 60, 79, 100, 125, 81 },		// Milotic	
	{ 95, 80, 105, 40, 70, 100 },		// Miltank	
	{ 20, 25, 45, 70, 90, 60 },			// Mime Jr.
	{ 60, 40, 50, 75, 85, 95 },			// Minun
	{ 60, 60, 60, 85, 85, 85 },			// Misdreavus
	{ 60, 60, 60, 105, 105, 105 },		// Mismagius	
	{ 90, 100, 90, 125, 85, 90 },		// Moltres	
	{ 64, 78, 52, 78, 52, 81 },			// Monferno
	{ 70, 94, 50, 94, 50, 66 },			// Mothim
	{ 40, 45, 65, 100, 120, 90 },		// Mr. Mime	
	{ 50, 70, 50, 50, 50, 40 },			// Mudkip
	{ 105, 105, 75, 65, 100, 50 },		// Muk	
	{ 135, 85, 40, 40, 85, 5 },			// Munchlax
	{ 60, 85, 42, 85, 42, 91 },			// Murkrow
	{ 40, 50, 45, 70, 45, 70 },			// Natu
	{ 81, 92, 77, 85, 75, 85 },			// Nidoking
	{ 90, 82, 87, 75, 85, 76 },			// Nidoqueen
	{ 55, 47, 52, 40, 40, 41 },			// Nidoran-F
	{ 46, 57, 40, 40, 40, 50 },			// Nidoran-M
	{ 70, 62, 67, 55, 55, 56 },			// Nidorina
	{ 61, 72, 57, 55, 55, 65 },			// Nidorino
	{ 31, 45, 90, 30, 30, 40 },			// Nincada
	{ 73, 76, 75, 81, 100, 100 },		// Ninetales	
	{ 61, 90, 45, 50, 50, 160 },		// Ninjask	
	{ 100, 50, 50, 76, 96, 70 },		// Noctowl	
	{ 30, 45, 135, 45, 90, 30 },		// Nosepass	
	{ 60, 60, 40, 65, 45, 35 },			// Numel
	{ 70, 70, 40, 60, 40, 60 },			// Nuzleaf
	{ 75, 105, 75, 105, 75, 45 },		// Octillery	
	{ 45, 50, 55, 75, 65, 30 },			// Oddish
	{ 35, 40, 100, 90, 55, 35 },		// Omanyte	
	{ 70, 60, 125, 115, 70, 55 },		// Omastar	
	{ 35, 45, 160, 30, 45, 70 },		// Onix	
	{ 60, 45, 70, 45, 90, 95 },			// Pachirisu
	{ 90, 120, 100, 150, 120, 100 },	// Palkia		
	{ 35, 70, 55, 45, 55, 25 },			// Paras
	{ 60, 95, 80, 60, 80, 30 },			// Parasect
	{ 60, 50, 100, 85, 70, 65 },		// Pelipper	
	{ 65, 70, 60, 65, 65, 115 },		// Persian	
	{ 90, 60, 60, 40, 40, 40 },			// Phanpy
	{ 80, 80, 80, 80, 80, 80 },			// Phione
	{ 20, 40, 15, 35, 35, 60 },			// Pichu
	{ 83, 80, 75, 70, 70, 91 },			// Pidgeot
	{ 63, 60, 55, 50, 50, 71 },			// Pidgeotto
	{ 40, 45, 40, 35, 35, 56 },			// Pidgey
	{ 35, 55, 30, 50, 40, 90 },			// Pikachu
	{ 100, 100, 80, 60, 60, 50 },		// Piloswine	
	{ 50, 65, 90, 35, 35, 15 },			// Pineco
	{ 65, 125, 100, 55, 70, 85 },		// Pinsir	
	{ 53, 51, 53, 61, 56, 40 },			// Piplup
	{ 60, 50, 40, 85, 75, 95 },			// Plusle
	{ 90, 75, 75, 90, 100, 70 },		// Politoed	
	{ 40, 50, 40, 40, 40, 90 },			// Poliwag
	{ 65, 65, 65, 50, 50, 90 },			// Poliwhirl
	{ 90, 85, 95, 70, 90, 70 },			// Poliwrath
	{ 50, 85, 55, 65, 65, 90 },			// Ponyta
	{ 35, 55, 35, 30, 30, 35 },			// Poochyena
	{ 65, 60, 70, 85, 75, 40 },			// Porygon
	{ 85, 80, 70, 135, 75, 90 },		// Porygon-Z	
	{ 85, 80, 90, 105, 95, 60 },		// Porygon2	
	{ 65, 105, 60, 60, 70, 95 },		// Primeape	
	{ 64, 66, 68, 81, 76, 50 },			// Prinplup
	{ 60, 55, 145, 75, 150, 40 },		// Probopass	
	{ 50, 52, 48, 65, 50, 55 },			// Psyduck
	{ 70, 84, 70, 65, 70, 51 },			// Pupitar
	{ 71, 82, 64, 64, 59, 112 },		// Purugly	
	{ 95, 85, 85, 65, 65, 35 },			// Quagsire
	{ 58, 64, 58, 80, 65, 80 },			// Quilava
	{ 65, 95, 75, 55, 55, 85 },			// Qwilfish
	{ 60, 90, 55, 90, 80, 100 },		// Raichu	
	{ 90, 85, 75, 115, 100, 115 },		// Raikou	
	{ 28, 25, 25, 45, 35, 40 },			// Ralts
	{ 97, 165, 60, 65, 50, 58 },		// Rampardos	
	{ 65, 100, 70, 80, 80, 105 },		// Rapidash	
	{ 55, 81, 60, 50, 70, 97 },			// Raticate
	{ 30, 56, 35, 25, 35, 72 },			// Rattata
	{ 105, 150, 90, 150, 90, 95 },		// Rayquaza	
	{ 80, 50, 100, 100, 200, 50 },		// Regice	
	{ 110, 160, 110, 80, 110, 100 },	// Regigigas		
	{ 80, 100, 200, 50, 100, 50 },		// Regirock	
	{ 80, 75, 150, 75, 150, 50 },		// Registeel	
	{ 100, 90, 130, 45, 65, 55 },		// Relicanth	
	{ 35, 65, 35, 65, 35, 65 },			// Remoraid
	{ 105, 130, 120, 45, 45, 40 },		// Rhydon	
	{ 80, 85, 95, 30, 30, 25 },			// Rhyhorn
	{ 115, 140, 130, 55, 55, 40 },		// Rhyperior	
	{ 40, 70, 40, 35, 40, 60 },			// Riolu
	{ 50, 60, 45, 100, 80, 65 },		// Roselia	
	{ 60, 70, 55, 125, 105, 90 },		// Roserade	
	{ 50, 50, 77, 95, 77, 91 },			// Rotom
	{ 50, 65, 107, 105, 107, 86 },		// Rotom-C	
	{ 50, 65, 107, 105, 107, 86 },		// Rotom-F	
	{ 50, 65, 107, 105, 107, 86 },		// Rotom-H	
	{ 50, 65, 107, 105, 107, 86 },		// Rotom-S	
	{ 50, 65, 107, 105, 107, 86 },		// Rotom-W	
	{ 50, 75, 75, 65, 65, 50 },			// Sableye
	{ 95, 135, 80, 110, 80, 100 },		// Salamence	
	{ 50, 75, 85, 20, 30, 40 },			// Sandshrew
	{ 75, 100, 110, 45, 55, 65 },		// Sandslash	
	{ 70, 85, 65, 105, 85, 120 },		// Sceptile	
	{ 70, 130, 100, 55, 80, 65 },		// Scizor	
	{ 70, 110, 80, 55, 80, 105 },		// Scyther	
	{ 55, 65, 95, 95, 45, 85 },			// Seadra
	{ 80, 92, 65, 65, 80, 68 },			// Seaking
	{ 90, 60, 70, 75, 70, 45 },			// Sealeo
	{ 40, 40, 50, 30, 30, 30 },			// Seedot
	{ 65, 45, 55, 45, 70, 45 },			// Seel
	{ 35, 46, 34, 35, 45, 20 },			// Sentret
	{ 73, 100, 60, 100, 60, 65 },		// Seviper	
	{ 70, 120, 40, 95, 40, 95 },		// Sharpedo	
	{ 100, 100, 100, 100, 100, 100 },	// Shaymin-L		
	{ 100, 103, 75, 120, 75, 127 },		// Shaymin-S	
	{ 1, 90, 45, 30, 30, 40 },			// Shedinja
	{ 65, 95, 100, 60, 50, 50 },		// Shelgon	
	{ 30, 65, 100, 45, 25, 40 },		// Shellder	
	{ 76, 48, 48, 57, 62, 34 },			// Shellos
	{ 30, 42, 118, 42, 88, 30 },		// Shieldon	
	{ 90, 100, 60, 90, 60, 80 },		// Shiftry	
	{ 45, 65, 34, 40, 34, 45 },			// Shinx
	{ 60, 40, 60, 40, 60, 35 },			// Shroomish
	{ 20, 10, 230, 10, 230, 5 },		// Shuckle	
	{ 44, 75, 35, 63, 33, 45 },			// Shuppet
	{ 50, 35, 55, 25, 25, 15 },			// Silcoon
	{ 65, 80, 140, 40, 70, 70 },		// Skarmory	
	{ 55, 45, 50, 45, 65, 80 },			// Skiploom
	{ 50, 45, 45, 35, 35, 50 },			// Skitty
	{ 40, 50, 90, 30, 55, 65 },			// Skorupi
	{ 103, 93, 67, 71, 61, 84 },		// Skuntank	
	{ 150, 160, 100, 95, 65, 100 },		// Slaking	
	{ 60, 60, 60, 35, 35, 30 },			// Slakoth
	{ 95, 75, 110, 100, 80, 30 },		// Slowbro	
	{ 95, 75, 80, 100, 110, 30 },		// Slowking	
	{ 90, 65, 65, 40, 40, 15 },			// Slowpoke
	{ 40, 40, 40, 70, 40, 20 },			// Slugma
	{ 55, 20, 35, 20, 45, 75 },			// Smeargle
	{ 45, 30, 15, 85, 65, 65 },			// Smoochum
	{ 55, 95, 55, 35, 75, 115 },		// Sneasel	
	{ 160, 110, 65, 65, 110, 30 },		// Snorlax	
	{ 50, 50, 50, 50, 50, 50 },			// Snorunt
	{ 60, 62, 50, 62, 60, 40 },			// Snover
	{ 60, 80, 50, 40, 40, 30 },			// Snubbull
	{ 70, 95, 85, 55, 65, 70 },			// Solrock
	{ 40, 60, 30, 31, 31, 70 },			// Spearow
	{ 70, 40, 50, 55, 50, 25 },			// Spheal
	{ 40, 60, 40, 40, 40, 30 },			// Spinarak
	{ 60, 60, 60, 60, 60, 60 },			// Spinda
	{ 50, 92, 108, 92, 108, 35 },		// Spiritomb	
	{ 60, 25, 35, 70, 80, 60 },			// Spoink
	{ 44, 48, 65, 50, 64, 43 },			// Squirtle
	{ 73, 95, 62, 85, 65, 85 },			// Stantler
	{ 85, 120, 70, 50, 50, 100 },		// Staraptor	
	{ 55, 75, 50, 40, 40, 80 },			// Staravia
	{ 40, 55, 30, 30, 30, 60 },			// Starly
	{ 60, 75, 85, 100, 85, 115 },		// Starmie	
	{ 30, 45, 55, 70, 55, 85 },			// Staryu
	{ 75, 85, 200, 55, 65, 30 },		// Steelix	
	{ 63, 63, 47, 41, 41, 74 },			// Stunky
	{ 70, 100, 115, 30, 65, 30 },		// Sudowoodo	
	{ 100, 75, 115, 90, 115, 85 },		// Suicune	
	{ 75, 75, 55, 105, 85, 30 },		// Sunflora	
	{ 30, 30, 30, 30, 30, 30 },			// Sunkern
	{ 40, 30, 32, 50, 52, 65 },			// Surskit
	{ 45, 40, 60, 40, 75, 50 },			// Swablu
	{ 100, 73, 83, 73, 83, 55 },		// Swalot	
	{ 100, 110, 90, 85, 90, 60 },		// Swampert	
	{ 60, 85, 60, 50, 50, 125 },		// Swellow	
	{ 50, 50, 40, 30, 30, 50 },			// Swinub
	{ 40, 55, 30, 30, 30, 85 },			// Taillow
	{ 65, 55, 115, 100, 40, 60 },		// Tangela	
	{ 100, 100, 125, 110, 50, 50 },		// Tangrowth	
	{ 75, 100, 95, 40, 70, 110 },		// Tauros	
	{ 60, 80, 50, 50, 50, 40 },			// Teddiursa
	{ 40, 40, 35, 50, 100, 70 },		// Tentacool	
	{ 80, 70, 65, 80, 120, 100 },		// Tentacruel	
	{ 85, 50, 95, 120, 115, 80 },		// Togekiss	
	{ 35, 20, 65, 40, 65, 20 },			// Togepi
	{ 55, 40, 85, 80, 105, 40 },		// Togetic	
	{ 45, 60, 40, 70, 50, 45 },			// Torchic
	{ 70, 85, 140, 85, 70, 20 },		// Torkoal	
	{ 95, 109, 105, 75, 85, 56 },		// Torterra	
	{ 50, 65, 64, 44, 48, 43 },			// Totodile
	{ 83, 106, 65, 86, 65, 85 },		// Toxicroak	
	{ 45, 100, 45, 45, 45, 10 },		// Trapinch	
	{ 40, 45, 35, 65, 55, 70 },			// Treecko
	{ 99, 68, 83, 72, 87, 51 },			// Tropius
	{ 55, 68, 64, 45, 55, 31 },			// Turtwig
	{ 78, 84, 78, 109, 85, 100 },		// Typhlosion	
	{ 100, 134, 110, 95, 100, 61 },		// Tyranitar	
	{ 35, 35, 35, 35, 35, 35 },			// Tyrogue
	{ 95, 65, 110, 60, 130, 65 },		// Umbreon	
	{ 48, 72, 48, 72, 48, 48 },			// Unown
	{ 90, 130, 75, 75, 75, 55 },		// Ursaring	
	{ 75, 75, 130, 75, 130, 95 },		// Uxie	
	{ 130, 65, 60, 110, 95, 65 },		// Vaporeon	
	{ 70, 65, 60, 90, 75, 90 },			// Venomoth
	{ 60, 55, 50, 40, 55, 45 },			// Venonat
	{ 80, 82, 83, 100, 100, 80 },		// Venusaur	
	{ 70, 80, 102, 80, 102, 40 },		// Vespiquen	
	{ 50, 70, 50, 50, 50, 70 },			// Vibrava
	{ 80, 105, 65, 100, 60, 70 },		// Victreebel	
	{ 80, 80, 80, 55, 55, 90 },			// Vigoroth
	{ 75, 80, 85, 100, 90, 50 },		// Vileplume	
	{ 65, 73, 55, 47, 75, 85 },			// Volbeat
	{ 40, 30, 50, 55, 55, 100 },		// Voltorb	
	{ 38, 41, 40, 50, 65, 65 },			// Vulpix
	{ 130, 70, 35, 70, 35, 60 },		// Wailmer	
	{ 170, 90, 45, 90, 45, 60 },		// Wailord	
	{ 110, 80, 90, 95, 90, 65 },		// Walrein	
	{ 59, 63, 80, 65, 80, 58 },			// Wartortle
	{ 70, 120, 65, 45, 85, 125 },		// Weavile	
	{ 40, 35, 30, 20, 20, 50 },			// Weedle
	{ 65, 90, 50, 85, 45, 55 },			// Weepinbell
	{ 65, 90, 120, 85, 70, 60 },		// Weezing	
	{ 110, 78, 73, 76, 71, 60 },		// Whiscash	
	{ 64, 51, 23, 51, 23, 28 },			// Whismur
	{ 140, 70, 45, 75, 50, 45 },		// Wigglytuff	
	{ 40, 30, 30, 55, 30, 85 },			// Wingull
	{ 190, 33, 58, 33, 58, 33 },		// Wobbuffet	
	{ 55, 45, 45, 25, 25, 15 },			// Wooper
	{ 60, 59, 85, 79, 105, 36 },		// Wormadam-P	
	{ 60, 79, 105, 59, 85, 36 },		// Wormadam-S	
	{ 60, 69, 95, 69, 95, 36 },			// Wormadam-T
	{ 45, 45, 35, 20, 30, 20 },			// Wurmple
	{ 95, 23, 48, 23, 48, 23 },			// Wynaut
	{ 65, 75, 70, 95, 70, 95 },			// Xatu
	{ 65, 65, 45, 75, 45, 95 },			// Yanma
	{ 86, 76, 86, 116, 56, 95 },		// Yanmega	
	{ 73, 115, 60, 60, 60, 90 },		// Zangoose	
	{ 90, 90, 85, 125, 90, 100 },		// Zapdos	
	{ 38, 30, 41, 30, 41, 60 },			// Zigzagoon
	{ 40, 45, 35, 30, 40, 55 }		// Zubat
};

}
