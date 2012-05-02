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
#include <string>
#include <vector>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include "ability.hpp"
#include "move.hpp"
#include "pokemon.hpp"
#include "team.hpp"
#include "weather.hpp"

#include <iostream>

namespace technicalmachine {
namespace {
class ResetIndex {
	public:
		ResetIndex (Team & team):
			reset (team),
			index (team.pokemon.index()) {
		}
		~ResetIndex () {
			reset.pokemon.set_index(index);
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
		Move const * move_ptr = team.pokemon().move.find_if ([&](Move const & move) {
			return (move.name == Move::BATON_PASS);
		});
		if (move_ptr != nullptr) {
			int64_t const stat_stages = team.stage [Stat::ATK] * atk_stage + team.stage [Stat::DEF] * def_stage + team.stage [Stat::SPA] * spa_stage + team.stage [Stat::SPD] * spd_stage + team.stage [Stat::SPE] * spe_stage;
			score += baton_pass * (team.aqua_ring * aqua_ring + team.focus_energy * focus_energy + team.ingrain * ingrain + team.magnet_rise * magnet_rise + stat_stages);
			if (team.substitute)
				score += baton_pass * substitute;
		}
	}
	return score;
}

int64_t Score::score_all_pokemon (Team & team, Team const & other, Weather const & weather) const {
	ResetIndex const reset_index (team);
	int64_t score = 0;
	for (uint8_t index = 0; index != team.pokemon.size(); ++index) {
		team.pokemon.set_index(index);
		score += score_pokemon (team, other, weather);
	}
	return score;
}

int64_t Score::score_pokemon (Team const & team, Team const & other, Weather const & weather) const {
	Pokemon const & pokemon = team.pokemon();
	int64_t score = team.stealth_rock * stealth_rock * static_cast<int>(Type::stealth_rock_effectiveness(pokemon)) / 4;
	if (grounded (team, pokemon, weather))
		score += team.spikes * spikes + team.toxic_spikes * toxic_spikes;
	if (pokemon.hp.stat != 0) {
		score += members;
		score += hp * pokemon.hp.stat / pokemon.hp.max;
		score += hidden * pokemon.hidden;
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
	team.pokemon().move.for_each([&](Move const & move) {
		if (move.physical)
			score += other.reflect * reflect;
		else if (move.basepower > 0)		// Non-damaging moves have physical == false
			score += other.light_screen * light_screen;
		if (move.is_out_of_pp())
			score += no_pp;
	});
	return score;
}


int64_t Score::win (Team const & team) {
	if (team.pokemon.size() == 0)
		return team.me ? -VICTORY : VICTORY;
	return 0;
}

int64_t Score::sleep_clause (Team const & team) {
	unsigned const sleeper_count = team.pokemon.count_if([](Pokemon const & pokemon) {
		return (pokemon.status.name == Status::SLEEP);
	});
	if (sleeper_count > 1)
		return team.me ? VICTORY : -VICTORY;
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
	hidden (0),
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
	boost::property_tree::ptree file;
	read_xml ("settings/evaluate.xml", file);
	boost::property_tree::ptree const pt = file.get_child("score");
	transposition_table = pt.get<int>("transposition_table", 0);
	light_screen = pt.get<int>("light_screen", 0);
	lucky_chant = pt.get<int>("lucky_chant", 0);
	mist = pt.get<int>("mist", 0);
	reflect = pt.get<int>("reflect", 0);
	safeguard = pt.get<int>("safeguard", 0);
	tailwind = pt.get<int>("tailwind", 0);
	wish = pt.get<int>("wish", 0);
	spikes = pt.get<int>("spikes", 0);
	stealth_rock = pt.get<int>("stealth_rock", 0);
	toxic_spikes = pt.get<int>("toxic_spikes", 0);
	members = pt.get<int>("members", 0);
	hp = pt.get<int>("hp", 0);
	hidden = pt.get<int>("hidden", 0);
	aqua_ring = pt.get<int>("aqua_ring", 0);
	curse = pt.get<int>("curse", 0);
	imprison = pt.get<int>("imprison", 0);
	ingrain = pt.get<int>("ingrain", 0);
	leech_seed = pt.get<int>("leech_seed", 0);
	loaf = pt.get<int>("loaf", 0);
	magnet_rise = pt.get<int>("magnet_rise", 0);
	nightmare = pt.get<int>("nightmare", 0);
	substitute = pt.get<int>("substitute", 0);
	substitute_hp = pt.get<int>("substitute_hp", 0);
	torment = pt.get<int>("torment", 0);
	trapped = pt.get<int>("trapped", 0);
	burn = pt.get<int>("burn", 0);
	freeze = pt.get<int>("freeze", 0);
	paralysis = pt.get<int>("paralysis", 0);
	poison = pt.get<int>("poison", 0);
	sleep = pt.get<int>("sleep", 0);
	atk_stage = pt.get<int>("attack_stage", 0);
	def_stage = pt.get<int>("defense_stage", 0);
	spa_stage = pt.get<int>("special_attack_stage", 0);
	spd_stage = pt.get<int>("special_defense_stage", 0);
	spe_stage = pt.get<int>("speed_stage", 0);
	focus_energy = pt.get<int>("focus_energy", 0);
	baton_pass = pt.get<int>("baton_pass", 0);
	no_pp = pt.get<int>("no_pp", 0);
}

}	// namespace technicalmachine
