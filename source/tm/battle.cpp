// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

module;

#include <std_module/prelude.hpp>
#include <iostream>

export module tm.battle;

import tm.move.actual_damage;
import tm.move.call_move;
import tm.move.is_switch;
import tm.move.known_move;
import tm.move.move;
import tm.move.move_name;
import tm.move.other_move;
import tm.move.used_move;

import tm.pokemon.faint;
import tm.pokemon.find_required_pokemon_index;
import tm.pokemon.get_hidden_power_type;
import tm.pokemon.level;
import tm.pokemon.max_pokemon_per_team;
import tm.pokemon.seen_pokemon;
import tm.pokemon.species;

import tm.status.status_name;

import tm.string_conversions.status_name;

import tm.type.move_type;

import tm.ability;
import tm.activate_ability_on_switch;
import tm.any_team;
import tm.end_of_turn;
import tm.end_of_turn_flags;
import tm.environment;
import tm.gender;
import tm.generation;
import tm.item;
import tm.other_team;
import tm.team;
import tm.visible_hp;

import bounded;
import containers;
import std_module;

namespace technicalmachine {
using namespace bounded::literal;
using namespace std::string_view_literals;

export template<Generation generation>
struct Battle {
	// TODO: Properly order instead of having a default
	Battle(
		KnownTeam<generation> ai_,
		SeenTeam<generation> foe_,
		bool const ai_first = true
	):
		m_ai(std::move(ai_)),
		m_foe(std::move(foe_))
	{
		auto const ai_pokemon = m_ai.pokemon();
		auto const foe_pokemon = m_foe.pokemon();
		auto do_switch = [&](auto const switcher, auto const other) {
			switcher.switch_in(m_environment);
			activate_ability_on_switch(switcher, other, m_environment);
		};
		if (ai_first) {
			do_switch(ai_pokemon, foe_pokemon);
			do_switch(foe_pokemon, ai_pokemon);
		} else {
			do_switch(ai_pokemon, foe_pokemon);
			do_switch(foe_pokemon, ai_pokemon);
		}
	}

	auto const & ai() const {
		return m_ai;
	}
	auto const & foe() const {
		return m_foe;
	}
	auto environment() const {
		return m_environment;
	}

	// This assumes Species Clause is in effect. Throws if the Species is not in
	// the team.
	auto find_ai_pokemon(Species const species, std::string_view, Level, Gender) const -> TeamIndex {
		// TODO: Validate nickname, level, and gender?
		return find_required_pokemon_index(m_ai.all_pokemon(), species);
	}
	// This assumes Species Clause is in effect. Adds a Pokemon to the team if
	// Species has not been seen yet.
	auto find_or_add_foe_pokemon(Species const species, std::string_view nickname, Level const level, Gender const gender) & -> TeamIndex {
		auto const it = containers::find_if(m_foe.all_pokemon(), [=](SeenPokemon<generation> const & pokemon) {
			return pokemon.species() == species;
		});
		if (it == containers::end(m_foe.all_pokemon())) {
			m_foe.all_pokemon().add({species, std::move(nickname), level, gender});
		}
		return bounded::assume_in_range<TeamIndex>(it - containers::begin(m_foe.all_pokemon()));
	}

	void active_has(bool const is_ai, Ability const ability) {
		apply_to_teams(is_ai, [=](auto & team, auto const &) {
			team.pokemon().set_base_ability(ability);
		});
	}
	void replacement_has(bool const is_ai, TeamIndex const index, Ability const ability) {
		apply_to_teams(is_ai, [=](auto & team, auto const &) {
			team.pokemon(index).set_initial_ability(ability);
		});
	}

	void active_has(bool const is_ai, Item const item) {
		apply_to_teams(is_ai, [=](auto & team, auto const &) {
			team.all_pokemon()().set_item(item);
		});
	}
	void replacement_has(bool const is_ai, TeamIndex const index, Item const item) {
		apply_to_teams(is_ai, [=](auto & team, auto const &) {
			team.pokemon(index).set_item(item);
		});
	}

	void add_move(bool const is_ai, MoveName const move_name) {
		apply_to_teams(is_ai, [=](auto & team, auto const &) {
			team.pokemon().add_move(Move(generation, move_name));
		});
	}
	template<any_team UserTeam>
	void handle_use_move(UsedMove<UserTeam> const move, bool const clear_status, bool const is_fully_paralyzed, ActualDamage const damage) {
		constexpr auto is_ai = std::same_as<UserTeam, KnownTeam<generation_from<UserTeam>>>;

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
		auto const other_pokemon = teams.other.pokemon();
		auto const last_used_move = other_pokemon.last_used_move();
		auto const other_move = last_used_move.moved_this_turn() ?
			OtherMove([&]{
				auto const move_name = last_used_move.name();
				auto const type = move_type(generation, move_name, get_hidden_power_type(other_pokemon));
				return KnownMove{move_name, type};
			}()) :
			OtherMove(FutureMove{
				move.executed == MoveName::Sucker_Punch and damage.did_any_damage()
			});

		call_move(
			teams.user,
			move,
			teams.other,
			other_move,
			m_environment,
			clear_status,
			damage,
			is_fully_paralyzed
		);
	}

	void handle_end_turn(bool const ai_went_first, EndOfTurnFlags const first_flags, EndOfTurnFlags const last_flags) & {
		apply_to_teams(ai_went_first, [&](auto & first, auto & last) {
			end_of_turn(first, first_flags, last, last_flags, m_environment);
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
					std::cerr << "Known HP out of sync with server messages. Expected " << pokemon.hp().current() << " but received " << visible_hp.current.value() << " (max of " << pokemon.hp().max() << ")\n";
					pokemon.set_hp(visible_hp.current.value());
					//throw std::runtime_error("ahh");
				}
			}
		});
	}

	void correct_status(bool const is_ai, StatusName const visible_status, auto... maybe_index) {
		apply_to_teams(is_ai, [=](auto const & team, auto &) {
			auto const original_status = team.all_pokemon()(maybe_index...).status().name();
			auto const normalized_original_status = (original_status == StatusName::rest) ? StatusName::sleep : original_status;
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
	Environment m_environment;
};

} // namespace technicalmachine
