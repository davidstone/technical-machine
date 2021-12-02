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

#include <tm/any_team.hpp>
#include <tm/end_of_turn.hpp>
#include <tm/known_team.hpp>
#include <tm/seen_team.hpp>
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
	Battle(KnownTeam<generation> ai_, SeenTeam<generation> foe_):
		m_ai(std::move(ai_)),
		m_foe(std::move(foe_))
	{
		// TODO: Properly order this
		auto const ai_pokemon = m_ai.pokemon();
		auto const foe_pokemon = m_foe.pokemon();
		auto do_switch = [&](auto const switcher, auto const other) {
			switcher.switch_in(m_weather);
			activate_ability_on_switch(switcher, other, m_weather);
		};
		do_switch(ai_pokemon, foe_pokemon);
		do_switch(foe_pokemon, ai_pokemon);
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
	void handle_end_turn(bool const ai_went_first, EndOfTurnFlags const first_flags, EndOfTurnFlags const last_flags) & {
		apply_to_teams(ai_went_first, [&](auto & first, auto & last) {
			end_of_turn(first, first_flags, last, last_flags, m_weather);
		});
	}

	void add_move(bool const is_ai, Moves const move_name) {
		apply_to_teams(is_ai, [=](auto & team, auto const &) {
			team.pokemon().add_move(Move(generation, move_name));
		});
	}
	template<any_team UserTeam>
	void handle_use_move(UsedMove<UserTeam> const move, bool const clear_status, ActualDamage const damage, OtherMove const other_move) {
		constexpr auto is_ai = std::is_same_v<UserTeam, KnownTeam<generation_from<UserTeam>>>;

		auto const teams = [&] {
			struct Teams {
				UserTeam & user;
				OtherTeam<UserTeam> & other;
			};
			if constexpr (is_ai) {
				return Teams{m_ai, m_foe};
			} else {
				return Teams{m_foe, m_ai};
			}
		}();
		call_move(
			teams.user,
			move,
			teams.other,
			other_move,
			m_weather,
			clear_status,
			damage
		);
	}
	// This assumes Species Clause is in effect. This does not perform any
	// switching, it just adds them to the team.
	auto find_or_add_pokemon(bool const is_ai, Species const species, containers::string nickname, Level const level, Gender const gender) -> Moves {
		if (is_ai) {
			return to_switch(find_required_index(m_ai.all_pokemon(), species));
		} else {
			auto const index = find_index(m_foe.all_pokemon(), species);
			if (index == m_foe.number_of_seen_pokemon()) {
				m_foe.all_pokemon().add({species, std::move(nickname), level, gender});
			}
			return to_switch(index);
		}
	}
	void handle_fainted(bool const is_ai) {
		apply_to_teams(is_ai, [](auto & team, auto const &) {
			faint(team.pokemon());
		});
	}

	void set_value_on_active(bool const is_ai, Ability const ability) {
		apply_to_teams(is_ai, [=](auto & team, auto const &) {
			team.pokemon().set_base_ability(ability);
		});
	}
	void set_value_on_index(bool const is_ai, TeamIndex const index, Ability const ability) {
		apply_to_teams(is_ai, [=](auto & team, auto const &) {
			team.pokemon(index).set_initial_ability(ability);
		});
	}

	void set_value_on_active(bool const is_ai, Item const item) {
		apply_to_teams(is_ai, [=](auto & team, auto const &) {
			team.all_pokemon()().set_item(item);
		});
	}
	void set_value_on_index(bool const is_ai, TeamIndex const index, Item const item) {
		apply_to_teams(is_ai, [=](auto & team, auto const &) {
			team.pokemon(index).set_item(item);
		});
	}

	// TODO: What happens here if a Pokemon has a pinch item?
	void correct_hp(bool const is_ai, VisibleHP const visible_hp, auto... maybe_index) {
		apply_to_teams(is_ai, [=]<any_team TeamType>(TeamType & team, auto const &) {
			auto & pokemon = team.all_pokemon()(maybe_index...);
			if constexpr (any_seen_team<TeamType>) {
				auto const original_hp = pokemon.visible_hp();
				auto const current_hp = original_hp.current;
				auto const hp_acceptable = current_hp.value() - 1_bi <= visible_hp.current.value() and visible_hp.current.value() <= current_hp.value() + 1_bi;
				if (!hp_acceptable) {
					std::cerr << "Seen HP out of sync with server messages. Expected " << current_hp.value() << " but received " << visible_hp.current.value() << '\n';
				}
				pokemon.set_hp(visible_hp.current);
			} else {
				static_assert(any_known_team<TeamType>);
				if (pokemon.hp().current() != visible_hp.current.value()) {
					std::cerr << "Known HP out of sync with server messages. Expected " << pokemon.hp().current().value() << " but received " << visible_hp.current.value() << " (max of " << pokemon.hp().max().value() << ")\n";
				}
				pokemon.set_hp(visible_hp.current.value());
			}
		});
	}

	void correct_status(bool const is_ai, Statuses const visible_status, auto... maybe_index) {
		apply_to_teams(is_ai, [=](auto const & team, auto &) {
			auto const original_status = team.all_pokemon()(maybe_index...).status().name();
			auto const normalized_original_status = (original_status == Statuses::rest) ? Statuses::sleep : original_status;
			if (normalized_original_status != visible_status) {
				throw std::runtime_error(containers::concatenate<std::string>(
					"Status out of sync with server messages: expected "sv,
					to_string(original_status),
					" but received "sv,
					to_string(visible_status)
				));
			}
		});
	}

private:
	decltype(auto) apply_to_teams(bool const is_ai, auto function) {
		if (is_ai) {
			return function(m_ai, m_foe);
		 } else {
			 return function(m_foe, m_ai);
		 }
	}

	KnownTeam<generation> m_ai;
	SeenTeam<generation> m_foe;
	Weather m_weather;
};

}	// namespace technicalmachine
