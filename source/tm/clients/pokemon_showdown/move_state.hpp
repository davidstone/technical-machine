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

#include <tm/clients/pokemon_showdown/hp_and_status.hpp>
#include <tm/clients/pokemon_showdown/parsed_hp.hpp>

#include <tm/clients/party.hpp>

#include <tm/move/is_switch.hpp>
#include <tm/move/move.hpp>
#include <tm/move/moves.hpp>
#include <tm/move/used_move.hpp>

#include <bounded/detail/variant/variant.hpp>
#include <bounded/optional.hpp>

#include <stdexcept>

namespace technicalmachine {
namespace ps {

struct MoveState {
	struct NoDamage {};
	struct SubstituteDamaged {};
	struct SubstituteBroke {};
	using Damage = bounded::variant<NoDamage, HPAndStatus, SubstituteDamaged, SubstituteBroke>;
	struct Result {
		Party party;
		UsedMove move;
		Damage damage;
		bounded::optional<HPAndStatus> user_hp_and_status;
		bounded::optional<HPAndStatus> other_hp_and_status;
		bool clear_status;
		bool recoil;
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
			insert(m_party, party);
		}
		if (m_move) {
			m_move->executed = move;
		} else {
			insert(m_move, UsedMove{move});
		}
	}
	
	bool move_damages_self(Party const party) const {
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

	void damage(Party const party, HPAndStatus const hp_and_status) {
		validate(party);
		m_damage = hp_and_status;
	}
	void damage_substitute(Party const party) {
		validate(party);
		m_damage = SubstituteDamaged{};
	}
	void break_substitute(Party const party) {
		validate(party);
		m_damage = SubstituteBroke{};
	}

	void clear_status(Party const party) {
		if (m_party) {
			validate(party);
		} else {
			insert(m_party, party);
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
	void flinch(Party const party) {
		validate(party);
		// TODO: Validate that the used move can cause a flinch
		m_move->variable.set_flinch(true);
	}
	void hp_change(Party const party, HPAndStatus const hp_and_status) {
		if (!m_party) {
			// TODO: Handle end-of-turn damage
			return;
		}
		auto & target_hp_and_status = (*m_party == party) ? m_user_hp_and_status : m_other_hp_and_status;
		insert(target_hp_and_status, hp_and_status);
	}
	void miss(Party const party) {
		validate(party);
		m_move->miss = true;
	}
	template<Generation generation>
	void phaze_index(Party const party, Team<generation> const & phazed_team, Species const species) {
		validate(party);
		if (!is_phaze(m_move->executed)) {
			throw std::runtime_error("We did not use a phazing move, but we were given phazing data");
		}
		m_move->variable.set_phaze_index(phazed_team, species);
	}
	void recoil(Party const party) {
		validate(party);
		m_recoil = true;
	}
	void status(Party const party, Statuses const status) {
		if (m_move and is_switch(m_move->executed)) {
			// Don't need to do anything for Toxic Spikes
			return;
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
		m_move->variable.apply_status(m_move->executed, status);
	}

	auto complete() -> bounded::optional<Result> {
		if (!m_move) {
			*this = {};
			return bounded::none;
		}
		auto const result = Result{
			*m_party,
			*m_move,
			m_damage,
			m_user_hp_and_status,
			m_other_hp_and_status,
			m_clear_status,
			m_recoil
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
	bounded::optional<UsedMove> m_move;
	Damage m_damage{NoDamage()};
	bounded::optional<HPAndStatus> m_user_hp_and_status;
	bounded::optional<HPAndStatus> m_other_hp_and_status;
	bool m_clear_status = false;
	bool m_recoil = false;
};

}	// namespace ps
}	// namespace technicalmachine
