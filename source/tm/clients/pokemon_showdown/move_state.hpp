// Copyright (C) 2019 David Stone
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

#include <tm/move/damage_type.hpp>
#include <tm/move/executed_move.hpp>
#include <tm/move/move.hpp>
#include <tm/move/moves.hpp>

#include <bounded/optional.hpp>

#include <stdexcept>

namespace technicalmachine {
namespace ps {

struct MoveState {
	struct Damage {
		HP::current_type current_hp;
		HP::max_type max_hp;
		Statuses status;
	};
	struct Result {
		Party party;
		ExecutedMove move;
		bounded::optional<Damage> damage;
		bool clear_status;
	};
	
	auto party() const -> bounded::optional<Party> {
		return m_party;
	}
	// Requires that there is an executed move (use_move has been called).
	auto executed_move() const {
		return m_move->executed;
	}

	void use_move(Party const party, Moves const move) {
		if (m_party) {
			if (*m_party != party) {
				throw std::runtime_error("Early move state messages do not match party of user");
			}
		} else {
			m_party.emplace(party);
		}
		if (m_move) {
			m_move->executed = move;
		} else {
			m_move.emplace(move, move);
		}
	}
	
	bool move_damages_self(Party const party) const {
		if (!m_party or !m_move) {
			throw_error();
		}
		auto const result = m_move->executed == Moves::Substitute;
		if (result and *m_party != party) {
			throw_error();
		}
		return result;
	}

	void clear_status(Party const party) {
		if (m_party) {
			validate(party);
		} else {
			m_party.emplace(party);
		}
		m_clear_status = true;
	}
	void confuse() {
		if (!m_party or !m_move) {
			throw_error();
		}
		m_move->variable.confuse();
	}
	void critical_hit(Party const party) {
		validate(party);
		m_move->critical_hit = true;
	}
	void damage(Party const party, Damage const damage_) {
		validate(party);
		m_damage.emplace(damage_);
	}
	void flinch(Party const party) {
		validate(party);
		// TODO: Validate that the used move can cause a flinch
		m_move->variable.set_flinch(true);
	}
	void miss(Party const party) {
		validate(party);
		m_move->miss = true;
	}
	void phaze_index(Party const party, Team const & phazed_team, Species const species) {
		validate(party);
		if (!is_phaze(m_move->executed)) {
			throw std::runtime_error("We did not use a phazing move, but we were given phazing data");
		}
		m_move->variable.set_phaze_index(phazed_team, species);
	}
	void status(Party const party, Statuses const status) {
		if (m_move and is_switch(m_move->executed)) {
			// Don't need to do anything for Toxic Spikes
			return;
		}
		if (m_move and m_move->executed == Moves::Rest) {
			if (party != *m_party) {
				throw_error();
			}
		} else {
			validate(other(party));
		}
		m_move->variable.apply_status(m_move->executed, status);
	}

	auto complete() -> bounded::optional<Result> {
		if (!m_move) {
			m_party = bounded::none;
			return bounded::none;
		}
		auto const result = Result{
			*m_party,
			*m_move,
			m_damage,
			m_clear_status
		};
		*this = {};
		return result;
	}
private:
	void validate(Party const party) const {
		if (m_party != party or !m_move) {
			throw_error();
		}
	}
	
	[[noreturn]] void throw_error() const {
		throw std::runtime_error("Received battle messages out of order");
	}
	bounded::optional<Party> m_party;
	bounded::optional<ExecutedMove> m_move;
	bounded::optional<Damage> m_damage;
	bool m_clear_status = false;
};

}	// namespace ps
}	// namespace technicalmachine
