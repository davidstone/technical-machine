// Copyright (C) 2018 David Stone
//
// This file is part of Technical Machine.
//
// Technical Machine is free software: you can redistribute it and / or modify
// it under the terms of the GNU Affero General Public License as
// published by the Free Software Foundation, either version 3 of the
// License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.

#include <tm/clients/pokemon_showdown/move_state.hpp>

#include <tm/team.hpp>

namespace technicalmachine::ps {

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

void MoveState::status(Party const party, Statuses const status) {
	if (m_move and is_switch(m_move->executed)) {
		// Don't need to do anything for Toxic Spikes
		return;
	}
	if (m_move->status) {
		throw std::runtime_error("Tried to status a Pokemon twice");
	}
	auto update_status = [=](bounded::optional<HPAndStatus> & old_hp_and_status) {
		// TODO: Should update status even if we didn't get HP in the past
		if (old_hp_and_status) {
			old_hp_and_status->status = status;
		}
	};
	if (m_move and m_move->executed == Moves::Rest) {
		if (party != *m_party) {
			throw_error();
		}
		update_status(m_user_hp_and_status);
	} else {
		validate(other(party));
		update_status(m_other_hp_and_status);
	}
	m_move->status = status;
}

namespace {

template<Generation generation>
auto get_side_effect(auto const move, ActivePokemon<generation> const user, Team<generation> const & other, Weather const weather) {
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
		auto const effect_index = (*move.phaze_index < target_index) ? PhazeIndex(*move.phaze_index) : PhazeIndex(*move.phaze_index - 1_bi);
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
auto MoveState::complete(Party const ai_party, Team<generation> const & ai, Team<generation> const & foe, Weather const weather) -> bounded::optional<Result<generation>> {
	if (!m_move) {
		*this = {};
		return bounded::none;
	}
	auto const move = *m_move;
	struct Affected {
		ActivePokemon<generation> user;
		Team<generation> const & other;
	};
	auto const affected = *m_party == ai_party ?
		Affected{ai.pokemon(), foe} :
		Affected{foe.pokemon(), ai};
	auto const result = Result<generation>{
		*m_party,
		UsedMove<generation>(
			move.selected,
			move.executed,
			move.critical_hit,
			move.miss,
			get_side_effect(move, affected.user, affected.other, weather)
		),
		m_damage,
		m_user_hp_and_status,
		m_other_hp_and_status,
		m_clear_status,
		m_recoil
	};
	*this = {};
	return result;
}

#define TECHNICALMACHINE_EXPLICIT_INSTANTIATION(generation) \
	template auto MoveState::complete<generation>( \
		Party, \
		Team<generation> const &, \
		Team<generation> const &, \
		Weather \
	) -> bounded::optional<Result<generation>>

TECHNICALMACHINE_EXPLICIT_INSTANTIATION(Generation::one);
TECHNICALMACHINE_EXPLICIT_INSTANTIATION(Generation::two);
TECHNICALMACHINE_EXPLICIT_INSTANTIATION(Generation::three);
TECHNICALMACHINE_EXPLICIT_INSTANTIATION(Generation::four);
TECHNICALMACHINE_EXPLICIT_INSTANTIATION(Generation::five);
TECHNICALMACHINE_EXPLICIT_INSTANTIATION(Generation::six);
TECHNICALMACHINE_EXPLICIT_INSTANTIATION(Generation::seven);
TECHNICALMACHINE_EXPLICIT_INSTANTIATION(Generation::eight);

} // namespace technicalmachine::ps