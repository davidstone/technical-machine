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

#include <containers/algorithms/concatenate.hpp>
#include <containers/front_back.hpp>
#include <containers/size.hpp>

#include <string>
#include <string_view>
#include <utility>

namespace technicalmachine::ps {

using namespace std::string_view_literals;

void MoveState::use_move(Party const party, MoveName const move) {
	set_party(party);
	if (m_move) {
		m_move->executed = move;
	} else {
		if (m_still_asleep and !usable_while_sleeping(move)) {
			throw std::runtime_error(containers::concatenate<std::string>("Tried to use "sv, to_string(move), " while asleep"sv));
		}
		insert(m_move, UsedMoveBuilder{move});
	}
}

auto MoveState::move_damaged_self(Party const damaged_party) const -> bool {
	if (!m_party or !m_move) {
		throw error();
	}
	auto const result = [&] {
		switch (m_move->executed) {
			case MoveName::Belly_Drum:
			case MoveName::Clangorous_Soul:
			case MoveName::Substitute:
				return true;
			default:
				return false;
		}
	}();
	if (result and *m_party != party) {
		throw error();
	}
	return result;
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
	if (!m_move) {
		throw std::runtime_error("Tried to status without an active move");
	}
	if (m_move->status) {
		throw std::runtime_error("Tried to status a Pokemon twice");
	}
	if (is_switch(m_move->executed) or m_move->executed == MoveName::Rest) {
		if (party != *m_party) {
			throw error();
		}
		bounded::insert(m_user.status, status);
	} else if (is_phaze(m_move->executed)) {
		if (party != other(*m_party)) {
			throw error();
		}
		bounded::insert(m_other.status, status);
	} else if (clears_team_status(m_move->executed)) {
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
		m_move->status = status;
	}
}

namespace {

template<any_active_pokemon UserPokemon>
auto get_side_effect(auto const move, UserPokemon const user, OtherTeam<UserPokemon> const & other, Weather const weather) {
	auto const side_effects = possible_side_effects(move.executed, user, other, weather);

	if (containers::size(side_effects) == 1_bi) {
		return containers::front(side_effects).function;
	}

	if (move.phaze_index) {
		if (move.confuse or move.flinch or move.status) {
			throw std::runtime_error("Tried to phaze and do other side effects.");
		}
		auto const target_index = other.all_pokemon().index();
		using PhazeIndex = bounded::integer<0, bounded::normalize<max_pokemon_per_team - 2_bi>>;
		BOUNDED_ASSERT(move.phaze_index != target_index);
		auto const effect_index = (*move.phaze_index < target_index) ?
			bounded::assume_in_range<PhazeIndex>(*move.phaze_index) :
			bounded::assume_in_range<PhazeIndex>(*move.phaze_index - 1_bi);
		return side_effects[effect_index].function;
	}

	// TODO: Handle multi-effect situations
	if (move.confuse or move.flinch or move.status) {
		return side_effects[1_bi].function;
	}

	return containers::front(side_effects).function;
}

constexpr auto validate_contact_ability_status(Ability const ability, StatusName const status) -> void {
	using namespace std::string_view_literals;
	auto check_status_is_one_of = [&](auto const... valid_statuses) {
		if ((... and (status != valid_statuses))) {
			throw std::runtime_error(containers::concatenate<std::string>(
				"Tried to apply "sv,
				to_string(status),
				" from "sv,
				to_string(ability)
			));
		}
	};
	switch (ability) {
		case Ability::Effect_Spore:
			check_status_is_one_of(StatusName::paralysis, StatusName::poison, StatusName::sleep);
			break;
		case Ability::Flame_Body:
			check_status_is_one_of(StatusName::burn);
			break;
		case Ability::Poison_Point:
			check_status_is_one_of(StatusName::poison);
			break;
		case Ability::Static:
			check_status_is_one_of(StatusName::paralysis);
			break;
		default:
			throw std::runtime_error(containers::concatenate<std::string>(
				"Tried to apply status from a contact move with ability: "sv,
				to_string(ability)
			));
	}
}

} // namespace

auto MoveState::apply_contact_ability_status(Party const party, Ability const ability, StatusName const status) -> void {
	validate_contact_ability_status(ability, status);
	m_move->contact_ability_effect = status_to_contact_ability_effect(status);
	set_expected(party, status);
}

template<Generation generation>
auto MoveState::complete(Party const ai_party, KnownTeam<generation> const & ai, SeenTeam<generation> const & foe, Weather const weather) -> CompleteResult<generation> {
	auto generation_one_awaken = [&] {
		return generation == Generation::one and m_status_change == StatusChange::thaw_or_awaken;
	};
	if (!m_move and m_status_change != StatusChange::still_asleep and !generation_one_awaken()) {
		*this = {};
		return CompleteResult<generation>(NoResult());
	}
	if (!m_move) {
		// Technically incorrect with things like Sucker Punch and priority
		insert(m_move, UsedMoveBuilder{MoveName::Struggle});
	}
	auto const move = *m_move;
	auto execute = [&]<any_active_pokemon UserPokemon>(UserPokemon const user, OtherTeam<UserPokemon> const & other) {
		using UserTeam = AssociatedTeam<UserPokemon>;
		auto const result = Result<UserTeam>{
			UsedMove<UserTeam>(
				move.selected,
				move.executed,
				move.critical_hit,
				move.miss,
				move.contact_ability_effect,
				get_side_effect(move, user, other, weather)
			),
			m_damage,
			m_user,
			m_other,
			m_status_change == StatusChange::thaw_or_awaken,
			move.recoil
		};
		*this = {};
		return CompleteResult<generation>(result);
	};
	if (*m_party == ai_party) {
		return execute(ai.pokemon(), foe);
	} else {
		return execute(foe.pokemon(), ai);
	}
}

#define TECHNICALMACHINE_EXPLICIT_INSTANTIATION(generation) \
	template auto MoveState::complete( \
		Party, \
		KnownTeam<generation> const &, \
		SeenTeam<generation> const &, \
		Weather \
	) -> CompleteResult<generation>

TECHNICALMACHINE_EXPLICIT_INSTANTIATION(Generation::one);
TECHNICALMACHINE_EXPLICIT_INSTANTIATION(Generation::two);
TECHNICALMACHINE_EXPLICIT_INSTANTIATION(Generation::three);
TECHNICALMACHINE_EXPLICIT_INSTANTIATION(Generation::four);
TECHNICALMACHINE_EXPLICIT_INSTANTIATION(Generation::five);
TECHNICALMACHINE_EXPLICIT_INSTANTIATION(Generation::six);
TECHNICALMACHINE_EXPLICIT_INSTANTIATION(Generation::seven);
TECHNICALMACHINE_EXPLICIT_INSTANTIATION(Generation::eight);

} // namespace technicalmachine::ps