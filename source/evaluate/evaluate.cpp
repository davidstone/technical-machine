// Evaluate the state of the game
// Copyright (C) 2014 David Stone
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

#include <algorithm>
#include <cstdint>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include "../team.hpp"

#include "../move/move.hpp"
#include "../move/moves.hpp"

#include "../pokemon/collection.hpp"
#include "../pokemon/pokemon.hpp"

#include "../stat/stage.hpp"

#include "../type/effectiveness.hpp"

namespace technicalmachine {

int64_t Evaluate::operator()(Team const & ai, Team const & foe, Weather const & weather) const {
	int64_t score = score_team (ai) - score_team (foe);
	return score + score_all_pokemon (ai, foe, weather) - score_all_pokemon (foe, ai, weather);
}

int64_t Evaluate::score_team (Team const & team) const {
	auto score = static_cast<int64_t>(lucky_chant() * team.screens.lucky_chant().turns_remaining());
	score += mist() * team.screens.mist().turns_remaining();
	score += safeguard() * team.screens.safeguard().turns_remaining();
	score += tailwind() * team.screens.tailwind().turns_remaining();
	score += wish() * bounded_integer::make_bounded(team.wish.is_active());
	return score;
}

namespace {

template<typename Array>
int dot_product(Stage const & stage, Array const & multiplier) {
	int result = 0;
	for (StatNames stat = static_cast<StatNames>(0); stat != StatNames::END; stat = static_cast<StatNames>(static_cast<int>(stat) + 1)) {
		result += stage[stat] * static_cast<int>(multiplier.at(stat));
	}
	return result;
}

}	// namespace

int64_t Evaluate::baton_passable_score(ActivePokemon const & pokemon) const {
	int64_t score = 0;
	if (pokemon.aqua_ring)
		score += aqua_ring();
	if (pokemon.focusing_energy)
		score += focus_energy();
	if (pokemon.ingrained())
		score += ingrain();
	score += pokemon.magnet_rise().turns_remaining() * magnet_rise();
	if (pokemon.m_substitute)
		score += substitute() + substitute_hp() * pokemon.m_substitute.hp() / get_hp(pokemon).max();
	score += dot_product(pokemon.stage(), stage());
	return score;
}

int64_t Evaluate::score_all_pokemon (Team const & team, Team const & other, Weather const & weather) const {
	int64_t score = 0;
	for (auto const index : bounded_integer::range(team.all_pokemon().size())) {
		bool const is_active = (index == team.pokemon().index());
		score += score_pokemon(team.pokemon(index), team.entry_hazards, other, weather);
		if (is_active) {
			score += score_active_pokemon(team.pokemon());
		}
	}
	return score;
}

int64_t Evaluate::score_active_pokemon(ActivePokemon const & pokemon) const {
	if (get_hp(pokemon) == 0_bi) {
		return 0;
	}
	int64_t score = 0;
	if (pokemon.is_cursed())
		score += curse();
	if (pokemon.imprisoned())
		score += imprison();
	if (pokemon.leech_seed)
		score += leech_seed();
	if (pokemon.loaf)
		score += loaf();
	if (pokemon.fully_trapped)
		score += trapped();
	if (pokemon.nightmare())
		score += nightmare();
	if (pokemon.tormented())
		score += torment();
	bool const has_baton_pass = static_cast<bool>(pokemon.all_moves().index(Moves::Baton_Pass));
	score += baton_passable_score(pokemon) * (has_baton_pass ? 2 : 1);
	return score;
}

int64_t Evaluate::score_pokemon (Pokemon const & pokemon, EntryHazards const & entry_hazards, Team const & other, Weather const & weather, int const toxic_counter) const {
	if (get_hp(pokemon) == 0_bi) {
		return 0;
	}
	auto score = entry_hazards.stealth_rock * static_cast<int64_t>(stealth_rock()) * Effectiveness::stealth_rock_effectiveness(pokemon);
	if (grounded(pokemon, weather)) {
		score += entry_hazards.spikes * spikes() + entry_hazards.toxic_spikes * toxic_spikes();
	}
	score += members();
	score += static_cast<int64_t>(hp()) * Rational(hp_ratio(pokemon));
	score += hidden() * !pokemon.seen();
	score += score_status(pokemon, toxic_counter);
	score += score_move (pokemon, other, weather);
	return score;
}

int64_t Evaluate::score_status(Pokemon const & pokemon, int const toxic_counter) const {
	switch (get_status(pokemon).name()) {
		case Status::BURN:
			return static_cast<int64_t>(burn());
		case Status::FREEZE:
			return static_cast<int64_t>(freeze());
		case Status::PARALYSIS:
			return static_cast<int64_t>(paralysis());
		case Status::POISON:
			return static_cast<int64_t>(poison());
		case Status::POISON_TOXIC:
			return static_cast<int64_t>(poison()) * toxic_counter / 2;
		case Status::REST:
		case Status::SLEEP:
			return static_cast<int64_t>(sleep());
		default:
			return 0;
	}
}

int64_t Evaluate::score_move (Pokemon const & pokemon, Team const & other, Weather const & weather) const {
	// TODO: alter the score of a move based on the weather
	int64_t score = 0;
	pokemon.move.for_each([&](Move const & move) {
		if (is_physical(move))
			score += other.screens.reflect().turns_remaining() * reflect();
		else if (is_special(move))
			score += other.screens.light_screen().turns_remaining() * light_screen();
		if (move.pp.is_empty())
			score += no_pp();
	});
	return score;
}


int64_t Evaluate::win (Team const & team) {
	if (team.all_pokemon().size() == 1_bi and get_hp(team.pokemon()) == 0_bi)
		return team.is_me() ? -victory : victory;
	return 0;
}

int64_t Evaluate::sleep_clause (Team const & team) {
	static constexpr auto sleepers = [](Pokemon const & pokemon) {
		return get_status(pokemon).is_sleeping_due_to_other();
	};
	auto const sleeper_count = std::count_if(team.all_pokemon().begin(), team.all_pokemon().end(), sleepers);
	if (sleeper_count > 1_bi)
		return team.is_me() ? victory : -victory;
	return 0;
}

Evaluate::Evaluate():
	m_light_screen(0_bi),
	m_lucky_chant(0_bi),
	m_mist(0_bi),
	m_reflect(0_bi),
	m_safeguard(0_bi),
	m_tailwind(0_bi),

