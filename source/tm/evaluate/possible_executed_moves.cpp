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
		case Moves::Assist:
		case Moves::Bide:
		case Moves::Chatter:
		case Moves::Copycat:
		case Moves::Focus_Punch:
		case Moves::Me_First:
		case Moves::Metronome:
		case Moves::Mirror_Move:
		case Moves::Sleep_Talk:
		case Moves::Uproar:
		case Moves::Blast_Burn:
		case Moves::Frenzy_Plant:
		case Moves::Giga_Impact:
		case Moves::Hydro_Cannon:
		case Moves::Hyper_Beam:
		case Moves::Roar_of_Time:
		case Moves::Rock_Wrecker:
		case Moves::Solar_Beam:
			return false;
		default:
			return true;
	}
}

}	// namespace

template<Generation generation>
auto possible_executed_moves(Moves const selected_move, Team<generation> const & user_team) -> PossibleExecutedMoves {
	auto const user_pokemon = user_team.pokemon();
	auto type = [=](Moves const move) {
		return get_type(generation, move, get_hidden_power_type(user_pokemon));
	};
	auto known = [=](Move const move) {
		return KnownMove{move.name(), type(move.name())};
	};
	using containers::filter;
	using containers::transform;
	switch (selected_move) {
		case Moves::Sleep_Talk:
			return is_sleeping(user_pokemon.status()) ?
				PossibleExecutedMoves(filter(transform(user_pokemon.regular_moves(), known), can_be_selected_by_sleep_talk)) :
				PossibleExecutedMoves({KnownMove{selected_move, type(selected_move)}});
		default:
			return PossibleExecutedMoves({KnownMove{selected_move, type(selected_move)}});
	}
}

#define TECHNICALMACHINE_EXPLICIT_INSTANTIATION(generation) \
	template auto possible_executed_moves<generation>(Moves const selected_move, Team<generation> const & user_team) -> PossibleExecutedMoves

TECHNICALMACHINE_EXPLICIT_INSTANTIATION(Generation::one);
TECHNICALMACHINE_EXPLICIT_INSTANTIATION(Generation::two);
TECHNICALMACHINE_EXPLICIT_INSTANTIATION(Generation::three);
TECHNICALMACHINE_EXPLICIT_INSTANTIATION(Generation::four);
TECHNICALMACHINE_EXPLICIT_INSTANTIATION(Generation::five);
TECHNICALMACHINE_EXPLICIT_INSTANTIATION(Generation::six);
TECHNICALMACHINE_EXPLICIT_INSTANTIATION(Generation::seven);
TECHNICALMACHINE_EXPLICIT_INSTANTIATION(Generation::eight);

}	// namespace technicalmachine
