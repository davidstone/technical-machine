// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

module;

#include <operators/forward.hpp>

export module tm.get_legal_selections;

import tm.move.is_switch;
import tm.move.legal_selections;
import tm.move.move;
import tm.move.move_name;

import tm.pokemon.active_pokemon;
import tm.pokemon.any_pokemon;
import tm.pokemon.max_pokemon_per_team;
import tm.pokemon.pokemon_collection;

import tm.ability_blocks_switching;
import tm.any_team;
import tm.blocks_selection_and_execution;
import tm.environment;
import tm.item;
import tm.generation;
import tm.switch_decision_required;

import bounded;
import containers;
import std_module;

namespace technicalmachine {
using namespace bounded::literal;

template<typename Function>
struct not_fn {
	explicit constexpr not_fn(Function function):
		m_function(std::move(function))
	{
	}
	constexpr auto operator()(auto && ... args) const {
		return !m_function(OPERATORS_FORWARD(args)...);
	}
private:
	[[no_unique_address]] Function m_function;
};

// If switching at all is legal, these are the legal switches
using PotentialSwitches = containers::static_vector<MoveName, max_pokemon_per_team - 1_bi>;
constexpr auto potential_switches(TeamSize const team_size, TeamIndex const active) {
	return team_size > 1_bi ?
		PotentialSwitches(containers::transform(
			containers::filter(
				containers::integer_range(team_size),
				not_fn(bounded::equal_to(active))
			),
			to_switch
		)) :
		PotentialSwitches();
}

constexpr auto must_repeat(any_active_pokemon auto const user, Environment const environment) {
	return user.is_encored() or is_choice_item(user.item(environment));
}

constexpr auto must_repeat_different_move(any_active_pokemon auto const user, MoveName const move, Environment const environment) {
	auto const last_move = user.last_used_move().name();
	return
		!is_switch(last_move) and
		last_move != move and
		must_repeat(user, environment);
}

constexpr auto is_blocked_by_torment(any_active_pokemon auto const user, MoveName const move) {
	return user.is_tormented() and user.last_used_move().name() == move;
}

constexpr auto is_legal_regular_move(
	any_active_pokemon auto const user_pokemon,
	any_active_pokemon auto const other_pokemon,
	Environment const environment
) {
	return [=](Move const move) -> bool {
		return
			!must_repeat_different_move(user_pokemon, move.name(), environment) and
			!blocks_selection_and_execution_attempt(user_pokemon, move, other_pokemon) and
			!blocks_selection_and_execution(user_pokemon, move.name(), environment) and
			!is_blocked_by_torment(user_pokemon, move.name());
	};
}

constexpr auto potential_regular(
	any_active_pokemon auto const user_pokemon,
	any_active_pokemon auto const other_pokemon,
	Environment const environment
) {
	return containers::make_static_vector(containers::transform(
		containers::filter(
			user_pokemon.regular_moves(),
			is_legal_regular_move(user_pokemon, other_pokemon, environment)
		),
		&Move::name
	));
}

template<any_active_pokemon ActivePokemonType>
constexpr auto can_switch(ActivePokemonType const user, ActivePokemonType const other, Environment const environment) {
	auto const block_attempted =
		ability_blocks_switching(other.ability(), user, environment) or
		user.trapped();
	return !block_attempted or allows_switching(user.item(environment));
}

export template<any_real_team TeamType>
constexpr auto get_legal_selections(
	TeamType const & user,
	TeamType const & other,
	Environment const environment
) -> LegalSelections {
	auto replacement_switches = [&] {
		return potential_switches(
			user.size(),
			user.all_pokemon().index()
		);
	};
	if (switch_decision_required(user)) {
		return LegalSelections(replacement_switches());
	}
	if (switch_decision_required(other)) {
		return LegalSelections({MoveName::Pass});
	}
	auto const user_pokemon = user.pokemon();
	auto const last_used_move = user_pokemon.last_used_move();
	if (last_used_move.moved_this_turn()) {
		return LegalSelections({MoveName::Pass});
	}
	if (last_used_move.is_locked_in_by_move()) {
		return LegalSelections({last_used_move.name()});
	}
	auto const other_pokemon = other.pokemon();
	auto const regular = potential_regular(
		user_pokemon,
		other_pokemon,
		environment
	);
	auto const switches = can_switch(user_pokemon, other_pokemon, environment) ?
		replacement_switches() :
		PotentialSwitches();
	if (containers::is_empty(regular)) {
		return containers::concatenate<LegalSelections>(
			containers::array({MoveName::Struggle}),
			switches
		);
	}
	return containers::concatenate<LegalSelections>(regular, switches);
}

} // namespace technicalmachine
