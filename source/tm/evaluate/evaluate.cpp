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

auto score_pokemon(Evaluate const & evaluate, Generation const generation, Pokemon const & pokemon, EntryHazards const & entry_hazards) {
	auto const types = PokemonTypes(generation, pokemon.species());
	auto const grounded =
		containers::any_equal(types, Type::Flying) or
		is_immune_to_ground(pokemon.initial_ability());
	return
		Evaluate::value_type(hp_ratio(pokemon) * evaluate.hp()) +
		(!pokemon.has_been_seen() ? evaluate.hidden() : 0_bi) +
		(entry_hazards.stealth_rock() ? Effectiveness(generation, Type::Rock, types) * evaluate.stealth_rock() : 0_bi) +
		(grounded ? entry_hazards.spikes() * evaluate.spikes() + entry_hazards.toxic_spikes() * evaluate.toxic_spikes() : 0_bi);
}

auto score_team(Evaluate const & evaluate, Generation const generation, Team const & team) {
	auto has_hp = [](auto const & pokemon) { return pokemon.hp() != 0_bi; };
	auto get_score = [&](auto const & pokemon) {
		return score_pokemon(evaluate, generation, pokemon, team.entry_hazards());
	};
	return containers::sum(containers::transform(containers::filter(team.all_pokemon(), has_hp), get_score));
}

auto score_teams(Evaluate const & evaluate, Generation const generation, Team const & ai, Team const & foe) {
	return score_team(evaluate, generation, ai) - score_team(evaluate, generation, foe);
}

using ScoreTeam = decltype(score_teams(std::declval<Evaluate>(), std::declval<Generation>(), std::declval<Team>(), std::declval<Team>()));

// Extra is here to allow for one-past-the-end on both sides
using ResultType = decltype(std::declval<ScoreTeam>() + std::declval<bounded::integer<-1, 1>>());

template<typename LHS, typename RHS>
struct TypeMismatchInEvaluateMessage;

template<typename LHS, typename RHS>
struct TypeMismatchInEvaluate {
	static constexpr auto value = TypeMismatchInEvaluateMessage<LHS, RHS>{};
};
template<typename T>
struct TypeMismatchInEvaluate<T, T> {
	static constexpr auto value = true;
};

}	// namespace

auto Evaluate::operator()(Generation const generation, Team const & ai, Team const & foe, [[maybe_unused]] Weather const weather) const -> type {
	static_cast<void>(TypeMismatchInEvaluate<ResultType, Evaluate::type>::value);
	return score_teams(*this, generation, ai, foe);
}

namespace {

auto single_team_win(Team const & team) -> Evaluate::type {
	BOUNDED_ASSERT(team.size() != 0_bi);
	if (team.size() == 1_bi and team.pokemon().hp() == 0_bi) {
		return BOUNDED_CONDITIONAL(team.is_me(), -victory, victory);
	}
	return 0_bi;
}

} // namespace

auto Evaluate::win(Team const & team1, Team const & team2) -> bounded::optional<double> {
	auto const win1 = single_team_win(team1);
	auto const win2 = single_team_win(team2);
	if (win1 != 0_bi or win2 != 0_bi) {
		return static_cast<double>(win1 + win2);
	}
	return bounded::none;
}

// TODO: Load generation from configuration
Evaluate::Evaluate() {
	boost::property_tree::ptree file;
	read_xml("settings/evaluate.xml", file);
	boost::property_tree::ptree const pt = file.get_child("score");

	// TODO: Use change_policy
	using underlying_type = bounded::checked_integer<
		static_cast<int>(bounded::min_value<value_type>),
		static_cast<int>(bounded::max_value<value_type>)
	>;

	m_spikes = pt.get<underlying_type>("spikes", 0_bi);
	m_stealth_rock = pt.get<underlying_type>("stealth_rock", 0_bi);
	m_toxic_spikes = pt.get<underlying_type>("toxic_spikes", 0_bi);
	m_hp = pt.get<underlying_type>("hp", 0_bi);
	m_hidden = pt.get<underlying_type>("hidden", 0_bi);
}

}	// namespace technicalmachine
