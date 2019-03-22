// Evaluate the state of the game
// Copyright (C) 2018 David Stone
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

#include <tm/evaluate/evaluate.hpp>

#include <tm/team.hpp>
#include <tm/weather.hpp>

#include <tm/move/category.hpp>
#include <tm/move/move.hpp>
#include <tm/move/moves.hpp>

#include <tm/pokemon/collection.hpp>
#include <tm/pokemon/pokemon.hpp>

#include <tm/stat/stage.hpp>

#include <tm/type/effectiveness.hpp>

#include <containers/algorithms/accumulate.hpp>
#include <containers/algorithms/all_any_none.hpp>
#include <containers/algorithms/count.hpp>
#include <containers/algorithms/filter_iterator.hpp>
#include <containers/legacy_iterator.hpp>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include <numeric>
#include <type_traits>

namespace technicalmachine {
namespace {

template<typename T>
struct MaxTurns;
template<int max_turns, CounterOperations... operations>
struct MaxTurns<EndOfTurnCounter<max_turns, operations...>> {
	static constexpr auto value = max_turns;
};

auto baton_passable_score(Evaluate const & evaluate, ActivePokemon const pokemon) {
	using stage_type = decltype(Stage::number_of_stats * (std::declval<Stage::value_type>() * std::declval<Evaluate::value_type>()));
	auto const substitute = pokemon.substitute();
	auto const magnet_rise = pokemon.magnet_rise();
	auto const & stage = pokemon.stage();
	return
		(pokemon.aqua_ring_is_active() ? evaluate.aqua_ring() : 0_bi) +
		(pokemon.has_focused_energy() ? evaluate.focus_energy() : 0_bi) +
		(pokemon.ingrained() ? evaluate.ingrain() : 0_bi) +
		(magnet_rise.is_active() ? evaluate.magnet_rise() * (bounded::constant<MaxTurns<MagnetRise>::value> - *magnet_rise.turns_active()) : 0_bi) +
		(substitute ? (evaluate.substitute() + evaluate.substitute_hp() * substitute.hp() / get_hp(pokemon).max()) : 0_bi) +
		std::inner_product(containers::legacy_iterator(begin(stage)), containers::legacy_iterator(end(stage)), containers::legacy_iterator(begin(evaluate.stage())), static_cast<stage_type>(0_bi))
	;
}

auto score_status(Evaluate const & evaluate, Pokemon const & pokemon) -> Evaluate::value_type {
	switch (get_status(pokemon).name()) {
		case Statuses::burn:
			return evaluate.burn();
		case Statuses::freeze:
			return evaluate.freeze();
		case Statuses::paralysis:
			return evaluate.paralysis();
		case Statuses::poison:
			return evaluate.poison();
		case Statuses::poison_toxic:
			return evaluate.toxic();
		case Statuses::sleep_rest:
		case Statuses::sleep:
			return evaluate.sleep();
		default:
			return 0_bi;
	}
}

auto score_move(Evaluate const & evaluate, Move const move, Screens const & other) {
	auto const reflect_turns = other.reflect().turns_remaining();
	auto const score_reflect = reflect_turns != 0_bi and is_physical(move.name());
	auto const light_screen_turns = other.light_screen().turns_remaining();
	auto const score_light_screen = !score_reflect and light_screen_turns != 0_bi and is_special(move.name());
	return
		(
			BOUNDED_CONDITIONAL(score_reflect, evaluate.reflect() * reflect_turns,
			BOUNDED_CONDITIONAL(score_light_screen, evaluate.light_screen() * light_screen_turns,
			0_bi))
		) +
		BOUNDED_CONDITIONAL(move.pp().is_empty(), evaluate.no_pp(), 0_bi)
	;
}

auto score_moves(Evaluate const & evaluate, Pokemon const & pokemon, Screens const & other, Weather const) {
	// TODO: alter the score of a move based on the weather
	auto get_score = [&](auto const move) { return score_move(evaluate, move, other); };
	return containers::accumulate(containers::transform(regular_moves(pokemon), get_score));
}


auto score_active_pokemon(Evaluate const & evaluate, ActivePokemon const pokemon) {
	auto const has_baton_pass = containers::any_equal(regular_moves(pokemon), Moves::Baton_Pass);
	return
		BOUNDED_CONDITIONAL(pokemon.is_cursed(), evaluate.curse(), 0_bi) +
		BOUNDED_CONDITIONAL(pokemon.used_imprison(), evaluate.imprison(), 0_bi) +
		BOUNDED_CONDITIONAL(pokemon.leech_seeded(), evaluate.leech_seed(), 0_bi) +
		BOUNDED_CONDITIONAL(pokemon.is_loafing(), evaluate.loaf(), 0_bi) +
		BOUNDED_CONDITIONAL(pokemon.fully_trapped(), evaluate.trapped(), 0_bi) +
		BOUNDED_CONDITIONAL(pokemon.is_having_a_nightmare(), evaluate.nightmare(), 0_bi) +
		BOUNDED_CONDITIONAL(pokemon.is_tormented(), evaluate.torment(), 0_bi) +
		baton_passable_score(evaluate, pokemon) * BOUNDED_CONDITIONAL(has_baton_pass, 2_bi, 1_bi)
	;
}


auto score_pokemon(Evaluate const & evaluate, Pokemon const & pokemon, EntryHazards const & entry_hazards, Team const & other, Weather const weather) {
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

auto score_all_pokemon(Evaluate const & evaluate, Team const & team, Team const & other, Weather const weather) {
	auto has_hp = [](auto const & pokemon) { return get_hp(pokemon) != 0_bi; };
	auto get_score = [&](auto const & pokemon) {
		return score_pokemon(evaluate, pokemon, team.entry_hazards, other, weather);
	};
	return
		containers::accumulate(containers::transform(containers::filter(team.all_pokemon(), has_hp), get_score)) +
		score_active_pokemon(evaluate, team.pokemon());
}

auto score_field_effects(Evaluate const & evaluate, Screens const & screens, Wish const & wish) {
	return
		screens.lucky_chant().turns_remaining() * evaluate.lucky_chant() +
		screens.mist().turns_remaining() * evaluate.mist() +
		screens.safeguard().turns_remaining() * evaluate.safeguard() +
		screens.tailwind().turns_remaining() * evaluate.tailwind() +
		BOUNDED_CONDITIONAL(wish.is_active(), 1_bi, 0_bi) * evaluate.wish()
	;
}

auto score_team(Evaluate const & evaluate, Team const & ai, Team const & foe, Weather const weather) {
	auto const ai_field_effects = score_field_effects(evaluate, ai.screens, ai.wish);
	auto const foe_field_effects = score_field_effects(evaluate, foe.screens, foe.wish);
	auto const ai_pokemon = score_all_pokemon(evaluate, ai, foe, weather);
	auto const foe_pokemon = score_all_pokemon(evaluate, foe, ai, weather);
	return ai_field_effects - foe_field_effects + ai_pokemon - foe_pokemon;
}
// Extra is here to allow for one-past-the-end on both sides
constexpr bounded::integer<-1, 1> extra = 0_bi;
using ScoreTeam = decltype(score_team(std::declval<Evaluate>(), std::declval<Team>(), std::declval<Team>(), std::declval<Weather>()));

template<typename LHS, typename RHS>
struct TypeMismatchInEvaluateMessage;

template<typename LHS, typename RHS, bool = std::is_same_v<LHS, RHS>>
struct TypeMismatchInEvaluate {
	static constexpr auto value = true;
};
template<typename LHS, typename RHS>
struct TypeMismatchInEvaluate<LHS, RHS, false> {
	static constexpr auto value = TypeMismatchInEvaluateMessage<LHS, RHS>{};
};

using ResultType = decltype(std::declval<ScoreTeam>() + extra);

}	// namespace

auto Evaluate::operator()(Team const & ai, Team const & foe, Weather const weather) const -> type {
	static_cast<void>(TypeMismatchInEvaluate<ResultType, Evaluate::type>::value);
	return score_team(*this, ai, foe, weather);
}

namespace {

auto sleep_clause(Team const & team) -> Evaluate::type {
	auto const sleepers = [](Pokemon const & pokemon) {
		return is_sleeping_due_to_other(get_status(pokemon));
	};
	auto const sleeper_count = containers::count_if(team.all_pokemon(), sleepers);
	if (sleeper_count > 1_bi) {
		return BOUNDED_CONDITIONAL(team.is_me(), victory, -victory);
	}
	return 0_bi;
}

auto single_team_win(Team const & team) -> Evaluate::type {
	assert(team.size() != 0_bi);
	if (team.size() == 1_bi and get_hp(team.pokemon()) == 0_bi) {
		return BOUNDED_CONDITIONAL(team.is_me(), -victory, victory);
	}
	return 0_bi;
}

} // namespace

auto Evaluate::win(Team const & team1, Team const & team2) -> bounded::optional<double> {
	auto const sleep_clause1 = sleep_clause(team1);
	auto const sleep_clause2 = sleep_clause(team2);
	if (sleep_clause1 != 0_bi or sleep_clause2 != 0_bi) {
		return static_cast<double>(sleep_clause1 + sleep_clause2);
	}
	auto const win1 = single_team_win(team1);
	auto const win2 = single_team_win(team2);
	if (win1 != 0_bi or win2 != 0_bi) {
		return static_cast<double>(win1 + win2);
	}
	return bounded::none;
}

Evaluate::Evaluate() {
	boost::property_tree::ptree file;
	read_xml("settings/evaluate.xml", file);
	boost::property_tree::ptree const pt = file.get_child("score");

	// TODO: Use change_policy
	using underlying_type = bounded::checked_integer<
		static_cast<int>(value_type::min()),
		static_cast<int>(value_type::max())
	>;

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
