// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <tm/clients/pokemon_showdown/move_state.hpp>

#include <tm/string_conversions/ability.hpp>
#include <tm/string_conversions/move_name.hpp>
#include <tm/string_conversions/status_name.hpp>

#include <tm/block.hpp>
#include <tm/status_name.hpp>
#include <tm/team.hpp>

#include <bounded/overload.hpp>

#include <containers/algorithms/concatenate.hpp>
#include <containers/front_back.hpp>
#include <containers/size.hpp>

#include <string>
#include <string_view>
#include <utility>

namespace technicalmachine::ps {

using namespace std::string_view_literals;

void MoveState::use_move(Party const party, MoveName const move) {
	bounded::visit(m_move, bounded::overload(
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

namespace {

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

} // namespace

auto MoveState::move_damaged_self(Party const damaged_party) const -> bool {
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

namespace {

constexpr auto clears_team_status(MoveName const move) -> bool {
	switch (move) {
		case MoveName::Aromatherapy:
		case MoveName::Heal_Bell:
			return true;
		default:
			return false;
	}
}

} // namespace

void MoveState::status_from_move(Party const party, StatusName const status) {
	auto & move = validated();
	if (move.status) {
		throw std::runtime_error("Tried to status a Pokemon twice");
	}
	if (is_switch(move.executed) or move.executed == MoveName::Rest) {
		if (party != *m_party) {
			throw error();
		}
		bounded::insert(m_user.status, status);
	} else if (is_phaze(move.executed)) {
		if (party != other(*m_party)) {
			throw error();
		}
		bounded::insert(m_other.status, status);
	} else if (clears_team_status(move.executed)) {
		if (party != *m_party) {
			throw error();
		}
		if (status != StatusName::clear) {
			throw std::runtime_error("Tried to clear status to a status other than clear");
		}
		bounded::insert(m_user.status, status);
	} else {
		if (party == *m_party) {
			throw error();
		}
		bounded::insert(m_other.status, status);
		move.status = status;
	}
}

namespace {

constexpr auto contact_ability_statuses(Ability const ability) -> containers::static_vector<StatusName, 3_bi> {
	switch (ability) {
		case Ability::Effect_Spore: return {{StatusName::paralysis, StatusName::poison, StatusName::sleep}};
		case Ability::Flame_Body: return {{StatusName::burn}};
		case Ability::Poison_Point: return {{StatusName::poison}};
		case Ability::Static: return {{StatusName::paralysis}};
		default: return {};
	}
}

} // namespace

auto MoveState::status_from_contact_ability(Party const party, Ability const ability, StatusName const status) & -> void {
	if (!containers::any_equal(contact_ability_statuses(ability), status)) {
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
	move.contact_ability_effect = status_to_contact_ability_effect(status);
	set_expected(party, status);
}

auto MoveState::complete() -> bounded::optional<CompleteResult> {
	auto const is_initial = m_move.index() == bounded::types<InitialMoveResult>();
	if (is_initial and m_status_change != StatusChange::still_asleep) {
		*this = {};
		return bounded::none;
	}
	auto const result = CompleteResult{
		m_move,
		m_user,
		m_other,
		*m_party,
		m_status_change == StatusChange::thaw_or_awaken
	};
	*this = {};
	return CompleteResult(result);
}

} // namespace technicalmachine::ps