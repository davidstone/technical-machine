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
namespace {
#define CONDITIONAL BOUNDED_INTEGER_CONDITIONAL

auto baton_passable_score(Evaluate const & evaluate, ActivePokemon const & pokemon) {
	using stage_type = decltype(Stage::number_of_stats * (std::declval<Stage::value_type>() * std::declval<Stage::value_type>()));
	return
		(pokemon.aqua_ring_is_active() ? evaluate.aqua_ring() : 0_bi) +
		(pokemon.has_focused_energy() ? evaluate.focus_energy() : 0_bi) +
		(pokemon.ingrained() ? evaluate.ingrain() : 0_bi) +
		evaluate.magnet_rise() * pokemon.magnet_rise().turns_remaining() +
		(pokemon.substitute() ? (evaluate.substitute() + evaluate.substitute_hp() * pokemon.substitute().hp() / get_hp(pokemon).max()) : 0_bi) +
		std::inner_product(pokemon.stage().begin(), pokemon.stage().end(), evaluate.stage().begin(), static_cast<stage_type>(0_bi))
	;
}
using BatonPassableScore = decltype(baton_passable_score(std::declval<Evaluate>(), std::declval<ActivePokemon>()));

using ScoreStatus = Evaluate::value_type;
auto score_status(Evaluate const & evaluate, Pokemon const & pokemon) -> ScoreStatus {
	switch (get_status(pokemon).name()) {
		case Status::BURN:
			return evaluate.burn();
		case Status::FREEZE:
			return evaluate.freeze();
		case Status::PARALYSIS:
			return evaluate.paralysis();
		case Status::POISON:
			return evaluate.poison();
		case Status::POISON_TOXIC:
			return evaluate.toxic();
		case Status::REST:
		case Status::SLEEP:
			return evaluate.sleep();
		default:
			return 0_bi;
	}
}

auto score_move(Evaluate const & evaluate, Move const & move, Screens const & other) {
	return
		(
			CONDITIONAL(is_physical(move), evaluate.reflect() * other.reflect().turns_remaining(),
			CONDITIONAL(is_special(move), evaluate.light_screen() * other.light_screen().turns_remaining(),
			0_bi))
		) +
		CONDITIONAL(move.pp.is_empty(), evaluate.no_pp(), 0_bi)
	;
}
using ScoreMove = decltype(score_move(std::declval<Evaluate>(), std::declval<Move>(), std::declval<Screens>()));

using ScoreMoves = decltype(std::declval<ScoreMove>() * std::declval<RegularMoveSize>());
auto score_moves(Evaluate const & evaluate, Pokemon const & pokemon, Screens const & other, Weather const & weather) {
	// TODO: alter the score of a move based on the weather
	ScoreMoves score = 0_bi;
	pokemon.move.for_each([&](Move const & move) {
		score += score_move(evaluate, move, other);
	});
	return score;
}


auto score_active_pokemon(Evaluate const & evaluate, ActivePokemon const & pokemon) {
	bool const has_baton_pass = static_cast<bool>(pokemon.all_moves().index(Moves::Baton_Pass));
	return
		CONDITIONAL(pokemon.is_cursed(), evaluate.curse(), 0_bi) +
		CONDITIONAL(pokemon.imprisoned(), evaluate.imprison(), 0_bi) +
		CONDITIONAL(pokemon.leech_seeded(), evaluate.leech_seed(), 0_bi) +
		CONDITIONAL(pokemon.is_loafing(), evaluate.loaf(), 0_bi) +
		CONDITIONAL(pokemon.fully_trapped(), evaluate.trapped(), 0_bi) +
		CONDITIONAL(pokemon.nightmare(), evaluate.nightmare(), 0_bi) +
		CONDITIONAL(pokemon.tormented(), evaluate.torment(), 0_bi) +
		baton_passable_score(evaluate, pokemon) * CONDITIONAL(has_baton_pass, 2_bi, 1_bi)
	;
}


}	// namespace

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

int64_t Evaluate::score_all_pokemon (Team const & team, Team const & other, Weather const & weather) const {
	int64_t score = 0;
	for (auto const index : bounded_integer::range(team.all_pokemon().size())) {
		if (get_hp(team.pokemon(index)) == 0_bi) {
			continue;
		}
		bool const is_active = (index == team.pokemon().index());
		score += score_pokemon(team.pokemon(index), team.entry_hazards, other, weather);
		if (is_active) {
			score += score_active_pokemon(*this, team.pokemon());
		}
	}
	return score;
}

int64_t Evaluate::score_pokemon (Pokemon const & pokemon, EntryHazards const & entry_hazards, Team const & other, Weather const & weather, int const toxic_counter) const {
	auto score = static_cast<int64_t>(entry_hazards.stealth_rock() * stealth_rock() * Effectiveness(Type::Rock, pokemon));
	if (grounded(pokemon, weather)) {
		score += entry_hazards.spikes() * spikes() + entry_hazards.toxic_spikes() * toxic_spikes();
	}
	score += members();
	score += static_cast<int64_t>(hp()) * Rational(hp_ratio(pokemon));
	score += hidden() * !pokemon.seen();
	score += score_status(*this, pokemon);
	score += score_moves(*this, pokemon, other.screens, weather);
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

Evaluate::Evaluate() {
	boost::property_tree::ptree file;
	read_xml("settings/evaluate.xml", file);
	boost::property_tree::ptree const pt = file.get_child("score");

	using underlying_type = bounded_integer::equivalent_type<value_type, bounded_integer::throw_policy>;

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
	m_toxic = pt.get<underlying_type>("toxic", 0_bi);
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
