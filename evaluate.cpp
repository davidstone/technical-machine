// Evaluate the state of the game
// Copyright (C) 2011 David Stone
//
// This file is part of Technical Machine.
//
// Technical Machine is free software: you can redistribute it and / or modify
// it under the terms of the GNU Affero General Public License as
// published by the Free Software Foundation, either version 3 of the
// License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.

#include "evaluate.h"

#include <cstddef>
#include <cstdint>
#include <fstream>
#include <string>

#include <boost/lexical_cast.hpp>

#include "ability.h"
#include "move.h"
#include "pokemon.h"
#include "team.h"
#include "weather.h"

namespace technicalmachine {

int64_t Score::evaluate (Team & ai, Team & foe, Weather const & weather) const {
	int64_t score = scoreteam (ai) - scoreteam (foe);

	size_t index = ai.pokemon.index;
	for (ai.pokemon.index = 0; ai.pokemon.index != ai.pokemon.set.size(); ++ai.pokemon.index)
		score += scorepokemon (ai, foe, weather);
	ai.pokemon.index = index;

	index = foe.pokemon.index;
	for (foe.pokemon.index = 0; foe.pokemon.index != foe.pokemon.set.size(); ++foe.pokemon.index)
		score -= scorepokemon (foe, ai, weather);
	foe.pokemon.index = index;
	return score;
}

int64_t Score::scoreteam (Team const & team) const {
	int64_t score = lucky_chant * team.lucky_chant + mist * team.mist + safeguard * team.safeguard + tailwind * team.tailwind + wish * team.wish;
	if (team.pokemon->hp.stat != 0) {
		score += team.stage [Stat::ATK] * atk_stage;
		score += team.stage [Stat::DEF] * def_stage;
		score += team.stage [Stat::SPA] * spa_stage;
		score += team.stage [Stat::SPD] * spd_stage;
		score += team.stage [Stat::SPE] * spe_stage;
		score += team.magnet_rise * magnet_rise;
		if (team.substitute)
			score += substitute + substitute_hp * team.substitute / team.pokemon->hp.max;
		if (team.aqua_ring)
			score += aqua_ring;
		if (team.curse)
			score += curse;
		if (team.imprison)
			score += imprison;
		if (team.ingrain)
			score += ingrain;
		if (team.leech_seed)
			score += leech_seed;
	//	if (other.pokemon->leech_seed)
	//		score += 1 * other.pokemon->hp.max / member.hp.max;
		if (team.loaf)
			score += loaf;
		if (team.nightmare)
			score += nightmare;
		if (team.torment)
			score += torment;
		if (team.trapped)
			score += trapped;
		if (team.focus_energy)
			score += focus_energy;
		for (std::vector<Move>::const_iterator move = team.pokemon->move.set.begin(); move->name != Move::STRUGGLE; ++move) {
			if (move->name == Move::BATON_PASS) {
				score += baton_pass * (team.aqua_ring * aqua_ring + team.focus_energy * focus_energy + team.ingrain * ingrain + team.magnet_rise * magnet_rise + team.stage [Stat::ATK] * atk_stage + team.stage [Stat::DEF] * def_stage + team.stage [Stat::SPA] * spa_stage + team.stage [Stat::SPD] * spd_stage + team.stage [Stat::SPE] * spe_stage);
				if (team.substitute)
					score += baton_pass * substitute;
				break;
			}
		}
	}
	return score;
}

int64_t Score::scorepokemon (Team const & team, Team const & other, Weather const & weather) const {
	int64_t score = team.stealth_rock * stealth_rock * static_cast <int> (get_effectiveness (ROCK, *team.pokemon)) / 4;
	if (grounded (team, weather))
		score += team.spikes * spikes + team.toxic_spikes * toxic_spikes;
	if (team.pokemon->hp.stat != 0) {
		score += members;
		score += hp * team.pokemon->hp.stat / team.pokemon->hp.max;
		switch (team.pokemon->status.name) {
			case Status::BURN:
				score += burn;
				break;
			case Status::FREEZE:
				score += freeze;
				break;
			case Status::PARALYSIS:
				score += paralysis;
				break;
			case Status::POISON_NORMAL:
				score += poison;
				break;
			case Status::POISON_TOXIC:
				score += poison;		// fix
				break;
			case Status::REST:
			case Status::SLEEP:
				score += sleep;
				break;
			default:
				break;
		}
		score += scoremove (team, other, weather);
	}
	return score;
}


int64_t Score::scoremove (Team const & team, Team const & other, Weather const & weather) const {
	int64_t score = 0;
	for (Move const & move : team.pokemon->move.set) {
		if (move.physical)
			score += other.reflect * reflect;
		else if (move.basepower > 0)		// Non-damaging moves have physical == false
			score += other.light_screen * light_screen;
		if (move.pp == 0)
			score += no_pp;
	}
	return score;
}


int64_t Score::win (Team const & team) {
	if (team.pokemon.set.size() == 0) {
		if (team.me)
			return -VICTORY;
		return VICTORY;
	}
	return 0;
}

Score::Score () {
	load_evaluation_constants ();
}

void Score::load_evaluation_constants () {
	std::ifstream file ("evaluate.txt");
	std::string line;
	std::string const delimiter = ": ";
	for (getline (file, line); !file.eof(); getline (file, line)) {
		size_t x = line.find (delimiter);
		std::string data = line.substr (0, x);
		if (data == "Transposition Table") {
			transposition_table = boost::lexical_cast<int> (line.substr (x + delimiter.length ()));
		}
		else if (data == "Light Screen") {
			light_screen = boost::lexical_cast<int> (line.substr (x + delimiter.length ()));
		}
		else if (data == "Lucky Chant") {
			lucky_chant = boost::lexical_cast<int> (line.substr (x + delimiter.length ()));
		}
		else if (data == "Mist") {
			mist = boost::lexical_cast<int> (line.substr (x + delimiter.length ()));
		}
		else if (data == "Reflect") {
			reflect = boost::lexical_cast<int> (line.substr (x + delimiter.length ()));
		}
		else if (data == "Safeguard") {
			safeguard = boost::lexical_cast<int> (line.substr (x + delimiter.length ()));
		}
		else if (data == "Tailwind") {
			tailwind = boost::lexical_cast<int> (line.substr (x + delimiter.length ()));
		}
		else if (data == "Wish") {
			wish = boost::lexical_cast<int> (line.substr (x + delimiter.length ()));
		}
		else if (data == "Spikes") {
			spikes = boost::lexical_cast<int> (line.substr (x + delimiter.length ()));
		}
		else if (data == "Stealth Rock") {
			stealth_rock = boost::lexical_cast<int> (line.substr (x + delimiter.length ()));
		}
		else if (data == "Toxic Spikes") {
			toxic_spikes = boost::lexical_cast<int> (line.substr (x + delimiter.length ()));
		}
		else if (data == "Members") {
			members = boost::lexical_cast<int> (line.substr (x + delimiter.length ()));
		}
		else if (data == "HP") {
			hp = boost::lexical_cast<int> (line.substr (x + delimiter.length ()));
		}
		else if (data == "Aqua Ring") {
			aqua_ring = boost::lexical_cast<int> (line.substr (x + delimiter.length ()));
		}
		else if (data == "Curse") {
			curse = boost::lexical_cast<int> (line.substr (x + delimiter.length ()));
		}
		else if (data == "Imprison") {
			imprison = boost::lexical_cast<int> (line.substr (x + delimiter.length ()));
		}
		else if (data == "Ingrain") {
			ingrain = boost::lexical_cast<int> (line.substr (x + delimiter.length ()));
		}
		else if (data == "Leech Seed") {
			leech_seed = boost::lexical_cast<int> (line.substr (x + delimiter.length ()));
		}
		else if (data == "Loaf") {
			loaf = boost::lexical_cast<int> (line.substr (x + delimiter.length ()));
		}
		else if (data == "Magnet Rise") {
			magnet_rise = boost::lexical_cast<int> (line.substr (x + delimiter.length ()));
		}
		else if (data == "Nightmare") {
			nightmare = boost::lexical_cast<int> (line.substr (x + delimiter.length ()));
		}
		else if (data == "Substitute") {
			substitute = boost::lexical_cast<int> (line.substr (x + delimiter.length ()));
		}
		else if (data == "Substitute HP") {
			substitute_hp = boost::lexical_cast<int> (line.substr (x + delimiter.length ()));
		}
		else if (data == "Torment") {
			torment = boost::lexical_cast<int> (line.substr (x + delimiter.length ()));
		}
		else if (data == "Trapped") {
			trapped = boost::lexical_cast<int> (line.substr (x + delimiter.length ()));
		}
		else if (data == "Burn") {
			burn = boost::lexical_cast<int> (line.substr (x + delimiter.length ()));
		}
		else if (data == "Freeze") {
			freeze = boost::lexical_cast<int> (line.substr (x + delimiter.length ()));
		}
		else if (data == "Paralysis") {
			paralysis = boost::lexical_cast<int> (line.substr (x + delimiter.length ()));
		}
		else if (data == "Poison") {
			poison = boost::lexical_cast<int> (line.substr (x + delimiter.length ()));
		}
		else if (data == "Sleep") {
			sleep = boost::lexical_cast<int> (line.substr (x + delimiter.length ()));
		}
		else if (data == "Attack stage") {
			atk_stage = boost::lexical_cast<int> (line.substr (x + delimiter.length ()));
		}
		else if (data == "Defense stage") {
			def_stage = boost::lexical_cast<int> (line.substr (x + delimiter.length ()));
		}
		else if (data == "Special Attack stage") {
			spa_stage = boost::lexical_cast<int> (line.substr (x + delimiter.length ()));
		}
		else if (data == "Special Defense stage") {
			spd_stage = boost::lexical_cast<int> (line.substr (x + delimiter.length ()));
		}
		else if (data == "Speed stage") {
			spe_stage = boost::lexical_cast<int> (line.substr (x + delimiter.length ()));
		}
		else if (data == "Focus Energy") {
			focus_energy = boost::lexical_cast<int> (line.substr (x + delimiter.length ()));
		}
		else if (data == "Baton Pass") {
			baton_pass = boost::lexical_cast<int> (line.substr (x + delimiter.length ()));
		}
		else if (data == "No PP") {
			no_pp = boost::lexical_cast<int> (line.substr (x + delimiter.length ()));
		}
	}
	file.close();
}

}
