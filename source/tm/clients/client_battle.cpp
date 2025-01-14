// Copyright David Stone 2022.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.clients.client_battle;

import tm.clients.turn_count;

import tm.move.move_name;
import tm.move.move_result;
import tm.move.switch_;

import tm.pokemon.level;
import tm.pokemon.max_pokemon_per_team;
import tm.pokemon.nickname;
import tm.pokemon.species;

import tm.status.status_name;

import tm.ability;
import tm.end_of_turn_flags;
import tm.gender;
import tm.generation;
import tm.generation_generic;
import tm.item;
import tm.visible_hp;
import tm.visible_state;
import tm.weather;

import std_module;

namespace technicalmachine {

// ClientBattle is intended to be used in implementing clients. The goal is for
// there to be no undefined behavior when calling any of these functions to
// ensure we have a secure interface with the outside world. All of these
// functions will throw an exception if given data that is inconsistent overall.
export struct ClientBattle {
	virtual ~ClientBattle() = default;

	virtual auto generation() const -> Generation = 0;
	virtual auto state() const -> GenerationGeneric<VisibleState> = 0;

	virtual auto ai_has(Species, Nickname nickname, Level, Gender) const -> TeamIndex = 0;
	virtual auto foe_has(
		Species,
		Nickname nickname,
		Level,
		Gender,
		MaxVisibleHP
	) & -> TeamIndex = 0;

	virtual auto active_has(bool is_ai, MoveName) & -> void = 0;
	virtual auto active_has(bool is_ai, Ability) & -> void = 0;
	virtual auto replacement_has(bool is_ai, TeamIndex, Ability) & -> void = 0;
	virtual auto active_has(bool is_ai, Item) & -> void = 0;
	virtual auto replacement_has(bool is_ai, TeamIndex, Item) & -> void = 0;

	virtual auto is_end_of_turn() const -> bool = 0;
	virtual auto ai_is_fainted() const -> bool = 0;
	virtual auto foe_is_fainted() const -> bool = 0;

	virtual auto end_turn(bool ai_went_first, EndOfTurnFlags first_flags, EndOfTurnFlags last_flags) & -> void = 0;
	virtual auto use_move(bool ai_is_user, MoveResult, bool user_status_was_cleared) & -> void = 0;
	virtual auto use_switch(bool ai_is_user, Switch) & -> void = 0;
	virtual auto hit_self_in_confusion(bool ai_is_user, VisibleHP const damage) & -> void = 0;
	virtual auto end_disable(bool ai_is_user) & -> void = 0;

	// TODO: Delete this function
	virtual auto cures_target_status(bool is_ai, MoveName) const -> bool = 0;

	// For a correct implementation of a correct protocol, these functions have
	// no effect.

	virtual auto correct_hp(bool is_ai, VisibleHP) & -> void = 0;

	virtual auto correct_status(bool is_ai, StatusName, TeamIndex) & -> void = 0;
	virtual auto correct_status(bool is_ai, StatusName) & -> void = 0;

	virtual auto weather_is(Weather) const -> void = 0;
};

} // namespace technicalmachine
