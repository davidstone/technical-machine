// Evaluate the state of the game
// Copyright (C) 2012 David Stone
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

#include <cstdint>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include "team.hpp"

#include "move/move.hpp"
#include "move/moves.hpp"

#include "pokemon/collection.hpp"
#include "pokemon/pokemon.hpp"

namespace technicalmachine {
namespace {
class ResetIndex {
	public:
		ResetIndex (Team & team):
			reset (team),
			index (team.all_pokemon().index()) {
		}
		~ResetIndex () {
			reset.all_pokemon().set_index(index);
		}
	private:
		Team & reset;
		PokemonCollection::index_type const index;
};

}	// unnamed namespace

int64_t Score::evaluate (Team & ai, Team & foe, Weather const & weather) const {
	int64_t score = score_team (ai) - score_team (foe);
	return score + score_all_pokemon (ai, foe, weather) - score_all_pokemon (foe, ai, weather);
}

int64_t Score::score_team (Team const & team) const {
	int64_t score = lucky_chant * team.screens.m_lucky_chant.turns_remaining;
	score += mist * team.screens.m_mist.turns_remaining;
	score += safeguard * team.screens.m_safeguard.turns_remaining;
	score += tailwind * team.screens.m_tailwind.turns_remaining;
	score += wish * team.wish.is_active();
	if (team.pokemon().hp().stat != 0) {
		score += score_active_pokemon(team.pokemon());
	//	if (other.pokemon().leech_seed)
	//		score += 1 * other.pokemon().hp.max / member.hp.max;

		if (team.pokemon().get_pokemon().move.exists(Moves::Baton_Pass))
			score += baton_passable_score(team.pokemon()) * 2;
		else
			score += baton_passable_score(team.pokemon());
		
	}
	return score;
}

int64_t Score::score_active_pokemon(ActivePokemon const & pokemon) const {
	int64_t score = 0;
	if (pokemon.is_cursed())
		score += curse;
	if (pokemon.imprisoned())
		score += imprison;
	if (pokemon.leech_seed)
		score += leech_seed;
	if (pokemon.loaf)
		score += loaf;
	if (pokemon.fully_trapped)
		score += trapped;
	if (pokemon.nightmare())
		score += nightmare;
	if (pokemon.tormented())
		score += torment;
	return score;
}

int64_t Score::baton_passable_score(ActivePokemon const & pokemon) const {
	int64_t score = 0;
	if (pokemon.aqua_ring)
		score += aqua_ring;
	if (pokemon.focusing_energy)
		score += focus_energy;
	if (pokemon.ingrained())
		score += ingrain;
	score += pokemon.magnet_rise.turns_remaining * magnet_rise;
	if (pokemon.active_substitute)
		score += substitute + substitute_hp * pokemon.active_substitute.hp / pokemon.hp().max;
	score += Stage::dot_product(pokemon.stage, stage);
	return score;
}

int64_t Score::score_all_pokemon (Team & team, Team const & other, Weather const & weather) const {
	ResetIndex const reset_index (team);
	int64_t score = 0;
	for (uint8_t index = 0; index != team.all_pokemon().size(); ++index) {
		team.all_pokemon().set_index(index);
		score += score_pokemon (team, other, weather);
	}
	return score;
}

int64_t Score::score_pokemon (Team const & team, Team const & other, Weather const & weather) const {
	auto const & pokemon = team.pokemon().get_pokemon();
	int64_t score = team.entry_hazards.stealth_rock * stealth_rock * static_cast<int>(Type::stealth_rock_effectiveness(pokemon)) / 4;
	if (grounded (team.pokemon(), weather))
		score += team.entry_hazards.spikes * spikes + team.entry_hazards.toxic_spikes * toxic_spikes;
	if (pokemon.hp.stat != 0) {
		score += members;
		score += hp * pokemon.hp.stat / pokemon.hp.max;
		score += hidden * !pokemon.seen();
		score += score_status (team.pokemon());
		score += score_move (team.pokemon(), other, weather);
	}
	return score;
}

int64_t Score::score_status (ActivePokemon const & pokemon) const {
	switch (pokemon.status().name()) {
		case Status::BURN:
			return burn;
		case Status::FREEZE:
			return freeze;
		case Status::PARALYSIS:
			return paralysis;
		case Status::POISON:
			return poison;
		case Status::POISON_TOXIC:
			return poison * pokemon.toxic.counter / 2;
		case Status::REST:
		case Status::SLEEP:
			return sleep;
		default:
			return 0;
	}
}

int64_t Score::score_move (ActivePokemon const & pokemon, Team const & other, Weather const & weather) const {
	// TODO: alter the score of a move based on the weather
	int64_t score = 0;
	pokemon.get_pokemon().move.for_each([&](Move const & move) {
		if (move.is_physical())
			score += other.screens.m_reflect.turns_remaining * reflect;
		else if (move.is_special())
			score += other.screens.m_light_screen.turns_remaining * light_screen;
		if (move.pp.is_empty())
			score += no_pp;
	});
	return score;
}


int64_t Score::win (Team const & team) {
	if (team.all_pokemon().size() == 0)
		return team.is_me() ? -VICTORY : VICTORY;
	return 0;
}

int64_t Score::sleep_clause (Team const & team) {
	unsigned const sleeper_count = team.all_pokemon().count_if([](Pokemon const & pokemon) {
		return (pokemon.status().is_sleeping_due_to_other());
	});
	if (sleeper_count > 1)
		return team.is_me() ? VICTORY : -VICTORY;
	return 0;
}

Score::Score ():
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
	
	stage ({{}}),
	focus_energy (0),

	baton_pass (0),
	no_pp (0) {
	// This is a separate function instead of being stuck in directly to support
	// reloading of the constants.
	load_evaluation_constants ();
}

void Score::load_evaluation_constants () {
	boost::property_tree::ptree file;
	read_xml ("settings/evaluate.xml", file);
	boost::property_tree::ptree const pt = file.get_child("score");
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
	stage[Stat::ATK] = pt.get<int>("attack_stage", 0);
	stage[Stat::DEF] = pt.get<int>("defense_stage", 0);
	stage[Stat::SPA] = pt.get<int>("special_attack_stage", 0);
	stage[Stat::SPD] = pt.get<int>("special_defense_stage", 0);
	stage[Stat::SPE] = pt.get<int>("speed_stage", 0);
	focus_energy = pt.get<int>("focus_energy", 0);
	baton_pass = pt.get<int>("baton_pass", 0);
	no_pp = pt.get<int>("no_pp", 0);
}

}	// namespace technicalmachine
