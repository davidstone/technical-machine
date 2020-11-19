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

#include <tm/pokemon/pokemon_not_found.hpp>

#include <tm/phazing_in_same_pokemon.hpp>

#include <bounded/detail/variant/variant.hpp>
#include <bounded/optional.hpp>

#include <containers/front_back.hpp>
#include <containers/size.hpp>

#include <stdexcept>

namespace technicalmachine {
namespace ps {

struct NoDamage {};
struct SubstituteDamaged {};
struct SubstituteBroke {};

struct MoveState {
	using Damage = bounded::variant<NoDamage, HPAndStatus, SubstituteDamaged, SubstituteBroke>;
	template<Generation generation>
	struct Result {
		Party party;
		UsedMove<generation> move;
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
			insert(m_move, UsedMoveBuilder{move});
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
		if (m_move->confuse) {
			throw std::runtime_error("Tried to confuse a Pokemon twice");
		}
		m_move->confuse = true;
	}
	void critical_hit(Party const party) {
		validate(party);
		if (m_move->critical_hit) {
			throw std::runtime_error("Tried to critical hit a Pokemon twice");
		}
		m_move->critical_hit = true;
	}
	void flinch(Party const party) {
		validate(party);
		if (m_move->flinch) {
			throw std::runtime_error("Tried to flinch a Pokemon twice");
		}
		m_move->flinch = true;
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
		if (m_move->miss) {
			throw std::runtime_error("Tried to miss a Pokemon twice");
		}
		m_move->miss = true;
	}
	template<Generation generation>
	void phaze_index(Party const party, Team<generation> const & phazed_team, Species const species) {
		validate(party);
		if (!is_phaze(m_move->executed)) {
			throw std::runtime_error("We did not use a phazing move, but we were given phazing data");
		}
		if (m_move->phaze_index) {
			throw std::runtime_error("Tried to phaze a Pokemon twice");
		}
		auto const & all = phazed_team.all_pokemon();
		auto const pokemon_index = all.index();
		auto const new_index = find_present_index(all, species);
		if (new_index == pokemon_index) {
			throw PhazingInSamePokemon(new_index);
		}
		m_move->phaze_index = new_index;
	}
	void recoil(Party const party) {
		validate(party);
		if (m_recoil) {
			throw std::runtime_error("Tried to recoil a Pokemon twice");
		}
		m_recoil = true;
	}
	void status(Party const party, Statuses const status) {
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

	template<Generation generation>
	auto complete() -> bounded::optional<Result<generation>> {
		if (!m_move) {
			*this = {};
			return bounded::none;
		}
		auto side_effect = [
			executed = m_move->executed,
			confuse = m_move->confuse,
			flinch = m_move->flinch,
			phaze_index = m_move->phaze_index,
			status = m_move->status
		](auto & user, auto & other, auto & weather, auto const damage) {
			auto const side_effects = possible_side_effects(executed, as_const(user.pokemon()), other, weather);

			if (containers::size(side_effects) == 1_bi) {
				containers::front(side_effects).function(user, other, weather, damage);
				return;
			}

			if (phaze_index) {
				if (confuse or flinch or status) {
					throw std::runtime_error("Tried to phaze and do other side effects.");
				}
				auto const target_index = other.all_pokemon().index();
				using PhazeIndex = bounded::integer<0, static_cast<int>(max_pokemon_per_team - 2_bi)>;
				BOUNDED_ASSERT(phaze_index != target_index);
				auto const effect_index = (*phaze_index < target_index) ? PhazeIndex(*phaze_index) : PhazeIndex(*phaze_index - 1_bi);
				side_effects[effect_index].function(user, other, weather, damage);
				return;
			}

			// TODO: Handle multi-effect situations
			if (confuse or flinch or status) {
				side_effects[1_bi].function(user, other, weather, damage);
			}
		};

		auto const result = Result<generation>{
			*m_party,
			UsedMove<generation>(
				m_move->selected,
				m_move->executed,
				m_move->critical_hit,
				m_move->miss,
				side_effect
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
private:
	void validate(Party const party) const {
		if (m_party != party or !m_move) {
			throw_error();
		}
	}
	
	[[noreturn]] void throw_error() const {
		throw std::runtime_error("Received battle messages out of order");
	}

	struct UsedMoveBuilder {
		Moves selected;
		Moves executed = selected;
		bool critical_hit = false;
		bool miss = false;
		bool confuse = false;
		bool flinch = false;
		bounded::optional<TeamIndex> phaze_index = bounded::none;
		bounded::optional<Statuses> status = bounded::none;
	};


	bounded::optional<Party> m_party;
	bounded::optional<UsedMoveBuilder> m_move;
	Damage m_damage{NoDamage()};
	bounded::optional<HPAndStatus> m_user_hp_and_status;
	bounded::optional<HPAndStatus> m_other_hp_and_status;
	bool m_clear_status = false;
	bool m_recoil = false;
};

}	// namespace ps
}	// namespace technicalmachine
