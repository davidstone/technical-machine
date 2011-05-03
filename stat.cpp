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
#include "stat.h"
#include "statfunction.h"
#include "status.h"
#include "team.h"
#include "typefunction.h"
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

void attack (Team &attacker, const Weather &weather) {
	if (attacker.active->move->physical) {
		attacker.active->atk.stat = (2 * attacker.active->atk.base + attacker.active->atk.iv + attacker.active->atk.ev) * attacker.active->level / 100 + 5;
		if (ADAMANT == attacker.active->nature or BRAVE == attacker.active->nature or LONELY == attacker.active->nature or NAUGHTY == attacker.active->nature)
			attacker.active->atk.stat = attacker.active->atk.stat * 11 / 10;
		else if (BOLD == attacker.active->nature or CALM == attacker.active->nature or MODEST == attacker.active->nature or TIMID == attacker.active->nature)
			attacker.active->atk.stat = attacker.active->atk.stat * 9 / 10;

		if (attacker.active->atk.stage >= 0) // >= is better than == to reduce the frequency of checking for a CH
			attacker.active->atk.stat = attacker.active->atk.stat * (2 + attacker.active->atk.stage) / 2;
		else {
			if (!attacker.active->move->ch)
				attacker.active->atk.stat = attacker.active->atk.stat * 2 / (2 - attacker.active->atk.stage);
		}

		if (attacker.slow_start != 0)
			attacker.active->atk.stat /= 2;
		else if ((FLOWER_GIFT == attacker.active->ability and 0 != weather.sun) or (GUTS == attacker.active->ability and NO_STATUS != attacker.active->status) or (HUSTLE == attacker.active->ability))
			attacker.active->atk.stat = attacker.active->atk.stat * 3 / 2;
		else if (PURE_POWER == attacker.active->ability)
			attacker.active->atk.stat *= 2;
	
		if (CHOICE_BAND == attacker.active->item)
			attacker.active->atk.stat = attacker.active->atk.stat * 3 / 2;
		else if ((LIGHT_BALL == attacker.active->item and PIKACHU == attacker.active->name) or (THICK_CLUB == attacker.active->item and (CUBONE == attacker.active->name or MAROWAK == attacker.active->name)))
			attacker.active->atk.stat *= 2;
		
		if (attacker.active->atk.stat == 0)
			attacker.active->atk.stat = 1;
	}
	else {
		attacker.active->spa.stat = (2 * attacker.active->spa.base + attacker.active->spa.iv + attacker.active->spa.ev) * attacker.active->level / 100 + 5;
		if (MILD == attacker.active->nature or MODEST == attacker.active->nature or QUIET == attacker.active->nature or RASH == attacker.active->nature)
			attacker.active->spa.stat = attacker.active->spa.stat * 11 / 10;
		else if (ADAMANT == attacker.active->nature or CAREFUL == attacker.active->nature or IMPISH == attacker.active->nature or JOLLY == attacker.active->nature)
			attacker.active->spa.stat = attacker.active->spa.stat * 9 / 10;

		if (attacker.active->spa.stage >= 0)	// >= is better than == to reduce the frequency of checking for a CH
			attacker.active->spa.stat = attacker.active->spa.stat * (2 + attacker.active->spa.stage) / 2;
		else {
			if (!attacker.active->move->ch)
				attacker.active->spa.stat = attacker.active->spa.stat * 2 / (2 - attacker.active->spa.stage);
		}

		if (SOLAR_POWER == attacker.active->ability and 0 != weather.sun)
			attacker.active->spa.stat = attacker.active->spa.stat * 3 / 2;

		if ((CHOICE_SPECS == attacker.active->item) or (SOUL_DEW == attacker.active->item and (LATIAS == attacker.active->name or LATIOS == attacker.active->name)))
			attacker.active->spa.stat = attacker.active->spa.stat * 3 / 2;
		else if ((DEEPSEATOOTH == attacker.active->item and CLAMPERL == attacker.active->name) or (LIGHT_BALL == attacker.active->item and PIKACHU == attacker.active->name))
			attacker.active->spa.stat *= 2;

		if (attacker.active->spa.stat == 0)
			attacker.active->spa.stat = 1;
	}
}

