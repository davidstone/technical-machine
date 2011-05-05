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
#include "typefunction.h"
#include "weather.h"

namespace technicalmachine {

long evaluate (Team const &ai, Team const &foe, Weather const &weather, score_variables const &sv) {
	long score = (ai.lucky_chant - foe.lucky_chant) * sv.lucky_chant + (ai.mist - foe.mist) * sv.mist + (ai.safeguard - foe.safeguard) * sv.safeguard + (ai.tailwind - foe.tailwind) * sv.tailwind + (ai.wish - foe.wish) * sv.wish;
	score += scoreteam (ai, sv);
	for (std::vector<Pokemon>::const_iterator it = ai.active.set.begin(); it != ai.active.set.end(); ++it)
		score += scorepokemon (*it, ai, foe, weather, sv);
	score -= scoreteam (foe, sv);
	for (std::vector<Pokemon>::const_iterator it = foe.active.set.begin(); it != foe.active.set.end(); ++it)
		score -= scorepokemon (*it, foe, ai, weather, sv);
	return score;
}

long scoreteam (Team const &team, score_variables const &sv) {
	long score = 0;
	if (team.active->hp.stat != 0) {
		score += team.magnet_rise * sv.magnet_rise;
		score += sv.substitute * team.substitute / team.active->hp.max;
		if (team.aqua_ring)
			score += sv.aqua_ring;
		if (team.curse)
			score += sv.curse;
		if (team.imprison)
			score += sv.imprison;
		if (team.ingrain)
			score += sv.ingrain;
		if (team.leech_seed)
			score += sv.leech_seed;
	//	if (other.active->leech_seed)
	//		score += 1 * other.active->hp.max / member.hp.max;
		if (team.loaf)
			score += sv.loaf;
		if (team.nightmare)
			score += sv.nightmare;
		if (team.torment)
			score += sv.torment;
		if (team.trapped)
			score += sv.trapped;
		if (team.focus_energy)
			score += sv.focus_energy;
		bool bp = false;
		for (std::vector<Move>::const_iterator move = team.active->move.set.begin(); move != team.active->move.set.end(); ++move) {
			if (move->name == BATON_PASS) {
				bp = true;
				break;
			}
		}
		if (bp)
			score += sv.baton_pass * (team.aqua_ring * sv.aqua_ring + team.focus_energy * sv.focus_energy + team.ingrain * sv.ingrain + team.magnet_rise * sv.magnet_rise + team.substitute * sv.substitute + team.active->atk.stage * sv.atk_stage + team.active->def.stage * sv.def_stage + team.active->spa.stage * sv.spa_stage + team.active->spd.stage * sv.spd_stage + team.active->spe.stage * sv.spe_stage);
	}
	return score;
}

long scorepokemon (Pokemon const &member, Team const &team, Team const &other, Weather const &weather, score_variables const &sv) {
	long score = team.stealth_rock * sv.stealth_rock * effectiveness [ROCK] [member.type1] * effectiveness [ROCK] [member.type2] / 4;
	if (grounded (team, weather))
		score += team.spikes * sv.spikes + team.toxic_spikes * sv.toxic_spikes;
	if (member.hp.stat != 0) {
		score += sv.members;
		score += sv.hp * member.hp.stat / member.hp.max;
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
		score += member.atk.stage * sv.atk_stage;
		score += member.def.stage * sv.def_stage;
		score += member.spa.stage * sv.spa_stage;
		score += member.spd.stage * sv.spd_stage;
		score += member.spe.stage * sv.spe_stage;
		score += scoremove (member, team, other, weather, sv);
	}
	return score;
}


long scoremove (Pokemon const &member, Team const &team, Team const &other, Weather const &weather, score_variables const &sv) {
	long score = 0;
	for (std::vector<Move>::const_iterator move = member.move.set.begin(); move != member.move.set.end(); ++move) {
		if (move->physical)
			score += other.reflect * sv.reflect;
		else if (move->basepower > 0)		// Non-damaging moves have physical == false
			score += other.light_screen * sv.light_screen;
		if (move->pp == 0)
			score += sv.no_pp;
	}
	return score;
}


long win (Team const &team) {
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
		else if (data == "Magnet Rise")
			magnet_rise = boost::lexical_cast<int> (line.substr (x + 1));
		else if (data == "Nightmare")
			nightmare = boost::lexical_cast<int> (line.substr (x + 1));
		else if (data == "Substitute")
			substitute = boost::lexical_cast<int> (line.substr (x + 1));
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
		else if (data == "Attack stage")
			atk_stage = boost::lexical_cast<int> (line.substr (x + 1));
		else if (data == "Defense stage")
			def_stage = boost::lexical_cast<int> (line.substr (x + 1));
		else if (data == "Special Attack stage")
			spa_stage = boost::lexical_cast<int> (line.substr (x + 1));
		else if (data == "Special Defense stage")
			spd_stage = boost::lexical_cast<int> (line.substr (x + 1));
		else if (data == "Speed stage")
			spe_stage = boost::lexical_cast<int> (line.substr (x + 1));
		else if (data == "Focus Energy")
			focus_energy = boost::lexical_cast<int> (line.substr (x + 1));
		else if (data == "Baton Pass")
			baton_pass = boost::lexical_cast<int> (line.substr (x + 1));
		else if (data == "No PP")
			no_pp = boost::lexical_cast<int> (line.substr (x + 1));
	}
	file.close();
}

}
