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
#include <tm/move/move_name.hpp>
#include <tm/move/used_move.hpp>

#include <tm/pokemon/active_pokemon.hpp>
#include <tm/pokemon/pokemon_not_found.hpp>

#include <tm/any_team.hpp>
#include <tm/contact_ability_effect.hpp>
#include <tm/phazing_in_same_pokemon.hpp>
#include <tm/status_name.hpp>
#include <tm/weather.hpp>

#include <bounded/detail/variant/variant.hpp>
#include <bounded/optional.hpp>
#include <bounded/detail/variant/variant.hpp>

#include <stdexcept>

namespace technicalmachine {

namespace ps {

struct NoDamage {};
struct SubstituteDamaged {};
struct SubstituteBroke {};

struct MoveState {
	struct OptionalHPAndStatus {
		bounded::optional<VisibleHP> hp;
		bounded::optional<StatusName> status;
	};
	using Damage = bounded::variant<NoDamage, HPAndStatus, SubstituteDamaged, SubstituteBroke>;
	template<any_team UserTeam>
	struct Result {
		UsedMove<UserTeam> move;
		Damage damage;
		OptionalHPAndStatus user;
		OptionalHPAndStatus other;
		bool clear_status;
		bool recoil;
	};
	struct NoResult {};
	template<Generation generation>
	using AIResult = Result<KnownTeam<generation>>;
	template<Generation generation>
	using FoeResult = Result<SeenTeam<generation>>;
	template<Generation generation>
	using CompleteResult = bounded::variant<NoResult, AIResult<generation>, FoeResult<generation>>;

	auto party() const -> bounded::optional<Party> {
		return m_party;
	}
	auto executed_move() const {
		return BOUNDED_CONDITIONAL(m_move, m_move->executed, bounded::none);
	}

	void use_move(Party const party, MoveName const move);

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

	void thaw_or_awaken(Party const party) {
		if (m_party) {
			validate(party);
		} else {
			insert(m_party, party);
		}
		if (m_status_change != StatusChange::nothing_relevant) {
			throw std::runtime_error("Tried to thaw or awaken at a weird time");
		}
		set_expected(party, StatusName::clear);
		m_status_change = StatusChange::thaw_or_awaken;
	}
	void confuse() {
		if (!m_party or !m_move) {
			throw error();
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
	void fully_paralyze(Party const party) {
		validate(party);
		if (m_move->fully_paralyze) {
			throw std::runtime_error("Tried to fully paralyze a Pokemon twice");
		}
		m_move->fully_paralyze = true;
	}
	void set_expected(Party const party, VisibleHP const hp) {
		if (!m_party) {
			return;
		}
		auto & target = (*m_party == party) ? m_user : m_other;
		insert(target.hp, hp);
	}
	void miss(Party const party) {
		validate(party);
		if (m_move->miss) {
			throw std::runtime_error("Tried to miss a Pokemon twice");
		}
		m_move->miss = true;
	}
	void phaze_index(Party const party, auto const & phazed_pokemon_collection, Species const species) {
		validate(party);
		if (!is_phaze(m_move->executed)) {
			throw std::runtime_error("We did not use a phazing move, but we were given phazing data");
		}
		if (m_move->phaze_index) {
			throw std::runtime_error("Tried to phaze a Pokemon twice");
		}
		auto const pokemon_index = phazed_pokemon_collection.index();
		auto const new_index = find_required_index(phazed_pokemon_collection, species);
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
	void status_from_move(Party const party, StatusName const status);
	void contact_ability_statuses(Party const party, any_active_pokemon auto const user, Weather const weather, Ability const ability, StatusName const status) {
		validate(party);
		if (m_move->status) {
			throw std::runtime_error("Tried to status a Pokemon twice");
		}
		if (!indirect_status_can_apply(status, user, weather)) {
			throw std::runtime_error("Tried to set status on a Pokemon where status cannot apply");
		}
		apply_contact_ability_status(party, ability, status);
	}

	void set_expected(Party const party, StatusName const status) {
		if (!m_party) {
			return;
		}
		auto & target = (*m_party == party) ? m_user : m_other;
		bounded::insert(target.status, status);
	}
	void still_asleep(Party const party) {
		if (m_party) {
			throw error();
		}
		if (m_status_change != StatusChange::nothing_relevant) {
			throw std::runtime_error("Cannot be still asleep at this point");
		}
		insert(m_party, party);
		m_status_change = StatusChange::still_asleep;
	}
	auto switch_index() const -> bounded::optional<TeamIndex> {
		if (!m_move) {
			return bounded::none;
		}
		return
			is_switch(m_move->executed) ? to_replacement(m_move->executed) :
			m_move->phaze_index ? m_move->phaze_index :
			bounded::none;
	}

	template<Generation generation>
	auto complete(Party ai_party, KnownTeam<generation> const & ai, SeenTeam<generation> const & foe, Weather const weather) -> CompleteResult<generation>;

private:
	void validate(Party const party) const {
		if (m_party != party or !m_move) {
			throw error();
		}
	}

	auto error() const -> std::runtime_error {
		return std::runtime_error("Received battle messages out of order");
	}
	auto apply_contact_ability_status(Party, Ability, StatusName) -> void;

	struct UsedMoveBuilder {
		MoveName selected;
		MoveName executed = selected;
		bool critical_hit = false;
		bool miss = false;
		ContactAbilityEffect contact_ability_effect = ContactAbilityEffect::nothing;
		bool confuse = false;
		bool flinch = false;
		bool fully_paralyze = false;
		bounded::optional<TeamIndex> phaze_index = bounded::none;
		bounded::optional<StatusName> status = bounded::none;
	};

	enum class StatusChange {
		nothing_relevant,
		still_asleep,
		thaw_or_awaken
	};

	bounded::optional<Party> m_party;
	bounded::optional<UsedMoveBuilder> m_move;
	Damage m_damage{NoDamage()};
	OptionalHPAndStatus m_user;
	OptionalHPAndStatus m_other;
	StatusChange m_status_change = StatusChange::nothing_relevant;
	bool m_recoil = false;
	bool m_still_asleep = false;
};

#define TECHNICALMACHINE_EXTERN_INSTANTIATION(generation) \
	extern template auto MoveState::complete( \
		Party, \
		KnownTeam<generation> const &, \
		SeenTeam<generation> const &, \
		Weather \
	) -> CompleteResult<generation>

TECHNICALMACHINE_EXTERN_INSTANTIATION(Generation::one);
TECHNICALMACHINE_EXTERN_INSTANTIATION(Generation::two);
TECHNICALMACHINE_EXTERN_INSTANTIATION(Generation::three);
TECHNICALMACHINE_EXTERN_INSTANTIATION(Generation::four);
TECHNICALMACHINE_EXTERN_INSTANTIATION(Generation::five);
TECHNICALMACHINE_EXTERN_INSTANTIATION(Generation::six);
TECHNICALMACHINE_EXTERN_INSTANTIATION(Generation::seven);
TECHNICALMACHINE_EXTERN_INSTANTIATION(Generation::eight);

#undef TECHNICALMACHINE_EXTERN_INSTANTIATION

} // namespace ps
} // namespace technicalmachine
