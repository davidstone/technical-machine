// Stat formulas
// Copyright 2010 David Stone
//
// This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public License
// as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License along with this program.  If not, see <http://www.gnu.org/licenses/>.

#ifndef STAT_CPP_
#define STAT_CPP_

#include "ability.h"
#include "item.h"
#include "move.h"
#include "pokemon.h"
#include "simple.h"
#include "stat.h"
#include "status.h"
#include "team.h"
#include "weather.h"

void hitpoints (pokemon &member) {
	if (1 == member.hp.base)
		member.hp.max = 1;
	else
		member.hp.max = (2 * member.hp.base + member.hp.iv + member.hp.ev) * member.level / 100 + member.level + 10;
}

void attack (pokemon &member, const weathers &weather) {
	if (member.move->physical) {
		int nature;
		if (ADAMANT == member.nature or BRAVE == member.nature or LONELY == member.nature or NAUGHTY == member.nature)
			nature = 11;
		else if (BOLD == member.nature or CALM == member.nature or MODEST == member.nature or TIMID == member.nature)
			nature = 9;
		else
			nature = 10;

		int aam;				// Attack Ability Mod: Flower Gift (3), Guts (3), Huge Power (4), Hustle (3), Pure Power (4), Slow Start (1) / 2
		if (0 != member.slow_start)
			aam = 1;
		else if ((FLOWER_GIFT == member.ability and 0 != weather.sun) or (GUTS == member.ability and NO_STATUS != member.status) or (HUSTLE == member.ability))
			aam = 3;
		else if (PURE_POWER == member.ability)
			aam = 4;
		else
			aam = 2;
	
		int aim;				// Attack Item Mod: Choice Band (3), Light Ball (4), Thick Club (4) / 2
		if (CHOICE_BAND == member.item)
			aim = 3;
		else if ((LIGHT_BALL == member.item and PIKACHU == member.pokemon) or (THICK_CLUB == member.item and (CUBONE == member.pokemon or MAROWAK == member.pokemon)))
			aim = 4;
		else
			aim = 2;
	
		if (member.atk.stage >= 0)	// >= is superior to > due to no stage boosts not having to check for a CH
			member.atk.stat = ((2 * member.atk.base + member.atk.iv + member.atk.ev) * member.level / 100 + 5) * nature / 10 * (2 + member.atk.stage) / 2 * aam / 2 * aim / 2;
		else {
			if (member.move->ch)
				member.atk.stage = 0;
			member.atk.stat = ((2 * member.atk.base + member.atk.iv + member.atk.ev) * member.level / 100 + 5) * nature / 10 * 2 / (2 - member.atk.stage) * aam / 2 * aim / 2;
		}
		if (member.atk.stat == 0)
			member.atk.stat = 1;
	}
	else {
		int nature;
		if (MILD == member.nature or MODEST == member.nature or QUIET == member.nature or RASH == member.nature)
			nature = 11;
		else if (ADAMANT == member.nature or CAREFUL == member.nature or IMPISH == member.nature or JOLLY == member.nature)
			nature = 9;
		else
			nature = 10;

		int aam;				// Attack Ability Mod: Solar Power (3) / 2
		if (SOLAR_POWER == member.ability and 0 != weather.sun)
			aam = 3;
		else
			aam = 2;

		int aim;				// Attack Item Mod: Choice Specs (3), DeepSeaTooth (4), Light Ball (4), Soul Dew (3) / 2
		if ((CHOICE_SPECS == member.item) or (SOUL_DEW == member.item and (LATIAS == member.pokemon or LATIOS == member.pokemon)))
			aim = 3;
		else if ((DEEPSEATOOTH == member.item and CLAMPERL == member.pokemon) or (LIGHT_BALL == member.item and PIKACHU == member.pokemon))
			aim = 4;
		else
			aim = 2;

		if (member.spa.stage >= 0)	// >= is superior to > due to no stage boosts not having to check for a CH
			member.spa.stat = ((2 * member.spa.base + member.spa.iv + member.spa.ev) * member.level / 100 + 5) * nature / 10 * (2 + member.spa.stage) / 2 * aam / 2 * aim / 2;
		else {
			if (member.move->ch)
				member.spa.stage = 0;
			member.spa.stat = ((2 * member.spa.base + member.spa.iv + member.spa.ev) * member.level / 100 + 5) * nature / 10 * 2 / (2 - member.spa.stage) * aam / 2 * aim / 2;
		}
		if (member.spa.stat == 0)
			member.spa.stat = 1;
	}
}