	m_wish(0_bi),

	m_spikes(0_bi),
	m_stealth_rock(0_bi),
	m_toxic_spikes(0_bi),

	m_members(0_bi),
	m_hp(0_bi),
	m_hidden(0_bi),
	m_aqua_ring(0_bi),
	m_curse(0_bi),
	m_imprison(0_bi),
	m_ingrain(0_bi),
	m_leech_seed(0_bi),
	m_loaf(0_bi),
	m_magnet_rise(0_bi),
	m_nightmare(0_bi),
	m_substitute(0_bi),
	m_substitute_hp(0_bi),
	m_torment(0_bi),
	m_trapped(0_bi),

	m_burn(0_bi),
	m_freeze(0_bi),
	m_paralysis(0_bi),
	m_poison(0_bi),
	m_sleep(0_bi),
	
	m_stage({{}}),
	m_focus_energy(0_bi),

	m_baton_pass(0_bi),
	m_no_pp(0_bi) {
	// This is a separate function instead of being stuck in directly to support
	// reloading of the constants.
	load();
}

void Evaluate::load() {
	boost::property_tree::ptree file;
	read_xml("settings/evaluate.xml", file);
	boost::property_tree::ptree const pt = file.get_child("score");
	m_light_screen = pt.get<underlying_type>("light_screen", 0_bi);
	m_lucky_chant = pt.get<underlying_type>("lucky_chant", 0_bi);
	m_mist = pt.get<underlying_type>("mist", 0_bi);
	m_reflect = pt.get<underlying_type>("reflect", 0_bi);
	m_safeguard = pt.get<underlying_type>("safeguard", 0_bi);
	m_tailwind = pt.get<underlying_type>("tailwind", 0_bi);
	m_wish = pt.get<underlying_type>("wish", 0_bi);
	m_spikes = pt.get<underlying_type>("spikes", 0_bi);
	m_stealth_rock = pt.get<underlying_type>("stealth_rock", 0_bi);
	m_toxic_spikes = pt.get<underlying_type>("toxic_spikes", 0_bi);
	m_members = pt.get<underlying_type>("members", 0_bi);
	m_hp = pt.get<underlying_type>("hp", 0_bi);
	m_hidden = pt.get<underlying_type>("hidden", 0_bi);
	m_aqua_ring = pt.get<underlying_type>("aqua_ring", 0_bi);
	m_curse = pt.get<underlying_type>("curse", 0_bi);
	m_imprison = pt.get<underlying_type>("imprison", 0_bi);
	m_ingrain = pt.get<underlying_type>("ingrain", 0_bi);
	m_leech_seed = pt.get<underlying_type>("leech_seed", 0_bi);
	m_loaf = pt.get<underlying_type>("loaf", 0_bi);
	m_magnet_rise = pt.get<underlying_type>("magnet_rise", 0_bi);
	m_nightmare = pt.get<underlying_type>("nightmare", 0_bi);
	m_substitute = pt.get<underlying_type>("substitute", 0_bi);
	m_substitute_hp = pt.get<underlying_type>("substitute_hp", 0_bi);
	m_torment = pt.get<underlying_type>("torment", 0_bi);
	m_trapped = pt.get<underlying_type>("trapped", 0_bi);
	m_burn = pt.get<underlying_type>("burn", 0_bi);
	m_freeze = pt.get<underlying_type>("freeze", 0_bi);
	m_paralysis = pt.get<underlying_type>("paralysis", 0_bi);
	m_poison = pt.get<underlying_type>("poison", 0_bi);
	m_sleep = pt.get<underlying_type>("sleep", 0_bi);
	m_stage = {
		pt.get<underlying_type>("attack_stage", 0_bi),
		pt.get<underlying_type>("defense_stage", 0_bi),
		pt.get<underlying_type>("special_attack_stage", 0_bi),
		pt.get<underlying_type>("special_defense_stage", 0_bi),
		pt.get<underlying_type>("speed_stage", 0_bi)
	};
	m_focus_energy = pt.get<underlying_type>("focus_energy", 0_bi);
	m_baton_pass = pt.get<underlying_type>("baton_pass", 0_bi);
	m_no_pp = pt.get<underlying_type>("no_pp", 0_bi);
}

}	// namespace technicalmachine
