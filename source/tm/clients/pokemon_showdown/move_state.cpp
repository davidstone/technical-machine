// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

module;

#include <bounded/conditional.hpp>

export module tm.clients.ps.move_state;

import tm.clients.ps.parse_hp;

import tm.clients.move_result;
import tm.clients.party;

import tm.move.is_switch;
import tm.move.move_name;

import tm.pokemon.max_pokemon_per_team;
import tm.pokemon.pokemon_not_found;

import tm.status.status_name;

import tm.string_conversions.ability;
import tm.string_conversions.move_name;
import tm.string_conversions.status_name;

import tm.ability;
import tm.block;
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

constexpr auto ability_statuses(Ability const ability) -> containers::static_vector<StatusName, 4_bi> {
	switch (ability) {
		case Ability::Effect_Spore: return {{StatusName::paralysis, StatusName::poison, StatusName::sleep}};
		case Ability::Flame_Body: return {{StatusName::burn}};
		case Ability::Poison_Point: return {{StatusName::poison}};
		case Ability::Static: return {{StatusName::paralysis}};
		case Ability::Synchronize: return {{StatusName::burn, StatusName::freeze, StatusName::paralysis, StatusName::poison}};
		default: return {};
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


export struct MoveState {
	struct OptionalHPAndStatus {
		tv::optional<VisibleHP> hp;
		tv::optional<StatusName> status;
	};
	struct CompleteResult {
		MoveResult move;
		OptionalHPAndStatus user;
		OptionalHPAndStatus other;
		Party party;
		bool user_status_was_cleared;
	};

	auto party() const -> tv::optional<Party> {
		return m_party;
	}
	auto executed_move() const -> tv::optional<MoveName> {
		constexpr auto type = bounded::type<Used>;
		return BOUNDED_CONDITIONAL(m_move.index() == type, m_move[type].executed, tv::none);
	}

	auto use_move(Party const party, MoveName const move) -> void {
		tv::visit(m_move, tv::overload(
			[](Awakening) { throw std::runtime_error("Tried to use a move while awakening"); },
			[](Flinched) { throw std::runtime_error("Tried to use a move while flinching"); },
			[](FrozenSolid) { throw std::runtime_error("Tried to use a move while frozen solid"); },
			[](FullyParalyzed) { throw std::runtime_error("Tried to use a move while fully paralyzed"); },
			[](Recharging) { throw std::runtime_error("Tried to use a move while recharging"); },
			[&](Used & used) {
				check_party(party);
				if (used.executed != used.selected) {
					throw std::runtime_error("Tried to execute multiple moves");
				}
				used.executed = move;
			},
			[&](InitialMoveResult) {
				set_party(party);
				if (m_status_change == StatusChange::still_asleep and !usable_while_sleeping(move)) {
					throw std::runtime_error(containers::concatenate<std::string>("Tried to use "sv, to_string(move), " while asleep"sv));
				}
				m_move.emplace([&] { return Used{move}; });
			}
		));
	}

	auto move_damaged_self(Party const damaged_party) const -> bool {
		if (!m_party) {
			throw error();
		}
		auto const move_should_damage_self = move_damages_self(validated().executed);
		auto const party_says_damaged_self = *m_party == damaged_party;
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

	auto awaken(Party const party, Generation const generation) & -> void {
		if (m_status_change != StatusChange::nothing_relevant) {
			throw std::runtime_error("Tried to awaken after a status change");
		}
		if (generation == Generation::one) {
			set_move_state(party, Awakening());
		} else {
			set_party(party);
		}
		set_expected(party, StatusName::clear);
		m_status_change = StatusChange::thaw_or_awaken;
	}
	auto thaw(Party const party) & -> void {
		if (m_status_change != StatusChange::nothing_relevant) {
			throw std::runtime_error("Tried to thaw after a status change");
		}
		set_party(party);
		set_expected(party, StatusName::clear);
		m_status_change = StatusChange::thaw_or_awaken;
	}
	auto confuse(Party const party) -> void {
		set_used_flag(party, "Tried to confuse a Pokemon twice", &Used::confuse);
	}
	auto critical_hit(Party const party) -> void {
		set_used_flag(party, "Tried to critical hit a Pokemon twice", &Used::critical_hit);
	}
	auto flinch(Party const party) -> void {
		set_move_state(party, Flinched());
	}
	auto frozen_solid(Party const party) -> void {
		set_move_state(party, FrozenSolid());
	}
	auto fully_paralyze(Party const party) -> void {
		set_move_state(party, FullyParalyzed());
	}
	auto recharge(Party const party) -> void {
		set_move_state(party, Recharging());
	}
	auto set_expected(Party const party, VisibleHP const hp) -> void {
		if (!m_party) {
			return;
		}
		auto & target = (*m_party == party) ? m_user : m_other;
		insert(target.hp, hp);
	}
	auto miss(Party const party) -> void {
		set_used_flag(party, "Tried to miss a Pokemon twice", &Used::miss);
	}
	auto phazed_in(Party const party, TeamIndex const index) -> void {
		auto & move = validated(party);
		if (!is_phaze(move.executed)) {
			throw std::runtime_error("We used a non-phazing move, but we were given phazing data");
		}
		if (move.phaze_index) {
			throw std::runtime_error("Tried to phaze twice");
		}
		move.phaze_index = index;
	}
	auto recoil(Party const party) -> void {
		set_used_flag(party, "Tried to recoil a Pokemon twice", &Used::recoil);
	}

	auto status_from_move(Party const party, StatusName const status) -> void {
		auto & move = validated();
		if (move.status) {
			throw std::runtime_error("Tried to status a Pokemon twice");
		}
		if (is_switch(move.executed) or move.executed == MoveName::Rest) {
			if (party != *m_party) {
				throw error();
			}
			tv::insert(m_user.status, status);
		} else if (is_phaze(move.executed)) {
			if (party != other(*m_party)) {
				throw error();
			}
			tv::insert(m_other.status, status);
		} else if (clears_team_status(move.executed)) {
			if (party != *m_party) {
				throw error();
			}
			if (status != StatusName::clear) {
				throw std::runtime_error("Tried to clear status to a status other than clear");
			}
			tv::insert(m_user.status, status);
		} else {
			if (party == *m_party) {
				throw error();
			}
			tv::insert(m_other.status, status);
			move.status = status;
		}
	}
	auto status_from_ability(Party const party, Ability const ability, StatusName const status) & -> void {
		if (!containers::any_equal(ability_statuses(ability), status)) {
			throw std::runtime_error(containers::concatenate<std::string>(
				"Tried to apply "sv,
				to_string(status),
				" as a result of a contact move against "sv,
				to_string(ability)
			));
		}
		auto & move = validated(party);
		if (move.status) {
			throw std::runtime_error("Tried to status a Pokemon twice");
		}
		if (is_contact_ability(ability)) {
			move.contact_ability_effect = status_to_contact_ability_effect(status);
		}
		set_expected(party, status);
	}

	auto set_expected(Party const party, StatusName const status) -> void {
		if (!m_party) {
			return;
		}
		auto & target = (*m_party == party) ? m_user : m_other;
		tv::insert(target.status, status);
	}
	auto still_asleep(Party const party) -> void {
		if (m_party) {
			throw error();
		}
		if (m_status_change != StatusChange::nothing_relevant) {
			throw std::runtime_error("Cannot be still asleep at this point");
		}
		insert(m_party, party);
		m_status_change = StatusChange::still_asleep;
	}
	auto switch_index() const -> tv::optional<TeamIndex> {
		constexpr auto type = bounded::type<Used>;
		if (m_move.index() != type) {
			return tv::none;
		}
		auto & move = m_move[type];
		return
			is_switch(move.executed) ? to_replacement(move.executed) :
			move.phaze_index;
	}

	auto complete() -> tv::optional<CompleteResult> {
		auto const is_initial = m_move.index() == bounded::type<InitialMoveResult>;
		if (is_initial and m_status_change != StatusChange::still_asleep) {
			*this = {};
			return tv::none;
		}
		auto const result = CompleteResult{
			m_move,
			m_user,
			m_other,
			*m_party,
			m_status_change == StatusChange::thaw_or_awaken
		};
		*this = {};
		return result;
	}

private:
	enum class StatusChange {
		nothing_relevant,
		still_asleep,
		thaw_or_awaken
	};

	auto error() const -> std::runtime_error {
		return std::runtime_error("Received battle messages out of order");
	}

	auto check_party(Party const party) const -> void {
		if (m_party != party) {
			throw std::runtime_error("Inconsistent party");
		}
	}
	auto check_is_used() const -> void {
		if (m_move.index() != bounded::type<Used>) {
			throw error();
		}
	}
	auto validated() const & -> Used const & {
		check_is_used();
		return m_move[bounded::type<Used>];
	}
	auto validated() & -> Used & {
		check_is_used();
		return m_move[bounded::type<Used>];
	}
	auto validated(Party const party) & -> Used & {
		check_party(party);
		return validated();
	}
	auto set_party(Party const party) & -> void {
		if (m_party) {
			check_party(party);
		} else {
			insert(m_party, party);
		}
	}

	auto set_used_flag(Party const party, char const * const message, auto const member) & -> void {
		auto & flag = std::invoke(member, validated(party));
		if (flag) {
			throw std::runtime_error(message);
		}
		flag = true;
	}

	auto set_move_state(Party const party, auto state) & -> void {
		if (m_party or m_move.index() != bounded::type<InitialMoveResult>) {
			throw error();
		}
		insert(m_party, party);
		m_move = state;
	}

	tv::optional<Party> m_party;
	MoveResult m_move = MoveResult(InitialMoveResult());
	OptionalHPAndStatus m_user;
	OptionalHPAndStatus m_other;
	StatusChange m_status_change = StatusChange::nothing_relevant;
};

} // namespace technicalmachine::ps