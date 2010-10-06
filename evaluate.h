// Evaluate the state of the game
// Copyright 2010 David Stone
//
// This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public License
// as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License along with this program.  If not, see <http://www.gnu.org/licenses/>.

#ifndef EVALUATE_H_
#define EVALUATE_H_

#include <climits>
#include "move.h"
#include "pokemon.h"
#include "status.h"
#include "team.h"
#include "weather.h"

const int VICTORY = 1048576;

int scorepokemon (const pokemon &member, const teams &foe, const weathers &weather);
int scoremove (const moves &move, const pokemon &member, const teams &foe, const weathers &weather);

int evaluate (const teams &ai, const teams &foe, const weathers &weather) {
/*	int score = 0;
	for (std::vector<pokemon>::const_iterator it = ai.member.begin(); it != ai.member.end(); ++it)
		score += scorepokemon (*it, foe, weather);
	for (std::vector<pokemon>::const_iterator it = foe.member.begin(); it != foe.member.end(); ++it)
		score -= scorepokemon (*it, ai, weather);

*/	int score = 100 * ai.active->hp.stat / ai.active->hp.max - 100 * foe.active->hp.stat / foe.active->hp.max;
	return score;
}

int scorepokemon (const pokemon &member, const teams &foe, const weathers &weather) {
	int score = 0;
	if (member.hp.stat != 0) {
		score += 1024;
		score += 1024 * member.hp.stat / member.hp.max;	// Each % is worth about 10 points
/*		for (std::vector<moves>::const_iterator it = member.moveset.begin(); it != member.moveset.end(); ++it)
			score += scoremove (*it, member, foe, weather);
		if (member.aqua_ring)
			score += 1;
		if (member.imprison)
			score += 1;
		if (member.ingrain)
			score += 1;
		if (member.curse)
			score -= 1;
		if (member.item_unburden or member.item_recycle)
			score -= 1;
		if (member.leech_seed)
			score -= 1;
		if (foe.active->leech_seed)
			score += 1 * foe.active->hp.max / member.hp.max;
		if (member.loaf)
			score -= 1;
		if (member.nightmare)
			score -= 1;
		if (member.torment)
			score -= 1;
		if (member.trapped)
			score -= 1;
*/		if (member.status != NO_STATUS)
			score -= 10;
		score += 1 * member.atk.stage;
		score += 1 * member.def.stage;
		score += 1 * member.spa.stage;
		score += 1 * member.spd.stage;
		score += 1 * member.spe.stage;
	}
	return score;
}

int scoremove (const moves &move, const pokemon &member, const teams &foe, const weathers &weather) {
	int score = 0;
	if (move.pp == 0)
		score -= 256;					// Each move with 0 PP has a penalty equal to 25% of the Pokemon's HP
	return score;
}

int win (const teams &team) {
	if (team.active->hp.stat == 0) {
		if (team.me)
			return -VICTORY;
		return VICTORY;
	}
	return 0;
}

#endif
