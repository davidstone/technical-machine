// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

module;

#include <bounded/assert.hpp>

export module tm.get_legal_selections;

import tm.move.is_switch;
import tm.move.legal_selections;
import tm.move.move;
import tm.move.move_name;

import tm.pokemon.active_pokemon;
import tm.pokemon.any_pokemon;
import tm.pokemon.pokemon_collection;
import tm.pokemon.potentially_selectable_moves;

import tm.ability_blocks_switching;
import tm.any_team;
import tm.blocks_selection_and_execution;
import tm.environment;
import tm.item;
import tm.generation;
import tm.switch_decision_required;
import tm.team;

import containers;

namespace technicalmachine {

template<typename PokemonType>
constexpr auto would_switch_to_same_pokemon(PokemonCollection<PokemonType> const & collection, MoveName const move) {
	return to_replacement(move) == collection.index();
}

template<any_active_pokemon ActivePokemonType>
constexpr auto is_blocked_from_switching(ActivePokemonType const user, ActivePokemonType const other, Environment const environment) {
	auto const block_attempted = ability_blocks_switching(other.ability(), user, environment) or user.trapped();
	return block_attempted and !allows_switching(user.item(environment));
}

template<Generation generation>
constexpr auto is_illegal_switch(Team<generation> const & user, MoveName const move, ActivePokemon<generation> const other, Environment const environment) {
	return is_switch(move) and (
		would_switch_to_same_pokemon(user.all_pokemon(), move) or
		is_blocked_from_switching(user.pokemon(), other, environment)
	);
}

constexpr auto blocked_by_torment(any_active_pokemon auto const user, MoveName const move) {
	return user.is_tormented() and user.last_used_move().name() == move and not is_switch(move) and move != MoveName::Struggle;
}

constexpr auto is_locked_in(any_active_pokemon auto const user, Environment const environment) {
	return user.is_encored() or user.last_used_move().is_locked_in_by_move() or is_choice_item(user.item(environment));
}

constexpr auto is_locked_in_to_different_move(any_active_pokemon auto const user, MoveName const move, Environment const environment) {
	if (not is_locked_in(user, environment)) {
		return false;
	}
	auto const last_move = user.last_used_move().name();
	return not is_switch(last_move) and last_move != move;
}

constexpr auto is_blocked_due_to_lock_in(any_active_pokemon auto const user, MoveName const move, Environment const environment) {
	return !is_regular(move) ?
		user.last_used_move().is_locked_in_by_move() :
		is_locked_in_to_different_move(user, move, environment);
}

template<any_team TeamType>
constexpr auto is_legal_selection(TeamType const & user, Move const move, TeamType const & other, Environment const environment, bool const found_selectable_move) {
	BOUNDED_ASSERT(move != MoveName::Hit_Self);
	if (switch_decision_required(user)) {
		return is_switch(move.name()) and !would_switch_to_same_pokemon(user.all_pokemon(), move.name());
	}
	auto const is_pass = move == MoveName::Pass;
	if (switch_decision_required(other)) {
		return is_pass;
	}
	auto const user_pokemon = user.pokemon();
	if (user_pokemon.last_used_move().moved_this_turn()) {
		return is_pass;
	}
	if (move == MoveName::Struggle) {
		return !found_selectable_move;
	}
	auto const other_pokemon = other.pokemon();
	return
		!is_pass and
		!is_blocked_due_to_lock_in(user_pokemon, move.name(), environment) and
		!is_illegal_switch(user, move.name(), other_pokemon, environment) and
		!blocks_selection_and_execution_attempt(user_pokemon, move, other_pokemon) and
		!blocks_selection_and_execution(user_pokemon, move.name(), environment) and
		!blocked_by_torment(user_pokemon, move.name());
}

export template<any_team TeamType>
constexpr auto get_legal_selections(TeamType const & user, TeamType const & other, Environment const environment) -> LegalSelections {
	auto result = LegalSelections();
	for (auto const move : potentially_selectable_moves(user)) {
		bool const found_selectable_move = !containers::is_empty(result);
		if (is_legal_selection(user, move, other, environment, found_selectable_move)) {
			containers::push_back_into_capacity(result, move.name());
		}
	}
	BOUNDED_ASSERT(!containers::is_empty(result));
	return result;
}

} // namespace technicalmachine
