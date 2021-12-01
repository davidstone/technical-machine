// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <tm/clients/pokemon_showdown/move_state.hpp>

#include <tm/string_conversions/move.hpp>

#include <tm/block.hpp>
#include <tm/team.hpp>

#include <containers/algorithms/concatenate.hpp>

#include <string>
#include <string_view>

namespace technicalmachine::ps {

using namespace std::string_view_literals;

void MoveState::use_move(Party const party, Moves const move) {
	if (m_party) {
		if (*m_party != party) {
			throw std::runtime_error("Early move state messages do not match party of user");
		}
	} else {
		insert(m_party, party);
	}
	if (m_move) {
		m_move->executed = move;
	} else {
		if (m_still_asleep and !usable_while_sleeping(move)) {
			throw std::runtime_error(containers::concatenate<std::string>("Tried to use "sv, to_string(move), " while asleep"sv));
		}
		insert(m_move, UsedMoveBuilder{move});
	}
}

bool MoveState::move_damages_self(Party const party) const {
	if (!m_party or !m_move) {
		throw_error();
	}
	auto const result = [&]{
		switch (m_move->executed) {
			case Moves::Belly_Drum:
			case Moves::Substitute:
				return true;
			default:
				return false;
		}
	}();
	if (result and *m_party != party) {
		throw_error();
	}
	return result;
}

void MoveState::status_from_move(Party const party, Statuses const status) {
	if (!m_move) {
		throw std::runtime_error("Tried to status without an active move");
	}
	if (m_move->status) {
		throw std::runtime_error("Tried to status a Pokemon twice");
	}
	if (is_switch(m_move->executed) or m_move->executed == Moves::Rest) {
		if (party != *m_party) {
			throw_error();
		}
		bounded::insert(m_user.status, status);
	} else {
		if (party == *m_party) {
			throw_error();
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
		using PhazeIndex = bounded::integer<0, static_cast<int>(max_pokemon_per_team - 2_bi)>;
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

} // namespace

template<Generation generation>
auto MoveState::complete(Party const ai_party, KnownTeam<generation> const & ai, SeenTeam<generation> const & foe, Weather const weather) -> CompleteResult<generation> {
	auto generation_one_awaken = [&] {
		return generation == Generation::one and m_clear_status;
	};
	if (!m_move and !m_still_asleep and !generation_one_awaken()) {
		*this = {};
		return CompleteResult<generation>(NoResult());
	}
	if (!m_move) {
		// Technically incorrect with things like Sucker Punch and priority
		insert(m_move, UsedMoveBuilder{Moves::Struggle});
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
				get_side_effect(move, user, other, weather)
			),
			m_damage,
			m_user,
			m_other,
			m_clear_status,
			m_recoil
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