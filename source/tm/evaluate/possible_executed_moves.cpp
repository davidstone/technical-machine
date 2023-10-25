// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.evaluate.possible_executed_moves;

import tm.move.known_move;
import tm.move.move;
import tm.move.move_name;

import tm.pokemon.get_hidden_power_type;

import tm.status.status;

import tm.type.move_type;

import tm.any_team;
import tm.generation;

import bounded;
import containers;

namespace technicalmachine {
using namespace bounded::literal;

constexpr auto can_be_selected_by_sleep_talk(Generation const generation) {
	return [=](KnownMove const move) {
		switch (move.name) {
			case MoveName::Assist:
			case MoveName::Beak_Blast:
			case MoveName::Belch:
			case MoveName::Bide:
			case MoveName::Blazing_Torque:
			case MoveName::Bounce:
			case MoveName::Celebrate:
			case MoveName::Chatter:
			case MoveName::Combat_Torque:
			case MoveName::Copycat:
			case MoveName::Dig:
			case MoveName::Dive:
			case MoveName::Dynamax_Cannon:
			case MoveName::Fly:
			case MoveName::Freeze_Shock:
			case MoveName::Focus_Punch:
			case MoveName::Geomancy:
			case MoveName::Hold_Hands:
			case MoveName::Ice_Burn:
			case MoveName::Magical_Torque:
			case MoveName::Me_First:
			case MoveName::Mimic:
			case MoveName::Nature_Power:
			case MoveName::Noxious_Torque:
			case MoveName::Phantom_Force:
			case MoveName::Razor_Wind:
			case MoveName::Shadow_Force:
			case MoveName::Shell_Trap:
			case MoveName::Skull_Bash:
			case MoveName::Sky_Attack:
			case MoveName::Sky_Drop:
			case MoveName::Sleep_Talk:
			case MoveName::Solar_Beam:
			case MoveName::Solar_Blade:
			case MoveName::Struggle:
			case MoveName::Uproar:
			case MoveName::Wicked_Torque:
				return false;
			case MoveName::Metronome:
			case MoveName::Mirror_Move:
				return generation <= Generation::two;
			case MoveName::Sketch:
				return generation <= Generation::four;
			default:
				return true;
		}
	};
}

export using PossibleExecutedMoves = containers::static_vector<KnownMove, 3_bi>;

export template<any_team UserTeam>
auto possible_executed_moves(MoveName const selected_move, UserTeam const & user_team) -> PossibleExecutedMoves {
	auto const user_pokemon = user_team.pokemon();
	auto type = [=](MoveName const move) {
		return move_type(generation_from<UserTeam>, move, get_hidden_power_type(user_pokemon));
	};
	auto known = [=](Move const move) {
		return KnownMove{move.name(), type(move.name())};
	};
	using containers::filter;
	using containers::transform;
	switch (selected_move) {
		case MoveName::Sleep_Talk:
			return is_sleeping(user_pokemon.status()) ?
				PossibleExecutedMoves(
					filter(
						transform(user_pokemon.regular_moves(), known),
						can_be_selected_by_sleep_talk(generation_from<UserTeam>)
					)
				) :
				PossibleExecutedMoves({KnownMove{selected_move, type(selected_move)}});
		default:
			return PossibleExecutedMoves({KnownMove{selected_move, type(selected_move)}});
	}
}

} // namespace technicalmachine
