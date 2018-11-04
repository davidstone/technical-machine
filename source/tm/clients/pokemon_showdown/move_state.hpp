// Handle Pokemon Showdown messages in a battle
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

#pragma once

#include <tm/clients/party.hpp>

#include <tm/move/move.hpp>
#include <tm/move/moves.hpp>

#include <tm/damage.hpp>
#include <tm/variable.hpp>

#include <bounded/optional.hpp>

#include <stdexcept>

namespace technicalmachine {
namespace ps {

struct MoveState {
	struct Result {
		Party party;
		Moves move;
		Variable variable;
		bounded::optional<damage_type> damage;
		bool awakens;
		bool critical_hit;
		bool hit_self;
		bool miss;
	};

	void use_move(Party const party, Moves const move) {
		assert(!m_data);
		m_data.emplace(party, move);
	}
	
	bool move_damages_self(Party const party) const {
		if (!m_data) {
			throw_error();
		}
		auto const result = m_data->move == Moves::Substitute;
		if (result and m_data->party != party) {
			throw_error();
		}
		return result;
	}

	void awaken(Party const party) {
		validate(party);
		m_awakens = true;
	}
	void critical_hit(Party const party) {
		validate(party);
		m_critical_hit = true;
	}
	void damage(Party const party, damage_type const damage_) {
		validate(party);
		m_damage.emplace(damage_);
	}
	void flinch(Party const party) {
		validate(party);
		// TODO: Validate that the used move can cause a flinch
		set_flinch(m_variable, true);
	}
	void hit_self(Party const party) {
		validate(party);
		m_hit_self = true;
	}
	void miss(Party const party) {
		validate(party);
		m_miss = true;
	}
	void phaze_index(Party const party, Team const & team, Species const species) {
		validate(party);
		if (!is_phaze(m_data->move)) {
			throw std::runtime_error("We did not use a phazing move, but we were given phazing data");
		}
		set_phaze_index(m_variable, team, species);
	}

	auto complete() -> bounded::optional<Result> {
		if (!m_data) {
			return bounded::none;
		}
		auto const initial_data = *m_data;
		auto const result = Result{
			initial_data.party,
			initial_data.move,
			m_variable,
			m_damage,
			m_awakens,
			m_critical_hit,
			m_hit_self,
			m_miss
		};
		*this = {};
		return result;
	}
private:
	void validate(Party const party) const {
		if (!m_data or m_data->party != party) {
			throw_error();
		}
	}
	
	[[noreturn]] void throw_error() const {
		throw std::runtime_error("Received battle messages out of order");
	}
	struct InitialMoveData {
		Party party;
		Moves move;
	};
	bounded::optional<InitialMoveData> m_data;
	bounded::optional<damage_type> m_damage;
	Variable m_variable;
	bool m_awakens = false;
	bool m_critical_hit = false;
	bool m_hit_self = false;
	bool m_miss = false;
};

}	// namespace ps
}	// namespace technicalmachine
