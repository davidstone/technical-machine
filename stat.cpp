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
