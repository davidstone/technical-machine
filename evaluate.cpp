// Evaluate the state of the game
// Copyright 2011 David Stone
//
// This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public License
// as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "evaluate.h"
#include "move.h"
#include "pokemon.h"
#include "team.h"
#include "weather.h"

namespace technicalmachine {

long evaluate (const Team &ai, const Team &foe, const Weather &weather, const score_variables &sv) {
	long score = (ai.lucky_chant - foe.lucky_chant) * sv.lucky_chant + (ai.mist - foe.mist) * sv.mist + (ai.safeguard - foe.safeguard) * sv.safeguard + (ai.tailwind - foe.tailwind) * sv.tailwind + (ai.wish - foe.wish) * sv.wish;
	for (std::vector<Pokemon>::const_iterator it = ai.member.begin(); it != ai.member.end(); ++it)
		score += scorepokemon (*it, ai, foe, weather, sv);
	for (std::vector<Pokemon>::const_iterator it = foe.member.begin(); it != foe.member.end(); ++it)
		score -= scorepokemon (*it, foe, ai, weather, sv);
	return score;
}

long scorepokemon (const Pokemon &member, const Team &ai, const Team &foe, const Weather &weather, const score_variables &sv) {
	long score = ai.stealth_rock * sv.stealth_rock * effectiveness [ROCK] [member.type1] * effectiveness [ROCK] [member.type2] / 4;
	if (grounded (member, weather))
		score += ai.spikes * sv.spikes + ai.toxic_spikes * sv.toxic_spikes;
	if (member.hp.stat != 0) {
		score += sv.members;
		score += sv.hp * member.hp.stat / member.hp.max;	// Each % is worth about 10 points
		if (member.aqua_ring)
			score += sv.aqua_ring;
		if (member.curse)
			score += sv.curse;
		if (member.imprison)
			score += sv.imprison;
		if (member.ingrain)
			score += sv.ingrain;
		if (member.leech_seed)
			score += sv.leech_seed;
//		if (foe.active->leech_seed)
//			score += 1 * foe.active->hp.max / member.hp.max;
		if (member.loaf)
			score += sv.loaf;
		if (member.nightmare)
			score += sv.nightmare;
		if (member.torment)
			score += sv.torment;
		if (member.trapped)
			score += sv.trapped;
		if (member.status == BURN)
			score += sv.burn;
		else if (member.status == FREEZE)
			score += sv.freeze;
		else if (member.status == PARALYSIS)
			score += sv.paralysis;
		else if (member.status == POISON_NORMAL or member.status == POISON_TOXIC)
			score += sv.poison;
		else if (member.status == SLEEP)
			score += sv.sleep;
//		score += 1 * member.atk.stage;
//		score += 1 * member.def.stage;
//		score += 1 * member.spa.stage;
//		score += 1 * member.spd.stage;
//		score += 1 * member.spe.stage;
		for (std::vector<moves>::const_iterator it = member.moveset.begin(); it != member.moveset.end(); ++it)
			score += scoremove (*it, ai, foe, weather, sv);
	}
	return score;
}


long scoremove (const moves &move, const Team &ai, const Team &foe, const Weather &weather, const score_variables &sv) {
	long score = 0;
	if (move.physical)
		score += foe.reflect * sv.reflect;
	else
		score += foe.light_screen * sv.light_screen;
	if (move.pp == 0)
		score -= 256;					// Each move with 0 PP has a penalty equal to 25% of the Pokemon's HP
	return score;
}


long int win (const Team &team) {
	if (team.member.size() == 1 and team.active->hp.stat == 0) {
		if (team.me)
			return -VICTORY;
		return VICTORY;
	}
	return 0;
}

}