void defense (const Pokemon &attacker, Team &defender, const Weather &weather) {
	if (attacker.move->physical) {
		defender.active->def.stat = (2 * defender.active->def.base + defender.active->def.iv + defender.active->def.ev) * defender.active->level / 100 + 5;
		if (BOLD == defender.active->nature or IMPISH == defender.active->nature or LAX == defender.active->nature or RELAXED == defender.active->nature)
			defender.active->def.stat = defender.active->def.stat * 11 / 10;
		else if (GENTLE == defender.active->nature or HASTY == defender.active->nature or LONELY == defender.active->nature or MILD == defender.active->nature)
			defender.active->def.stat = defender.active->def.stat * 9 / 10;

		if (defender.active->def.stage > 0) {	// > is better than == to reduce the frequency of checking for a CH
			if (!attacker.move->ch)
				defender.active->def.stat = defender.active->def.stat * (2 + defender.active->def.stage) / 2;
		}
		else
			defender.active->def.stat = defender.active->def.stat * 2 / (2 - defender.active->def.stage);

		if (MARVEL_SCALE == defender.active->ability and NO_STATUS != defender.active->status)
			defender.active->def.stat = defender.active->def.stat * 3 / 2;
		
		if (METAL_POWDER == defender.active->item and DITTO == defender.active->name)
			defender.active->def.stat = defender.active->def.stat * 3 / 2;
		
		if (EXPLOSION == attacker.move->name or SELFDESTRUCT == attacker.move->name)
			defender.active->def.stat /= 2;
	
		if (0 == defender.active->def.stat)
			defender.active->def.stat = 1;
	}
	else {
		defender.active->spd.stat = (2 * defender.active->spd.base + defender.active->spd.iv + defender.active->spd.ev) * defender.active->level / 100 + 5;
		if (CALM == defender.active->nature or CAREFUL == defender.active->nature or GENTLE == defender.active->nature or SASSY == defender.active->nature)
			defender.active->spd.stat = defender.active->spd.stat * 11 / 10;
		else if (LAX == defender.active->nature or NAIVE == defender.active->nature or NAUGHTY == defender.active->nature or RASH == defender.active->nature)
			defender.active->spd.stat = defender.active->spd.stat * 9 / 10;

		if (defender.active->spd.stage > 0) {	// > is better than == to reduce the frequency of checking for a CH
			if (!attacker.move->ch)
				defender.active->spd.stat = defender.active->spd.stat * (2 + defender.active->spd.stage) / 2;
		}
		else
			defender.active->spd.stat = defender.active->spd.stat * 2 / (2 - defender.active->spd.stage);

		if (FLOWER_GIFT == defender.active->ability and 0 != weather.sun)
			defender.active->spd.stat = defender.active->spd.stat * 3 / 2;
		
		if (DEEPSEASCALE == defender.active->item and CLAMPERL == defender.active->name)
			defender.active->spd.stat *= 2;
		else if ((METAL_POWDER == defender.active->item and DITTO == defender.active->name) or (SOUL_DEW == defender.active->item and (LATIAS == defender.active->name or LATIOS == defender.active->name)))
			defender.active->spd.stat = defender.active->spd.stat * 3 / 2;
		
		if (istype (defender, ROCK) and 0 != weather.sand)
			defender.active->spd.stat = defender.active->spd.stat * 3 / 2;
		
		if (0 == defender.active->spd.stat)
			defender.active->spd.stat = 1;
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
	else if (0 != team.slow_start)
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
		faster_pokemon (team1, team2, weather, faster, slower);
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

void faster_pokemon (Team &team1, Team &team2, const Weather &weather, Team* &faster, Team* &slower) {
	if (team1.active->spe.stat > team2.active->spe.stat) {
		faster = &team1;
		slower = &team2;
	}
	else if (team2.active->spe.stat > team1.active->spe.stat) {
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

void Stat::boost (int n) {
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
