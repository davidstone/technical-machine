// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <tm/evaluate/possible_executed_moves.hpp>

#include <containers/algorithms/filter_iterator.hpp>
#include <containers/algorithms/transform.hpp>

namespace technicalmachine {
namespace {

constexpr auto can_be_selected_by_sleep_talk(KnownMove const move) {
	switch (move.name) {
		case MoveName::Assist:
		case MoveName::Bide:
		case MoveName::Chatter:
		case MoveName::Copycat:
		case MoveName::Focus_Punch:
		case MoveName::Me_First:
		case MoveName::Metronome:
		case MoveName::Mirror_Move:
		case MoveName::Sleep_Talk:
		case MoveName::Uproar:
		case MoveName::Blast_Burn:
		case MoveName::Frenzy_Plant:
		case MoveName::Giga_Impact:
		case MoveName::Hydro_Cannon:
		case MoveName::Hyper_Beam:
		case MoveName::Roar_of_Time:
		case MoveName::Rock_Wrecker:
		case MoveName::Solar_Beam:
			return false;
		default:
			return true;
	}
}

} // namespace

template<any_team UserTeam>
auto possible_executed_moves(MoveName const selected_move, UserTeam const & user_team) -> PossibleExecutedMoves {
	auto const user_pokemon = user_team.pokemon();
	auto type = [=](MoveName const move) {
		return get_type(generation_from<UserTeam>, move, get_hidden_power_type(user_pokemon));
	};
	auto known = [=](Move const move) {
		return KnownMove{move.name(), type(move.name())};
	};
	using containers::filter;
	using containers::transform;
	switch (selected_move) {
		case MoveName::Sleep_Talk:
			return is_sleeping(user_pokemon.status()) ?
				PossibleExecutedMoves(filter(transform(user_pokemon.regular_moves(), known), can_be_selected_by_sleep_talk)) :
				PossibleExecutedMoves({KnownMove{selected_move, type(selected_move)}});
		default:
			return PossibleExecutedMoves({KnownMove{selected_move, type(selected_move)}});
	}
}

#define TECHNICALMACHINE_EXPLICIT_INSTANTIATION(generation) \
	template auto possible_executed_moves(MoveName const selected_move, Team<generation> const & user_team) -> PossibleExecutedMoves

TECHNICALMACHINE_FOR_EACH_GENERATION(TECHNICALMACHINE_EXPLICIT_INSTANTIATION);

} // namespace technicalmachine
