// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <tm/clients/pokemon_showdown/hp_and_status.hpp>
#include <tm/clients/pokemon_showdown/parsed_hp.hpp>

#include <tm/clients/party.hpp>

#include <tm/move/is_switch.hpp>
#include <tm/move/move.hpp>
#include <tm/move/moves.hpp>
#include <tm/move/used_move.hpp>

#include <tm/pokemon/pokemon_not_found.hpp>

#include <tm/any_team.hpp>
#include <tm/phazing_in_same_pokemon.hpp>
#include <tm/weather.hpp>

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
		UsedMove<Team<generation>> move;
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

	void use_move(Party const party, Moves const move);
	
	bool move_damages_self(Party const party) const;

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
	void phaze_index(Party const party, any_team auto const & phazed_team, Species const species) {
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
	void status(Party const party, Statuses const status);

	template<Generation generation>
	auto complete(Party ai_party, Team<generation> const & ai, Team<generation> const & foe, Weather const weather) -> bounded::optional<Result<generation>>;
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

#define TECHNICALMACHINE_EXTERN_INSTANTIATION(generation) \
	extern template auto MoveState::complete<generation>( \
		Party, \
		Team<generation> const &, \
		Team<generation> const &, \
		Weather \
	) -> bounded::optional<Result<generation>>

TECHNICALMACHINE_EXTERN_INSTANTIATION(Generation::one);
TECHNICALMACHINE_EXTERN_INSTANTIATION(Generation::two);
TECHNICALMACHINE_EXTERN_INSTANTIATION(Generation::three);
TECHNICALMACHINE_EXTERN_INSTANTIATION(Generation::four);
TECHNICALMACHINE_EXTERN_INSTANTIATION(Generation::five);
TECHNICALMACHINE_EXTERN_INSTANTIATION(Generation::six);
TECHNICALMACHINE_EXTERN_INSTANTIATION(Generation::seven);
TECHNICALMACHINE_EXTERN_INSTANTIATION(Generation::eight);

#undef TECHNICALMACHINE_EXTERN_INSTANTIATION

}	// namespace ps
}	// namespace technicalmachine
