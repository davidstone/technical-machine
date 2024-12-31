// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

module;

#include <bounded/conditional.hpp>

export module tm.clients.ps.move_state_builder;

import tm.clients.ps.move_state;
import tm.clients.ps.optional_hp_and_status;

import tm.clients.party;

import tm.move.move_name;
import tm.move.move_result;
import tm.move.recharging;
import tm.move.usable_while_sleeping;

import tm.pokemon.max_pokemon_per_team;
import tm.pokemon.pokemon_not_found;

import tm.status.check_ability_can_status;
import tm.status.status_name;

import tm.string_conversions.move_name;

import tm.ability;
import tm.contact_ability_effect;
import tm.generation;
import tm.visible_hp;

import bounded;
import containers;
import tv;
import std_module;

namespace technicalmachine::ps {
using namespace bounded::literal;
using namespace std::string_view_literals;

constexpr auto move_damages_self(MoveName const move_name) -> bool {
	switch (move_name) {
		case MoveName::Belly_Drum:
		case MoveName::Clangorous_Soul:
		case MoveName::Substitute:
			return true;
		default:
			return false;
	}
}

constexpr auto clears_team_status(MoveName const move) -> bool {
	switch (move) {
		case MoveName::Aromatherapy:
		case MoveName::Heal_Bell:
			return true;
		default:
			return false;
	}
}

constexpr auto is_contact_ability(Ability const ability) -> bool {
	switch (ability) {
		case Ability::Effect_Spore:
		case Ability::Flame_Body:
		case Ability::Poison_Point:
		case Ability::Static:
			return true;
		default:
			return false;
	}
}


export struct MoveStateBuilder {
	constexpr explicit MoveStateBuilder(Party const party_):
		m_party(party_)
	{
	}
	auto party() const -> Party {
		return m_party;
	}
	auto executed_move() const -> tv::optional<MoveName> {
		if (!m_move) {
			return tv::none;
		}
		return tv::visit(*m_move, tv::overload(
			[](VisibleMove const move) -> tv::optional<MoveName> {
				return move.executed;
			},
			[](auto) -> tv::optional<MoveName> {
				return tv::none;
			}
		));
	}

	auto use_move(
		MoveName const move,
		bool const miss,
		bool const action_ends
	) & -> void {
		if (!m_move) {
			set_move_state(VisibleMove(move, miss, action_ends));
			return;
		}
		tv::visit(*m_move, tv::overload(
			[](Awakening) { throw std::runtime_error("Tried to use a move while awakening"); },
			[](Flinched) { throw std::runtime_error("Tried to use a move while flinching"); },
			[](FrozenSolid) { throw std::runtime_error("Tried to use a move while frozen solid"); },
			[](FullyParalyzed) { throw std::runtime_error("Tried to use a move while fully paralyzed"); },
			[](Immobilized) { throw std::runtime_error("Tried to use a move while immobilized"); },
			[](Recharging) { throw std::runtime_error("Tried to use a move while recharging"); },
			[&](StillAsleep) {
				if (!usable_while_sleeping(move)) {
					throw std::runtime_error(containers::concatenate<std::string>("Tried to use "sv, to_string(move), " while asleep"sv));
				}
				m_move->emplace([&] { return VisibleMove(move, miss); });
			},
			[&](VisibleMove & used) {
				if (used.executed != used.selected) {
					throw std::runtime_error("Tried to execute multiple moves");
				}
				used.executed = move;
				used.miss = miss;
			}
		));
	}

	auto move_damaged_self(Party const damaged_party) const -> bool {
		auto const move_should_damage_self = move_damages_self(validated().executed);
		auto const party_says_damaged_self = m_party == damaged_party;
		if (move_should_damage_self != party_says_damaged_self) {
			throw error();
		}
		return move_should_damage_self;
	}

	auto damage(Party const party, VisibleHP const hp) & -> void {
		validated(party).damage = hp;
	}
	auto damage_substitute(Party const party) & -> void {
		validated(party).damage = SubstituteDamaged();
	}
	auto break_substitute(Party const party) & -> void {
		validated(party).damage = SubstituteBroke();
	}

