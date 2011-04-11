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

#include <map>
#include <string>
#include "ability.h"
#include "item.h"
#include "move.h"
#include "pokemon.h"
#include "simple.h"
#include "stat.h"
#include "statfunction.h"
#include "status.h"
#include "team.h"
#include "weather.h"

namespace technicalmachine {

void hitpoints (Pokemon &member) {
	if (1 == member.hp.base)
		member.hp.max = 1;
	else
		member.hp.max = (2 * member.hp.base + member.hp.iv + member.hp.ev) * member.level / 100 + member.level + 10;
	member.hp.stat = member.hp.max;
}

void attack (Pokemon &member, const Weather &weather) {
	if (member.move->physical) {
		member.atk.stat = (2 * member.atk.base + member.atk.iv + member.atk.ev) * member.level / 100 + 5;
		if (ADAMANT == member.nature or BRAVE == member.nature or LONELY == member.nature or NAUGHTY == member.nature)
			member.atk.stat = member.atk.stat * 11 / 10;
		else if (BOLD == member.nature or CALM == member.nature or MODEST == member.nature or TIMID == member.nature)
			member.atk.stat = member.atk.stat * 9 / 10;

		if (member.atk.stage >= 0) // >= is better than == to reduce the frequency of checking for a CH
			member.atk.stat = member.atk.stat * (2 + member.atk.stage) / 2;
		else {
			if (!member.move->ch)
				member.atk.stat = member.atk.stat * 2 / (2 - member.atk.stage);
		}

		if (member.slow_start != 0)
			member.atk.stat /= 2;
		else if ((FLOWER_GIFT == member.ability and 0 != weather.sun) or (GUTS == member.ability and NO_STATUS != member.status) or (HUSTLE == member.ability))
			member.atk.stat = member.atk.stat * 3 / 2;
		else if (PURE_POWER == member.ability)
			member.atk.stat *= 2;
	
		if (CHOICE_BAND == member.item)
			member.atk.stat = member.atk.stat * 3 / 2;
		else if ((LIGHT_BALL == member.item and PIKACHU == member.name) or (THICK_CLUB == member.item and (CUBONE == member.name or MAROWAK == member.name)))
			member.atk.stat *= 2;
		
		if (member.atk.stat == 0)
			member.atk.stat = 1;
	}
	else {
		member.spa.stat = (2 * member.spa.base + member.spa.iv + member.spa.ev) * member.level / 100 + 5;
		if (MILD == member.nature or MODEST == member.nature or QUIET == member.nature or RASH == member.nature)
			member.spa.stat = member.spa.stat * 11 / 10;
		else if (ADAMANT == member.nature or CAREFUL == member.nature or IMPISH == member.nature or JOLLY == member.nature)
			member.spa.stat = member.spa.stat * 9 / 10;

		if (member.spa.stage >= 0)	// >= is better than == to reduce the frequency of checking for a CH
			member.spa.stat = member.spa.stat * (2 + member.spa.stage) / 2;
		else {
			if (!member.move->ch)
				member.spa.stat = member.spa.stat * 2 / (2 - member.spa.stage);
		}

		if (SOLAR_POWER == member.ability and 0 != weather.sun)
			member.spa.stat = member.spa.stat * 3 / 2;

		if ((CHOICE_SPECS == member.item) or (SOUL_DEW == member.item and (LATIAS == member.name or LATIOS == member.name)))
			member.spa.stat = member.spa.stat * 3 / 2;
		else if ((DEEPSEATOOTH == member.item and CLAMPERL == member.name) or (LIGHT_BALL == member.item and PIKACHU == member.name))
			member.spa.stat *= 2;

		if (member.spa.stat == 0)
			member.spa.stat = 1;
	}
}

void defense (const Pokemon &attacker, Pokemon &defender, const Weather &weather) {
	if (attacker.move->physical) {
		defender.def.stat = (2 * defender.def.base + defender.def.iv + defender.def.ev) * defender.level / 100 + 5;
		if (BOLD == defender.nature or IMPISH == defender.nature or LAX == defender.nature or RELAXED == defender.nature)
			defender.def.stat = defender.def.stat * 11 / 10;
		else if (GENTLE == defender.nature or HASTY == defender.nature or LONELY == defender.nature or MILD == defender.nature)
			defender.def.stat = defender.def.stat * 9 / 10;

		if (defender.def.stage > 0) {	// > is better than == to reduce the frequency of checking for a CH
			if (!attacker.move->ch)
				defender.def.stat = defender.def.stat * (2 + defender.def.stage) / 2;
		}
		else
			defender.def.stat = defender.def.stat * 2 / (2 - defender.def.stage);

		if (MARVEL_SCALE == defender.ability and NO_STATUS != defender.status)
			defender.def.stat = defender.def.stat * 3 / 2;
		
		if (METAL_POWDER == defender.item and DITTO == defender.name)
			defender.def.stat = defender.def.stat * 3 / 2;
		
		if (EXPLOSION == attacker.move->name or SELFDESTRUCT == attacker.move->name)
			defender.def.stat /= 2;
	
		if (0 == defender.def.stat)
			defender.def.stat = 1;
	}
	else {
		defender.spd.stat = (2 * defender.spd.base + defender.spd.iv + defender.spd.ev) * defender.level / 100 + 5;
		if (CALM == defender.nature or CAREFUL == defender.nature or GENTLE == defender.nature or SASSY == defender.nature)
			defender.spd.stat = defender.spd.stat * 11 / 10;
		else if (LAX == defender.nature or NAIVE == defender.nature or NAUGHTY == defender.nature or RASH == defender.nature)
			defender.spd.stat = defender.spd.stat * 9 / 10;

		if (defender.spd.stage > 0) {	// > is better than == to reduce the frequency of checking for a CH
			if (!attacker.move->ch)
				defender.spd.stat = defender.spd.stat * (2 + defender.spd.stage) / 2;
		}
		else
			defender.spd.stat = defender.spd.stat * 2 / (2 - defender.spd.stage);

		if (FLOWER_GIFT == defender.ability and 0 != weather.sun)
			defender.spd.stat = defender.spd.stat * 3 / 2;
		
		if (DEEPSEASCALE == defender.item and CLAMPERL == defender.name)
			defender.spd.stat *= 2;
		else if ((METAL_POWDER == defender.item and DITTO == defender.name) or (SOUL_DEW == defender.item and (LATIAS == defender.name or LATIOS == defender.name)))
			defender.spd.stat = defender.spd.stat * 3 / 2;
		
		if (istype (defender, ROCK) and 0 != weather.sand)
			defender.spd.stat = defender.spd.stat * 3 / 2;
		
		if (0 == defender.spd.stat)
			defender.spd.stat = 1;
	}
}

void speed (Team &team, const Weather &weather) {
	team.active->spe.stat = (2 * team.active->spe.base + team.active->spe.iv + team.active->spe.ev) * team.active->level / 100 + 5;

	if (HASTY == team.active->nature or JOLLY == team.active->nature or NAIVE == team.active->nature or TIMID == team.active->nature)
		team.active->spe.stat = team.active->spe.stat * 11 / 10;
	else if (BRAVE == team.active->nature or QUIET == team.active->nature or RELAXED == team.active->nature or SASSY == team.active->nature)
		team.active->spe.stat = team.active->spe.stat * 9 / 10;
	
	if (team.active->spe.stage >= 0)
		team.active->spe.stat = team.active->spe.stat * (2 + team.active->spe.stage) / 2;
	else
		team.active->spe.stat = team.active->spe.stat * 2 / (2 - team.active->spe.stage);

	if ((CHLOROPHYLL == team.active->ability and 0 != weather.sun) or (SWIFT_SWIM == team.active->ability and 0 != weather.rain) or (UNBURDEN == team.active->ability and false))
		team.active->spe.stat *= 2;
	else if (QUICK_FEET == team.active->ability and NO_STATUS != team.active->status)
		team.active->spe.stat = team.active->spe.stat * 3 / 2;
	else if (0 != team.active->slow_start)
		team.active->spe.stat /= 2;
		
	if (QUICK_POWDER == team.active->item and DITTO == team.active->name)
		team.active->spe.stat *= 2;
	else if (CHOICE_SCARF == team.active->item)
		team.active->spe.stat = team.active->spe.stat * 3 / 2;
	else if (MACHO_BRACE == team.active->item or POWER_ITEMS == team.active->item)
		team.active->spe.stat /= 2;
		
	if (PARALYSIS == team.active->status and QUICK_FEET != team.active->ability)
		team.active->spe.stat /= 4;
	
	if (team.tailwind)
		team.active->spe.stat *= 2;

	if (team.active->spe.stat == 0)
		team.active->spe.stat = 1;
}

void order (Team &team1, Team &team2, const Weather &weather, Team* &faster, Team* &slower) {
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

void statboost (char &stage, int n) {
	stage += n;
	if (stage > 6)
		stage = 6;
	else if (stage < -6)
		stage = -6;
}

void set_nature_map (std::map <std::string, natures> &natures_map) {
	natures_map["Adamant"] = ADAMANT;
	natures_map["Bashful"] = BASHFUL;
	natures_map["Bold"] = BOLD;
	natures_map["Brave"] = BRAVE;
	natures_map["Calm"] = CALM;
	natures_map["Careful"] = CAREFUL;
	natures_map["Docile"] = DOCILE;
	natures_map["Gentle"] = GENTLE;
	natures_map["Hardy"] = HARDY;
	natures_map["Hasty"] = HASTY;
	natures_map["Impish"] = IMPISH;
	natures_map["Jolly"] = JOLLY;
	natures_map["Lax"] = LAX;
	natures_map["Lonely"] = LONELY;
	natures_map["Mild"] = MILD;
	natures_map["Modest"] = MODEST;
	natures_map["Naive"] = NAIVE;
	natures_map["Naughty"] = NAUGHTY;
	natures_map["Quiet"] = QUIET;
	natures_map["Quirky"] = QUIRKY;
	natures_map["Rash"] = RASH;
	natures_map["Relaxed"] = RELAXED;
	natures_map["Sassy"] = SASSY;
	natures_map["Serious"] = SERIOUS;
	natures_map["Timid"] = TIMID;
}

}