void defense (const pokemon &attacker, pokemon &defender, const weathers &weather) {
	if (attacker.move->physical) {
		int nature;
		if (BOLD == defender.nature or IMPISH == defender.nature or LAX == defender.nature or RELAXED == defender.nature)
			nature = 11;
		else if (GENTLE == defender.nature or HASTY == defender.nature or LONELY == defender.nature or MILD == defender.nature)
			nature = 9;
		else
			nature = 10;

		int dam;					// Defense Ability Modifier: Marvel Scale (3) / 2
		if (MARVEL_SCALE == defender.ability and NO_STATUS != defender.status)
			dam = 3;
		else
			dam = 2;
		
		int dim;					// Defense Item Modifier: Metal Powder (3) / 2
		if (METAL_POWDER == defender.item and DITTO == defender.pokemon)
			dim = 3;
		else
			dim = 2;
		
		int boom;				// Explosion / Selfdestuct Defense modifier: 2
		if (EXPLOSION == attacker.move->name or SELFDESTRUCT == attacker.move->name)
			boom = 2;
		else
			boom = 1;
	
		if (defender.def.stage > 0) {	// > is superior to >= due to no stage boosts not having to check for a CH
			if (attacker.move->ch)
				defender.def.stage = 0;
			defender.def.stat = ((2 * defender.def.base + defender.def.iv + defender.def.ev) * defender.level / 100 + 5) * nature / 10 * (2 + defender.def.stage) / 2 * dam / 2 * dim / 2 / boom;
		}
		else
			defender.def.stat = ((2 * defender.def.base + defender.def.iv + defender.def.ev) * defender.level / 100 + 5) * nature / 10 * 2 / (2 - defender.def.stage) * dam / 2 * dim / 2 / boom;
		if (0 == defender.def.stat)
			defender.def.stat = 1;
	}
	else {
		int nature;
		if (CALM == defender.nature or CAREFUL == defender.nature or GENTLE == defender.nature or SASSY == defender.nature)
			nature = 11;
		else if (LAX == defender.nature or NAIVE == defender.nature or NAUGHTY == defender.nature or RASH == defender.nature)
			nature = 9;
		else
			nature = 10;

		int dam;					// Defense Ability Modifier: Flower Gift (3) / 2
		if (FLOWER_GIFT == defender.ability and 0 != weather.sun)
			dam = 3;
		else
			dam = 2;
		
		int dim;					// Defense Item Modifier: DeepSeaScale (4), Metal Powder (3), Soul Dew (3) / 2
		if (DEEPSEASCALE == defender.item and CLAMPERL == defender.pokemon)
			dim = 4;
		else if ((METAL_POWDER == defender.item and DITTO == defender.pokemon) or (SOUL_DEW == defender.item and (LATIAS == defender.pokemon or LATIOS == defender.pokemon)))
			dim = 3;
		else
			dim = 2;
		
		int ss;					// Sandstorm Special Defense bonus to Rocks: 3 / 2
		if (istype (defender, ROCK) and 0 != weather.sand)
			ss = 3;
		else
			ss = 2;
		
		if (defender.spd.stage > 0) {	// > is superior to >= due to no stage boosts not having to check for a CH
			if (attacker.move->ch)
				defender.spd.stage = 0;
			defender.spd.stat = ((2 * defender.spd.base + defender.spd.iv + defender.spd.ev) * defender.level / 100 + 5) * nature / 10 * (2 + defender.spd.stage) / 2 * dam / 2 * dim / 2 * ss / 2;
		}
		else
			defender.spd.stat = ((2 * defender.spd.base + defender.spd.iv + defender.spd.ev) * defender.level / 100 + 5) * nature / 10 * 2 / (2 - defender.spd.stage) * dam / 2 * dim / 2 * ss / 2;
		if (0 == defender.spd.stat)
			defender.spd.stat = 1;
	}
}