	auto awaken(Generation const generation) & -> void {
		if (m_thaw_or_awaken) {
			throw std::runtime_error("Tried to awaken after a status change");
		}
		if (generation == Generation::one) {
			set_move_state(Awakening());
		}
		tv::insert(m_user.status, StatusName::clear);
		m_thaw_or_awaken = true;
	}
	auto thaw() & -> void {
		if (m_thaw_or_awaken) {
			throw std::runtime_error("Tried to thaw after a status change");
		}
		tv::insert(m_user.status, StatusName::clear);
		m_thaw_or_awaken = true;
	}
	auto confuse(Party const party) & -> void {
		set_used_flag(party, "Tried to confuse a Pokemon twice", &VisibleMove::confuse);
	}
	auto critical_hit(Party const party) & -> void {
		set_used_flag(party, "Tried to critical hit a Pokemon twice", &VisibleMove::critical_hit);
	}
	auto flinch() & -> void {
		set_move_state(Flinched());
	}
	auto frozen_solid() & -> void {
		set_move_state(FrozenSolid());
	}
	auto fully_paralyze() & -> void {
		set_move_state(FullyParalyzed());
	}
	auto immobilize() & -> void {
		set_move_state(Immobilized());
	}
	auto recharge() & -> void {
		set_move_state(Recharging());
	}
	auto set_expected(Party const party, VisibleHP const hp) & -> void {
		auto & target = m_party == party ? m_user : m_other;
		insert(target.hp, hp);
	}
	auto phazed_in(Party const party, TeamIndex const index) & -> void {
		auto & move = validated(party);
		if (!is_phaze(move.executed)) {
			throw std::runtime_error("We used a non-phazing move, but we were given phazing data");
		}
		if (move.phaze_index) {
			throw std::runtime_error("Tried to phaze twice");
		}
		move.phaze_index = index;
	}
	auto recoil(Party const party) & -> void {
		set_used_flag(party, "Tried to recoil a Pokemon twice", &VisibleMove::recoil);
	}

	auto status_from_move(Party const party, StatusName const status) & -> void {
		auto & move = validated();
		if (move.status) {
			throw std::runtime_error("Tried to status a Pokemon twice");
		}
		if (move.executed == MoveName::Rest) {
			if (party != m_party) {
				throw error();
			}
			tv::insert(m_user.status, status);
		} else if (is_phaze(move.executed)) {
			if (party != other(m_party)) {
				throw error();
			}
			tv::insert(m_other.status, status);
		} else if (clears_team_status(move.executed)) {
			if (party != m_party) {
				throw error();
			}
			if (status != StatusName::clear) {
				throw std::runtime_error("Tried to clear status to a status other than clear");
			}
			tv::insert(m_user.status, status);
		} else {
			if (party == m_party) {
				throw error();
			}
			tv::insert(m_other.status, status);
			move.status = status;
		}
	}
	auto status_from_ability(Party const party, Ability const ability, StatusName const status) & -> void {
		auto & move = validated(party);
		if (move.status) {
			throw std::runtime_error("Tried to status a Pokemon twice");
		}
		if (is_contact_ability(ability)) {
			move.contact_ability_effect = status_to_contact_ability_effect(status);
		}
		set_expected(party, status);
	}

	auto set_expected(Party const party, StatusName const status) & -> void {
		auto & target = m_party == party ? m_user : m_other;
		tv::insert(target.status, status);
	}
	auto still_asleep() & -> void {
		if (m_thaw_or_awaken) {
			throw std::runtime_error("Cannot be still asleep at this point");
		}
		set_move_state(StillAsleep());
	}
	auto phaze_index() const -> tv::optional<TeamIndex> {
		if (!m_move) {
			return tv::none;
		}
		return tv::visit(*m_move, tv::overload(
			[](VisibleMove const used) -> tv::optional<TeamIndex> { return used.phaze_index; },
			[](auto const &) -> tv::optional<TeamIndex> { return tv::none; }
		));
	}

	auto complete() const -> tv::optional<MoveState> {
		if (!m_move) {
			return tv::none;
		}
		return MoveState(
			*m_move,
			m_user,
			m_other,
			m_party,
			m_thaw_or_awaken
		);
	}

private:
	auto error() const -> std::runtime_error {
		return std::runtime_error("Received battle messages out of order");
	}

	auto check_is_used() const -> void {
		if (!m_move or m_move->index() != bounded::type<VisibleMove>) {
			throw error();
		}
	}
	auto validated() const & -> VisibleMove const & {
		check_is_used();
		return (*m_move)[bounded::type<VisibleMove>];
	}
	auto validated() & -> VisibleMove & {
		check_is_used();
		return (*m_move)[bounded::type<VisibleMove>];
	}
	auto validated(Party const party) & -> VisibleMove & {
		if (m_party != party) {
			throw std::runtime_error("Inconsistent party");
		}
		return validated();
	}

	auto set_used_flag(
		Party const party,
		char const * const message,
		bool VisibleMove::* const member
	) & -> void {
		auto & flag = validated(party).*member;
		if (flag) {
			throw std::runtime_error(message);
		}
		flag = true;
	}

	auto set_move_state(MoveResult const state) & -> void {
		if (m_move) {
			throw error();
		}
		tv::insert(m_move, state);
	}

	Party m_party;
	tv::optional<MoveResult> m_move;
	OptionalHPAndStatus m_user;
	OptionalHPAndStatus m_other;
	bool m_thaw_or_awaken = false;
};

} // namespace technicalmachine::ps