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

#include "evaluate.hpp"

#include <cstddef>
#include <cstdint>
#include <fstream>
#include <string>
#include <vector>

#include "ability.hpp"
#include "move.hpp"
#include "pokemon.hpp"
#include "team.hpp"
#include "weather.hpp"

namespace technicalmachine {
namespace {
class ResetIndex {
	public:
		ResetIndex (Team & team):
			reset (team),
			index (team.pokemon.index) {
		}
		~ResetIndex () {
			reset.pokemon.index = index;
		}
	private:
		Team & reset;
		uint8_t const index;
};

}	// unnamed namespace

int64_t Score::evaluate (Team & ai, Team & foe, Weather const & weather) const {
	int64_t score = score_team (ai) - score_team (foe);
	return score + score_all_pokemon (ai, foe, weather) - score_all_pokemon (foe, ai, weather);
}

int64_t Score::score_team (Team const & team) const {
	int64_t score = lucky_chant * team.lucky_chant + mist * team.mist + safeguard * team.safeguard + tailwind * team.tailwind + wish * team.wish;
	if (team.pokemon().hp.stat != 0) {
		score += team.stage [Stat::ATK] * atk_stage;
		score += team.stage [Stat::DEF] * def_stage;
		score += team.stage [Stat::SPA] * spa_stage;
		score += team.stage [Stat::SPD] * spd_stage;
		score += team.stage [Stat::SPE] * spe_stage;
		score += team.magnet_rise * magnet_rise;
		if (team.substitute)
			score += substitute + substitute_hp * team.substitute / team.pokemon().hp.max;
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
	//	if (other.pokemon().leech_seed)
	//		score += 1 * other.pokemon().hp.max / member.hp.max;
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
		for (std::vector<Move>::const_iterator move = team.pokemon().move.set.cbegin(); move->name != Move::STRUGGLE; ++move) {
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

int64_t Score::score_all_pokemon (Team & team, Team const & other, Weather const & weather) const {
	ResetIndex index (team);
	int64_t score = 0;
	for (team.pokemon.index = 0; team.pokemon.index != team.pokemon.set.size(); ++team.pokemon.index)
		score += score_pokemon (team, other, weather);
	return score;
}

int64_t Score::score_pokemon (Team const & team, Team const & other, Weather const & weather) const {
	int64_t score = team.stealth_rock * stealth_rock * static_cast <int> (get_effectiveness (Type::ROCK, team.pokemon())) / 4;
	if (grounded (team, weather))
		score += team.spikes * spikes + team.toxic_spikes * toxic_spikes;
	if (team.pokemon().hp.stat != 0) {
		score += members;
		score += hp * team.pokemon().hp.stat / team.pokemon().hp.max;
		score += score_status (team);
		score += score_move (team, other, weather);
	}
	return score;
}

int64_t Score::score_status (Team const & team) const {
	switch (team.pokemon().status.name) {
		case Status::BURN:
			return burn;
		case Status::FREEZE:
			return freeze;
		case Status::PARALYSIS:
			return paralysis;
		case Status::POISON:
			return poison;
		case Status::POISON_TOXIC:
			return poison * team.toxic / 2;
		case Status::REST:
		case Status::SLEEP:
			return sleep;
		default:
			return 0;
	}
}

int64_t Score::score_move (Team const & team, Team const & other, Weather const & weather) const {
	int64_t score = 0;
	for (Move const & move : team.pokemon().move.set) {
		if (move.physical)
			score += other.reflect * reflect;
		else if (move.basepower > 0)		// Non-damaging moves have physical == false
			score += other.light_screen * light_screen;
		if (move.is_out_of_pp())
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

int64_t Score::sleep_clause (Team const & team) {
	bool sleep_clause = false;
	for (Pokemon const & pokemon : team.pokemon.set) {
		if (pokemon.status.name == Status::SLEEP) {
			if (sleep_clause == false)
				sleep_clause = true;
			else
				return team.me ? VICTORY : -VICTORY;
		}
	}
	return 0;
}

Score::Score ():
	transposition_table (1),	
	light_screen (0),
	lucky_chant (0),
	mist (0),
	reflect (0),
	safeguard (0),
	tailwind (0),

	wish (0),

	spikes (0),
	stealth_rock (0),
	toxic_spikes (0),

	members (0),
	hp (0),
	aqua_ring (0),
	curse (0),
	imprison (0),
	ingrain (0),
	leech_seed (0),
	loaf (0),
	magnet_rise (0),
	nightmare (0),
	substitute (0),
	substitute_hp (0),
	torment (0),
	trapped (0),

	burn (0),
	freeze (0),
	paralysis (0),
	poison (0),
	sleep (0),
	
	atk_stage (0),
	def_stage (0),
	spa_stage (0),
	spd_stage (0),
	spe_stage (0),
	focus_energy (0),

	baton_pass (0),
	no_pp (0)
 {
	// This is a separate function instead of being stuck in directly to support
	// reloading of the constants.
	load_evaluation_constants ();
}

void Score::load_evaluation_constants () {
	std::ifstream file ("settings/evaluate.txt");
	std::string line;
	std::string const delimiter = ": ";
	for (getline (file, line); !file.eof(); getline (file, line)) {
		size_t const x = line.find (delimiter);
		std::string data = line.substr (0, x);
		if (data == "Transposition Table") {
			transposition_table = std::stoi (line.substr (x + delimiter.length ()));
		}
		else if (data == "Light Screen") {
			light_screen = std::stoi (line.substr (x + delimiter.length ()));
		}
		else if (data == "Lucky Chant") {
			lucky_chant = std::stoi (line.substr (x + delimiter.length ()));
		}
		else if (data == "Mist") {
			mist = std::stoi (line.substr (x + delimiter.length ()));
		}
		else if (data == "Reflect") {
			reflect = std::stoi (line.substr (x + delimiter.length ()));
		}
		else if (data == "Safeguard") {
			safeguard = std::stoi (line.substr (x + delimiter.length ()));
		}
		else if (data == "Tailwind") {
			tailwind = std::stoi (line.substr (x + delimiter.length ()));
		}
		else if (data == "Wish") {
			wish = std::stoi (line.substr (x + delimiter.length ()));
		}
		else if (data == "Spikes") {
			spikes = std::stoi (line.substr (x + delimiter.length ()));
		}
		else if (data == "Stealth Rock") {
			stealth_rock = std::stoi (line.substr (x + delimiter.length ()));
		}
		else if (data == "Toxic Spikes") {
			toxic_spikes = std::stoi (line.substr (x + delimiter.length ()));
		}
		else if (data == "Members") {
			members = std::stoi (line.substr (x + delimiter.length ()));
		}
		else if (data == "HP") {
			hp = std::stoi (line.substr (x + delimiter.length ()));
		}
		else if (data == "Aqua Ring") {
			aqua_ring = std::stoi (line.substr (x + delimiter.length ()));
		}
		else if (data == "Curse") {
			curse = std::stoi (line.substr (x + delimiter.length ()));
		}
		else if (data == "Imprison") {
			imprison = std::stoi (line.substr (x + delimiter.length ()));
		}
		else if (data == "Ingrain") {
			ingrain = std::stoi (line.substr (x + delimiter.length ()));
		}
		else if (data == "Leech Seed") {
			leech_seed = std::stoi (line.substr (x + delimiter.length ()));
		}
		else if (data == "Loaf") {
			loaf = std::stoi (line.substr (x + delimiter.length ()));
		}
		else if (data == "Magnet Rise") {
			magnet_rise = std::stoi (line.substr (x + delimiter.length ()));
		}
		else if (data == "Nightmare") {
			nightmare = std::stoi (line.substr (x + delimiter.length ()));
		}
		else if (data == "Substitute") {
			substitute = std::stoi (line.substr (x + delimiter.length ()));
		}
		else if (data == "Substitute HP") {
			substitute_hp = std::stoi (line.substr (x + delimiter.length ()));
		}
		else if (data == "Torment") {
			torment = std::stoi (line.substr (x + delimiter.length ()));
		}
		else if (data == "Trapped") {
			trapped = std::stoi (line.substr (x + delimiter.length ()));
		}
		else if (data == "Burn") {
			burn = std::stoi (line.substr (x + delimiter.length ()));
		}
		else if (data == "Freeze") {
			freeze = std::stoi (line.substr (x + delimiter.length ()));
		}
		else if (data == "Paralysis") {
			paralysis = std::stoi (line.substr (x + delimiter.length ()));
		}
		else if (data == "Poison") {
			poison = std::stoi (line.substr (x + delimiter.length ()));
		}
		else if (data == "Sleep") {
			sleep = std::stoi (line.substr (x + delimiter.length ()));
		}
		else if (data == "Attack stage") {
			atk_stage = std::stoi (line.substr (x + delimiter.length ()));
		}
		else if (data == "Defense stage") {
			def_stage = std::stoi (line.substr (x + delimiter.length ()));
		}
		else if (data == "Special Attack stage") {
			spa_stage = std::stoi (line.substr (x + delimiter.length ()));
		}
		else if (data == "Special Defense stage") {
			spd_stage = std::stoi (line.substr (x + delimiter.length ()));
		}
		else if (data == "Speed stage") {
			spe_stage = std::stoi (line.substr (x + delimiter.length ()));
		}
		else if (data == "Focus Energy") {
			focus_energy = std::stoi (line.substr (x + delimiter.length ()));
		}
		else if (data == "Baton Pass") {
			baton_pass = std::stoi (line.substr (x + delimiter.length ()));
		}
		else if (data == "No PP") {
			no_pp = std::stoi (line.substr (x + delimiter.length ()));
		}
	}
}

}
