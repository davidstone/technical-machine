// Block selection and execution of moves
// Copyright (C) 2016 David Stone
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

#include "block.hpp"

#include "ability.hpp"
#include "team.hpp"
#include "weather.hpp"

#include "move/move.hpp"
#include "move/moves.hpp"
#include "move/is_switch.hpp"

#include "pokemon/pokemon.hpp"
#include "pokemon/active_pokemon.hpp"

#include <containers/algorithms/all_any_none.hpp>

#include <cassert>

namespace technicalmachine {
namespace {

bool is_legal_selection(Team const & user, Move move, ActivePokemon other, Weather weather, bool found_selectable_move);
bool is_blocked_by_bide (ActivePokemon user, Moves move);
bool is_not_illegal_switch(Team const & user, Moves move, ActivePokemon other, Weather weather);
bool imprison(Moves move, ActivePokemon other);
bool blocked_by_torment(ActivePokemon user, Moves move);
bool block1 (ActivePokemon user, Move move, ActivePokemon other);
bool block2(ActivePokemon user, Moves move, Weather weather);
bool is_blocked_due_to_lock_in(ActivePokemon user, Moves move);
bool standard_move_lock_in(ActivePokemon user, Moves move);
bool is_locked_in (ActivePokemon user);
bool is_locked_in_to_different_move(ActivePokemon user, Moves move);
bool is_blocked_due_to_status(MutableActivePokemon user, Moves move);
bool is_blocked_by_freeze(Pokemon const & user, Moves move);
bool handle_sleep_counter(MutableActivePokemon user, Moves move);

}	// namespace

LegalSelections::LegalSelections(Team const & user, ActivePokemon const other, Weather const weather):
	m_species(user.pokemon())
{
	for (auto const & move : all_moves(user.pokemon())) {
		bool const found_selectable_move = !empty(*this);
		if (is_legal_selection(user, move, other, weather, found_selectable_move)) {
			emplace_back(move);
		}
	}
	assert(!empty(*this));
}

namespace {

bool is_legal_selection(Team const & user, Move const move, ActivePokemon const other, Weather const weather, bool const found_selectable_move) {
	return !is_blocked_by_bide(user.pokemon(), move) and
			is_not_illegal_switch(user, move, other, weather) and
			(move != Moves::Struggle or !found_selectable_move) and
			!((block1(user.pokemon(), move, other)) or (block2(user.pokemon(), move, weather)) or blocked_by_torment(user.pokemon(), move)) and
			!is_blocked_due_to_lock_in(user.pokemon(), move);
}
}	// namespace

bool can_execute_move (MutableActivePokemon user, ActivePokemon const other, Weather const weather) {
	auto const move = current_move(user);
	assert(!is_switch(move) or !is_recharging(user));
	
	if (is_switch(move)) {
		return true;
	}
	// TODO: fix
	if (get_hp(user) == 0_bi or (is_fainted(other) and false)) {
		return false;
	}

	bool execute = !(is_blocked_due_to_status (user, move) or
			block1 (user, move, other) or
			is_loafing(user));

	if (execute) {
		// Confusion doesn't block execution, it just changes the move that will
		// execute
		user.handle_confusion();
		if (flinched(user)) {
			if (get_ability(user).boosts_speed_when_flinched ())
				boost(stage(user), StatNames::SPE, 1_bi);
			execute = false;
		}
		else if (block2(user, move, weather) or is_fully_paralyzed(user)) {
			execute = false;
		}
	}

	if (user.recharge())
		execute = false;
	return execute;
}

namespace {

bool is_blocked_by_bide(ActivePokemon const user, Moves const move) {
	return is_locked_in_to_bide(user) and move == Moves::Bide;
}

auto would_switch_to_different_pokemon(PokemonCollection const & collection, Moves const move) {
	return to_replacement(move) != collection.index();
}

auto is_blocked_from_switching(ActivePokemon const user, Pokemon const & other, Weather const weather) {
	auto const block_attempted = get_ability(other).blocks_switching(user, weather) or trapped(user);
	auto const result = block_attempted and !allows_switching(get_item(user));
	return result;
}

bool is_not_illegal_switch(Team const & user, Moves const move, ActivePokemon const other, Weather const weather) {
	return is_switch(move) ?
		would_switch_to_different_pokemon(user.all_pokemon(), move) and !is_blocked_from_switching(user.pokemon(), other, weather) :
		true;
}

bool is_healing(Moves const name) {
	switch (name) {
		case Moves::Heal_Order:
		case Moves::Milk_Drink:
		case Moves::Moonlight:
		case Moves::Morning_Sun:
		case Moves::Recover:
		case Moves::Rest:
		case Moves::Roost:
		case Moves::Slack_Off:
		case Moves::Softboiled:
		case Moves::Swallow:
		case Moves::Synthesis:
		case Moves::Wish:
			return true;
		default:
			return false;
	}
}

// Things that both block selection and block execution in between sleep and confusion
bool block1 (ActivePokemon const user, Move const move, ActivePokemon const other) {
	if (!is_regular(move)) {
		return false;
	}
	return (move.pp().is_empty())
			or (is_disabled(user, move))
			or (heal_block_is_active(user) and (is_healing(move)))
			or (imprison(move, other));
}

bool imprison(Moves const move, ActivePokemon const other) {
	auto const & moves = regular_moves(other);
	return used_imprison(other) and containers::any_equal(moves.begin(), moves.end(), move);
}

bool is_blocked_by_taunt(Moves const move) {
	return !is_damaging(move);
}

bool is_blocked_by_gravity(Moves const move) {
	switch (move) {
		case Moves::Bounce:
		case Moves::Fly:
		case Moves::Hi_Jump_Kick:
		case Moves::Jump_Kick:
		case Moves::Magnet_Rise:
		case Moves::Splash:
			return true;
		default:
			return false;
	}
}

// Things that both block selection and block execution after flinching
bool block2(ActivePokemon const user, Moves const move, Weather const weather) {
	return !is_switch(move) and
			((is_taunted(user) and is_blocked_by_taunt(move)) or
			(weather.gravity() and is_blocked_by_gravity(move)));
}

bool is_blocked_due_to_lock_in(ActivePokemon const user, Moves const move) {
	return !is_regular(move) ?
		is_recharging(user) :
		standard_move_lock_in(user, move);
}

bool standard_move_lock_in(ActivePokemon const user, Moves const move) {
	return is_locked_in(user) ? is_locked_in_to_different_move(user, move) : false;
}

bool is_locked_in (ActivePokemon const user) {
	return is_encored(user) or is_recharging(user) or is_choice_item(get_item(user));
}

auto moved_since_switch(ActivePokemon const pokemon) -> bool {
	return last_used_move(pokemon).has_moved();
}

auto was_used_last(ActivePokemon const pokemon, Moves const move) -> bool {
	return last_used_move(pokemon).was_used_last(
		static_cast<LastUsedMove::index_type>(*index(all_moves(pokemon), move))
	);
}

bool is_locked_in_to_different_move(ActivePokemon const user, Moves const move) {
	return moved_since_switch(user) and !was_used_last(user, move);
}

bool blocked_by_torment(ActivePokemon const user, Moves const move) {
	return is_tormented(user) and was_used_last(user, move);
}

bool is_blocked_due_to_status(MutableActivePokemon user, Moves const move) {
	return is_blocked_by_freeze(user, move) or handle_sleep_counter(user, move);
}

bool is_blocked_by_freeze(Pokemon const & user, Moves const move) {
	return is_frozen(get_status(user)) and !is_usable_while_frozen(move);
}

bool is_blocked_by_sleep(Moves const move) {
	switch (move) {
		case Moves::Sleep_Talk:
		case Moves::Snore:
			return false;
		default:
			return true;
	}
}

bool handle_sleep_counter(MutableActivePokemon user, Moves const move) {
	if (!is_sleeping(get_status(user))) {
		return false;
	}
	user.increase_sleep_counter();
	return is_blocked_by_sleep(move);
}

}	// namespace
}	// namespace technicalmachine