void speed (teams &team, const weathers &weather) {
	int nature;
	if (HASTY == team.active->nature or JOLLY == team.active->nature or NAIVE == team.active->nature or TIMID == team.active->nature)
		nature = 11;
	else if (BRAVE == team.active->nature or QUIET == team.active->nature or RELAXED == team.active->nature or SASSY == team.active->nature)
		nature = 9;
	else
		nature = 10;
	
	int sam;				// Speed Ability Modifier: Chlorophyll (4), Swift Swim (4), Unburden (4), Quick Feet (3), Slow Start (1) / 2
	if ((CHLOROPHYLL == team.active->ability and 0 != weather.sun) or (SWIFT_SWIM == team.active->ability and 0 != weather.rain) or (UNBURDEN == team.active->ability and team.active->item_unburden))
		sam = 4;
	else if (QUICK_FEET == team.active->ability and NO_STATUS != team.active->status)
		sam = 3;
	else if (0 != team.active->slow_start)
		sam = 1;
	else
		sam = 2;
		
	int sim;				// Speed Item Modifier: Quick Powder (4), Choice Scarf (3), Macho Brace (1), Power Items (1) / 2
	if (QUICK_POWDER == team.active->item and DITTO == team.active->pokemon)
		sim = 4;
	else if (CHOICE_SCARF == team.active->item)
		sim = 3;
	else if (MACHO_BRACE == team.active->item or POWER_ITEMS == team.active->item)
		sim = 1;
	else
		sim = 2;
		
	int paralysis;		// Paralysis Speed drop (4)
	if (PARALYSIS == team.active->status and QUICK_FEET != team.active->ability)
		paralysis = 4;
	else
		paralysis = 1;
	
	int tailwind_mod;	// 2
	if (team.tailwind)
		tailwind_mod = 2;
	else
		tailwind_mod = 1;
	
	team.active->spe.stat = ((2 * team.active->spe.base + team.active->spe.iv + team.active->spe.ev) * team.active->level / 100 + 5) * nature / 10;	
	if (team.active->spe.stage >= 0)
		team.active->spe.stat = team.active->spe.stat * (2 + team.active->spe.stage) / 2;
	else
		team.active->spe.stat = team.active->spe.stat * 2 / (2 - team.active->spe.stage);
	team.active->spe.stat = team.active->spe.stat * sam / 2 * sim / 2 / paralysis * tailwind_mod;

	if (0 == team.active->spe.stat)
		team.active->spe.stat = 1;
}

void order (teams &team1, teams &team2, const weathers &weather, teams* &faster, teams* &slower) {
	if (team1.active->move->priority == team2.active->move->priority) {
		speed (team1, weather);
		speed (team2, weather);
		if (team1.active->spe.stat > team2.active->spe.stat) {
			faster = &team1;
			slower = &team2;
		}
		else if (team2.active->spe.stat > team1.active->spe.stat) {
			faster = &team2;
			slower = &team1;
		}
		else				// All things are equal
			faster = NULL;			// I'll only check if faster == NULL, so no point wasting my time setting slower to NULL, too
	}
	else if (team1.active->move->priority > team2.active->move->priority) {
		faster = &team1;
		slower = &team2;
	}
	else {				// (team2.active->move->priority > team1.active->move->priority)
		faster = &team2;
		slower = &team1;
	}
}

void statboost (stats &stat, int n) {
	stat.stage += n;
	if (stat.stage > 6)
		stat.stage = 6;
	else if (stat.stage < -6)
		stat.stage = -6;
}

#endif
