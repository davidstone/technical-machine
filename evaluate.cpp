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

#include <cstddef>
#include <cstdint>
#include "evaluate.h"
#include "move.h"
#include "pokemon.h"
#include "team.h"
#include "weather.h"

namespace technicalmachine {

int64_t evaluate (Team &ai, Team &foe, Weather const &weather, score_variables const &sv) {
	int64_t score = scoreteam (ai, sv) - scoreteam (foe, sv);

	size_t index = ai.pokemon.index;
	for (ai.pokemon.index = 0; ai.pokemon.index != ai.pokemon.set.size(); ++ai.pokemon.index)
		score += scorepokemon (ai, foe, weather, sv);
	ai.pokemon.index = index;

	index = foe.pokemon.index;
	for (foe.pokemon.index = 0; foe.pokemon.index != foe.pokemon.set.size(); ++foe.pokemon.index)
		score -= scorepokemon (foe, ai, weather, sv);
	foe.pokemon.index = index;
	return score;
}

int64_t scoreteam (Team const &team, score_variables const &sv) {
	int64_t score = sv.lucky_chant * team.lucky_chant + sv.mist * team.mist + sv.safeguard * team.safeguard + sv.tailwind * team.tailwind + sv.wish * team.wish;
	if (team.pokemon->hp.stat != 0) {
		score += team.magnet_rise * sv.magnet_rise;
		score += sv.substitute * team.substitute / team.pokemon->hp.max;
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
	//	if (other.pokemon->leech_seed)
	//		score += 1 * other.pokemon->hp.max / member.hp.max;
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
		for (std::vector<Move>::const_iterator move = team.pokemon->move.set.begin(); move->name != STRUGGLE; ++move) {
			if (move->name == BATON_PASS) {
				score += sv.baton_pass * (team.aqua_ring * sv.aqua_ring + team.focus_energy * sv.focus_energy + team.ingrain * sv.ingrain + team.magnet_rise * sv.magnet_rise + team.substitute * sv.substitute + team.pokemon->atk.stage * sv.atk_stage + team.pokemon->def.stage * sv.def_stage + team.pokemon->spa.stage * sv.spa_stage + team.pokemon->spd.stage * sv.spd_stage + team.pokemon->spe.stage * sv.spe_stage);
				break;
			}
		}
	}
	return score;
}

int64_t scorepokemon (Team const &team, Team const &other, Weather const &weather, score_variables const &sv) {
	int64_t score = team.stealth_rock * sv.stealth_rock * static_cast <int> (effectiveness [ROCK] [team.pokemon->type1] * effectiveness [ROCK] [team.pokemon->type2]) / 4;
	if (grounded (team, weather))
		score += team.spikes * sv.spikes + team.toxic_spikes * sv.toxic_spikes;
	if (team.pokemon->hp.stat != 0) {
		score += sv.members;
		score += sv.hp * team.pokemon->hp.stat / team.pokemon->hp.max;
		switch (team.pokemon->status) {
			case BURN:
				score += sv.burn;
				break;
			case FREEZE:
				score += sv.freeze;
				break;
			case PARALYSIS:
				score += sv.paralysis;
				break;
			case POISON_NORMAL:
				score += sv.poison;
				break;
			case POISON_TOXIC:
				score += sv.poison;		// fix
				break;
			case SLEEP:
				score += sv.sleep;
				break;
			default:
				break;
		}
		score += team.pokemon->atk.stage * sv.atk_stage;
		score += team.pokemon->def.stage * sv.def_stage;
		score += team.pokemon->spa.stage * sv.spa_stage;
		score += team.pokemon->spd.stage * sv.spd_stage;
		score += team.pokemon->spe.stage * sv.spe_stage;
		score += scoremove (team, other, weather, sv);
	}
	return score;
}


int64_t scoremove (Team const &team, Team const &other, Weather const &weather, score_variables const &sv) {
	int64_t score = 0;
	for (std::vector<Move>::const_iterator move = team.pokemon->move.set.begin(); move != team.pokemon->move.set.end(); ++move) {
		if (move->physical)
			score += other.reflect * sv.reflect;
		else if (move->basepower > 0)		// Non-damaging moves have physical == false
			score += other.light_screen * sv.light_screen;
		if (move->pp == 0)
			score += sv.no_pp;
	}
	return score;
}


int64_t win (Team const &team) {
	if (team.pokemon.set.size() == 0) {
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
