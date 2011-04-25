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
	for (std::vector<Pokemon>::const_iterator it = ai.active.set.begin(); it != ai.active.set.end(); ++it)
		score += scorepokemon (*it, ai, foe, weather, sv);
	for (std::vector<Pokemon>::const_iterator it = foe.active.set.begin(); it != foe.active.set.end(); ++it)
		score -= scorepokemon (*it, foe, ai, weather, sv);
	return score;
}

long scorepokemon (const Pokemon &member, const Team &team, const Team &other, const Weather &weather, const score_variables &sv) {
	long score = team.stealth_rock * sv.stealth_rock * effectiveness [ROCK] [member.type1] * effectiveness [ROCK] [member.type2] / 4;
	if (grounded (member, weather))
		score += team.spikes * sv.spikes + team.toxic_spikes * sv.toxic_spikes;
	if (member.hp.stat != 0) {
		score += sv.members;
		score += sv.hp * member.hp.stat / member.hp.max;
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
//		if (other.active->leech_seed)
//			score += 1 * other.active->hp.max / member.hp.max;
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
		for (std::vector<Move>::const_iterator it = member.move.set.begin(); it != member.move.set.end(); ++it)
			score += scoremove (*it, team, other, weather, sv);
	}
	return score;
}


long scoremove (const Move &move, const Team &team, const Team &other, const Weather &weather, const score_variables &sv) {
	long score = 0;
	if (move.physical)
		score += other.reflect * sv.reflect;
	else if (move.basepower > 0)
		score += other.light_screen * sv.light_screen;
	if (move.pp == 0)
		score += sv.no_pp;
	return score;
}


long win (const Team &team) {
	if (team.active.set.size() == 0) {
		if (team.me)
			return -VICTORY;
		return VICTORY;
	}
	return 0;
}

score_variables::score_variables () {
	std::ifstream file ("evaluate.txt");
	std::string line;
	for (getline (file, line); !file.eof(); getline (file, line)) {
		size_t x = line.find ('\t');
		std::string data = line.substr (0, x);
		if (data == "Transposition Table")
			transposition_table = boost::lexical_cast<int> (line.substr (x + 1));
		else if (data == "Light Screen")
			light_screen = boost::lexical_cast<int> (line.substr (x + 1));
		else if (data == "Lucky Chant")
			lucky_chant = boost::lexical_cast<int> (line.substr (x + 1));
		else if (data == "Mist")
			mist = boost::lexical_cast<int> (line.substr (x + 1));
		else if (data == "Reflect")
			reflect = boost::lexical_cast<int> (line.substr (x + 1));
		else if (data == "Safeguard")
			safeguard = boost::lexical_cast<int> (line.substr (x + 1));
		else if (data == "Tailwind")
			tailwind = boost::lexical_cast<int> (line.substr (x + 1));
		else if (data == "Wish")
			wish = boost::lexical_cast<int> (line.substr (x + 1));
		else if (data == "Spikes")
			spikes = boost::lexical_cast<int> (line.substr (x + 1));
		else if (data == "Stealth Rock")
			stealth_rock = boost::lexical_cast<int> (line.substr (x + 1));
		else if (data == "Toxic Spikes")
			toxic_spikes = boost::lexical_cast<int> (line.substr (x + 1));
		else if (data == "Members")
			members = boost::lexical_cast<int> (line.substr (x + 1));
		else if (data == "HP")
			hp = boost::lexical_cast<int> (line.substr (x + 1));
		else if (data == "Aqua Ring")
			aqua_ring = boost::lexical_cast<int> (line.substr (x + 1));
		else if (data == "Curse")
			curse = boost::lexical_cast<int> (line.substr (x + 1));
		else if (data == "Imprison")
			imprison = boost::lexical_cast<int> (line.substr (x + 1));
		else if (data == "Ingrain")
			ingrain = boost::lexical_cast<int> (line.substr (x + 1));
		else if (data == "Leech Seed")
			leech_seed = boost::lexical_cast<int> (line.substr (x + 1));
		else if (data == "Loaf")
			loaf = boost::lexical_cast<int> (line.substr (x + 1));
		else if (data == "Nightmare")
			nightmare = boost::lexical_cast<int> (line.substr (x + 1));
		else if (data == "Torment")
			torment = boost::lexical_cast<int> (line.substr (x + 1));
		else if (data == "Trapped")
			trapped = boost::lexical_cast<int> (line.substr (x + 1));
		else if (data == "Burn")
			burn = boost::lexical_cast<int> (line.substr (x + 1));
		else if (data == "Freeze")
			freeze = boost::lexical_cast<int> (line.substr (x + 1));
		else if (data == "Paralysis")
			paralysis = boost::lexical_cast<int> (line.substr (x + 1));
		else if (data == "Poison")
			poison = boost::lexical_cast<int> (line.substr (x + 1));
		else if (data == "Sleep")
			sleep = boost::lexical_cast<int> (line.substr (x + 1));
		else if (data == "No PP")
			no_pp = boost::lexical_cast<int> (line.substr (x + 1));
	}
	file.close();
}

}
