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

#include "../team.hpp"

#include "../move/move.hpp"
#include "../move/moves.hpp"

#include "../pokemon/collection.hpp"
#include "../pokemon/pokemon.hpp"

#include "../stat/stage.hpp"

#include "../type/effectiveness.hpp"

#include <bounded_integer/integer_range.hpp>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include <algorithm>
#include <cstdint>

namespace technicalmachine {
namespace {

template<typename T>
class MaxTurns;
template<intmax_t max_turns, CounterOperations... operations>
class MaxTurns<EndOfTurnCounter<max_turns, operations...>> {
public:
	static constexpr auto value = max_turns;
};

auto baton_passable_score(Evaluate const & evaluate, ActivePokemon const & pokemon) {
	using stage_type = decltype(Stage::number_of_stats * (std::declval<Stage::value_type>() * std::declval<Stage::value_type>()));
	return
		(pokemon.aqua_ring_is_active() ? evaluate.aqua_ring() : 0_bi) +
		(pokemon.has_focused_energy() ? evaluate.focus_energy() : 0_bi) +
		(pokemon.ingrained() ? evaluate.ingrain() : 0_bi) +
		(pokemon.magnet_rise().is_active() ? evaluate.magnet_rise() * (bounded::make<MaxTurns<MagnetRise>::value>() - *pokemon.magnet_rise().turns_active()) : 0_bi) +
		(pokemon.substitute() ? (evaluate.substitute() + evaluate.substitute_hp() * pokemon.substitute().hp() / get_hp(pokemon).max()) : 0_bi) +
		std::inner_product(pokemon.stage().begin(), pokemon.stage().end(), evaluate.stage().begin(), static_cast<stage_type>(0_bi))
	;
}
using BatonPassableScore = decltype(baton_passable_score(std::declval<Evaluate>(), std::declval<ActivePokemon>()));

using ScoreStatus = Evaluate::value_type;
auto score_status(Evaluate const & evaluate, Pokemon const & pokemon) -> ScoreStatus {
	switch (get_status(pokemon).name()) {
		case Status::burn:
			return evaluate.burn();
		case Status::freeze:
			return evaluate.freeze();
		case Status::paralysis:
			return evaluate.paralysis();
		case Status::poison:
			return evaluate.poison();
		case Status::poison_toxic:
			return evaluate.toxic();
		case Status::sleep_rest:
		case Status::sleep:
			return evaluate.sleep();
		default:
			return 0_bi;
	}
}

auto score_move(Evaluate const & evaluate, Move const & move, Screens const & other) {
	return
		(
			BOUNDED_CONDITIONAL(is_physical(move), evaluate.reflect() * other.reflect().turns_remaining(),
			BOUNDED_CONDITIONAL(is_special(move), evaluate.light_screen() * other.light_screen().turns_remaining(),
			0_bi))
		) +
		BOUNDED_CONDITIONAL(move.pp().is_empty(), evaluate.no_pp(), 0_bi)
	;
}
using ScoreMove = decltype(score_move(std::declval<Evaluate>(), std::declval<Move>(), std::declval<Screens>()));

using ScoreMoves = decltype(std::declval<ScoreMove>() * std::declval<RegularMoveSize>());
auto score_moves(Evaluate const & evaluate, Pokemon const & pokemon, Screens const & other, Weather const & weather) {
	// TODO: alter the score of a move based on the weather
	ScoreMoves score = 0_bi;
	return std::accumulate(pokemon.move.begin(), pokemon.move.end(), score, [&](auto init, auto const & move) {
		return init + score_move(evaluate, move, other);
	});
}


auto score_active_pokemon(Evaluate const & evaluate, ActivePokemon const & pokemon) {
	auto const & moves = pokemon.all_moves().regular();
	auto const has_baton_pass = std::find(moves.begin(), moves.end(), Moves::Baton_Pass) != moves.end();
	return
		BOUNDED_CONDITIONAL(pokemon.is_cursed(), evaluate.curse(), 0_bi) +
		BOUNDED_CONDITIONAL(pokemon.imprisoned(), evaluate.imprison(), 0_bi) +
		BOUNDED_CONDITIONAL(pokemon.leech_seeded(), evaluate.leech_seed(), 0_bi) +
		BOUNDED_CONDITIONAL(pokemon.is_loafing(), evaluate.loaf(), 0_bi) +
		BOUNDED_CONDITIONAL(pokemon.fully_trapped(), evaluate.trapped(), 0_bi) +
		BOUNDED_CONDITIONAL(pokemon.nightmare(), evaluate.nightmare(), 0_bi) +
		BOUNDED_CONDITIONAL(pokemon.tormented(), evaluate.torment(), 0_bi) +
		baton_passable_score(evaluate, pokemon) * BOUNDED_CONDITIONAL(has_baton_pass, 2_bi, 1_bi)
	;
}
using ScoreActivePokemon = decltype(score_active_pokemon(std::declval<Evaluate>(), std::declval<ActivePokemon>()));


auto score_pokemon(Evaluate const & evaluate, Pokemon const & pokemon, EntryHazards const & entry_hazards, Team const & other, Weather const & weather) {
	return
		evaluate.members() +
		hp_ratio(pokemon) * evaluate.hp() +
		BOUNDED_CONDITIONAL(!pokemon.has_been_seen(), evaluate.hidden(), 0_bi) +
		BOUNDED_CONDITIONAL(entry_hazards.stealth_rock(), Effectiveness(Type::Rock, pokemon) * evaluate.stealth_rock(), 0_bi) +
		BOUNDED_CONDITIONAL(grounded(pokemon, weather), entry_hazards.spikes() * evaluate.spikes() + entry_hazards.toxic_spikes() * evaluate.toxic_spikes(), 0_bi) +
		score_status(evaluate, pokemon) +
		score_moves(evaluate, pokemon, other.screens, weather)
	;
}
using ScorePokemon = decltype(score_pokemon(std::declval<Evaluate>(), std::declval<Pokemon>(), std::declval<EntryHazards>(), std::declval<Team>(), std::declval<Weather>()));

using ScoreAllPokemon = decltype(std::declval<ScorePokemon>() * std::declval<TeamSize>() + std::declval<ScoreActivePokemon>());
auto score_all_pokemon(Evaluate const & evaluate, Team const & team, Team const & other, Weather const & weather) {
	ScoreAllPokemon score = 0_bi;
	for (auto const index : bounded::integer_range(team.all_pokemon().size())) {
		if (get_hp(team.pokemon(index)) == 0_bi) {
			continue;
		}
		bool const is_active = (index == team.pokemon().index());
		score += score_pokemon(evaluate, team.pokemon(index), team.entry_hazards, other, weather);
		if (is_active) {
			score += score_active_pokemon(evaluate, team.pokemon());
		}
	}
	return score;
}

auto score_field_effects(Evaluate const & evaluate, Screens const & screens, Wish const & wish) {
	return
		screens.lucky_chant().turns_remaining() * evaluate.lucky_chant() +
		screens.mist().turns_remaining() * evaluate.mist() +
		screens.safeguard().turns_remaining() * evaluate.safeguard() +
		screens.tailwind().turns_remaining() * evaluate.tailwind() +
		bounded::make(wish.is_active()) * evaluate.wish()
	;
}

auto score_team(Evaluate const & evaluate, Team const & ai, Team const & foe, Weather const & weather) {
	auto const ai_field_effects = score_field_effects(evaluate, ai.screens, ai.wish);
	auto const foe_field_effects = score_field_effects(evaluate, foe.screens, foe.wish);
	auto const ai_pokemon = score_all_pokemon(evaluate, ai, foe, weather);
	auto const foe_pokemon = score_all_pokemon(evaluate, foe, ai, weather);
	return bounded::make<bounded::null_policy>(ai_field_effects - foe_field_effects + ai_pokemon - foe_pokemon);
}
// Extra is here to allow for one-past-the-end on both sides
constexpr bounded::integer<-1, 1> extra = 0_bi;
using ScoreTeam = decltype(score_team(std::declval<Evaluate>(), std::declval<Team>(), std::declval<Team>(), std::declval<Weather>()));

template<typename T>
struct TypeMismatchInEvaluateMessage;

template<typename T, bool lazy>
struct TypeMismatchInEvaluate {
	static constexpr auto value = true;
};
template<typename T>
struct TypeMismatchInEvaluate<T, false> {
	static constexpr auto value = TypeMismatchInEvaluateMessage<T>{};
};

using ResultType = decltype(std::declval<ScoreTeam>() + extra);

}	// namespace

auto Evaluate::operator()(Team const & ai, Team const & foe, Weather const & weather) const -> type {
	static_cast<void>(TypeMismatchInEvaluate<ResultType, std::is_same<Evaluate::type, ResultType>::value>::value);
	auto const score = score_team(*this, ai, foe, weather);
	return score;
}

auto Evaluate::win(Team const & team) -> type {
	if (team.all_pokemon().size() == 1_bi and get_hp(team.pokemon()) == 0_bi) {
		return BOUNDED_CONDITIONAL(team.is_me(), -victory, victory);
	}
	return 0_bi;
}

auto Evaluate::sleep_clause (Team const & team) -> type {
	static constexpr auto sleepers = [](Pokemon const & pokemon) {
		return is_sleeping_due_to_other(get_status(pokemon));
	};
	auto const sleeper_count = std::count_if(team.all_pokemon().begin(), team.all_pokemon().end(), sleepers);
	if (sleeper_count > 1_bi) {
		return BOUNDED_CONDITIONAL(team.is_me(), victory, -victory);
	}
	return 0_bi;
}

Evaluate::Evaluate() {
	boost::property_tree::ptree file;
	read_xml("settings/evaluate.xml", file);
	boost::property_tree::ptree const pt = file.get_child("score");

	using underlying_type = bounded::equivalent_type<value_type, bounded::throw_policy>;

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
