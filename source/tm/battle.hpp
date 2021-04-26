// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <tm/clients/battle_result.hpp>
#include <tm/clients/party.hpp>

#include <tm/move/actual_damage.hpp>
#include <tm/move/call_move.hpp>
#include <tm/move/max_moves_per_pokemon.hpp>
#include <tm/move/other_move.hpp>
#include <tm/move/used_move.hpp>

#include <tm/pokemon/species_forward.hpp>

#include <tm/string_conversions/status.hpp>

#include <tm/end_of_turn.hpp>
#include <tm/team.hpp>
#include <tm/visible_hp.hpp>
#include <tm/weather.hpp>

#include <containers/algorithms/concatenate.hpp>

#include <cstdint>
#include <iostream>
#include <stdexcept>
#include <string>
#include <utility>

namespace technicalmachine {

enum class Moves : std::uint16_t;

template<Generation generation>
struct Battle {
	Battle(Team<generation> ai_, Team<generation> foe_):
		m_ai(std::move(ai_)),
		m_foe(std::move(foe_))
	{
		// TODO: Properly order this
		m_ai.pokemon().switch_in(m_weather);
		m_foe.pokemon().switch_in(m_weather);
	}
	
	auto const & ai() const {
		return m_ai;
	}
	auto const & foe() const {
		return m_foe;
	}
	auto weather() const {
		return m_weather;
	}

	void handle_begin_turn() & {
		m_ai.reset_start_of_turn();
		m_foe.reset_start_of_turn();
	}
	// TODO: require users to specify which team went first and whether Shed
	// Skin activated
	void handle_end_turn() & {
		constexpr auto ai_flags = EndOfTurnFlags{false, false};
		constexpr auto foe_flags = EndOfTurnFlags{false, false};
		end_of_turn(m_ai, ai_flags, m_foe, foe_flags, m_weather);
	}

	void handle_use_move(bool const is_ai, UsedMove<generation> const move, bool const clear_status, ActualDamage const damage, OtherMove const other_move) {
		auto & user = is_ai ? m_ai : m_foe;
		auto & other = is_ai ? m_foe : m_ai;

		add_seen_move(user.pokemon().regular_moves(), generation, move.selected);
		// TODO: Add move.executed in some circumstances

		call_move(
			user,
			move,
			other,
			other_move,
			m_weather,
			clear_status,
			damage
		);
	}
	// This assumes Species Clause is in effect. This does not perform any
	// switching, it just adds them to the team.
	auto find_or_add_pokemon(bool const is_ai, Species const species, Level const level, Gender const gender) -> Moves {
		auto & switcher = is_ai ? m_ai : m_foe;
		auto const index = find_index(switcher.all_pokemon(), species);
		if (index == switcher.number_of_seen_pokemon()) {
			switcher.all_pokemon().add(Pokemon<generation>(species, level, gender));
		}
		return to_switch(index);
	}
	void handle_fainted(bool const is_ai) {
		active_pokemon(is_ai).set_hp(0_bi);
	}

	void set_value_on_active(bool const is_ai, Ability const ability) {
		(is_ai ? m_ai : m_foe).pokemon().set_base_ability(ability);
	}

	void set_value_on_active(bool const is_ai, Item const item) {
		active_pokemon(is_ai).set_item(item);
	}

	// TODO: What happens here if a Pokemon has a pinch item?
	void correct_hp_and_status(bool const is_ai, VisibleHP const visible_hp, Statuses const visible_status, auto... maybe_index) {
		auto & pokemon = active_pokemon(is_ai, maybe_index...);
		auto const original_hp = pokemon.hp();
		pokemon.set_hp(correct_hp(is_ai, original_hp, visible_hp));
		if (visible_hp.current == 0_bi) {
			return;
		}
		validate_status(pokemon.status().name(), visible_status);
	}

private:
	// maybe_index is either an index into a PokemonCollection or nothing
	auto active_pokemon(bool const is_ai, auto... maybe_index) -> Pokemon<generation> & {
		return (is_ai ? m_ai : m_foe).all_pokemon()(maybe_index...);
	}
	auto correct_hp(bool const is_ai, HP const original_hp, VisibleHP const visible_hp) -> HP::current_type {
		auto const current_hp = original_hp.current();
		auto const seen_hp = to_real_hp(is_ai, original_hp, visible_hp);
		if (seen_hp.min > current_hp or seen_hp.max < current_hp) {
			// TODO: Find a better way to sync this up with server messages. Find a
			// better way to fail unit tests if this happens.
			std::cerr << "HP out of sync with server messages. Expected " << current_hp << " but visible HP is between " << seen_hp.min << " and " << seen_hp.max << " (max of " << original_hp.max() << ")\n";
		}
		return seen_hp.value;
	}
	void validate_status(Statuses const original_status, Statuses const visible_status) {
		auto const normalized_original_status = (original_status == Statuses::rest) ? Statuses::sleep : original_status;
		if (normalized_original_status != visible_status) {
			throw std::runtime_error(containers::concatenate<std::string>(
				std::string_view("Status out of sync with server messages: expected "),
				to_string(original_status),
				std::string_view(" but received "),
				to_string(visible_status)
			));
		}
	}

	Team<generation> m_ai;
	Team<generation> m_foe;
	Weather m_weather;
};

}	// namespace technicalmachine
