// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

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
#include <tm/visible_hp.hpp>
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
	using Damage = bounded::variant<NoDamage, VisibleHP, SubstituteDamaged, SubstituteBroke>;
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
	auto executed_move() const -> bounded::optional<MoveName> {
		constexpr auto type = bounded::types<UsedMoveBuilder>();
		return BOUNDED_CONDITIONAL(m_move.index() == type, m_move[type].executed, bounded::none);
	}

	void use_move(Party const party, MoveName const move);

	auto move_damaged_self(Party const damaged_party) const -> bool;

	auto damage(Party const party, VisibleHP const hp) & -> void {
		validated(party).damage = hp;
	}
	auto damage_substitute(Party const party) & -> void {
		validated(party).damage = SubstituteDamaged();
	}
	auto break_substitute(Party const party) & -> void {
		validated(party).damage = SubstituteBroke();
	}

	auto awaken(Party const party, Generation const generation) & -> void {
		if (m_status_change != StatusChange::nothing_relevant) {
			throw std::runtime_error("Tried to awaken after a status change");
		}
		if (generation == Generation::one) {
			set_move_state(party, Awakening());
		} else {
			set_party(party);
		}
		set_expected(party, StatusName::clear);
		m_status_change = StatusChange::thaw_or_awaken;
	}
	auto thaw(Party const party) & -> void {
		if (m_status_change != StatusChange::nothing_relevant) {
			throw std::runtime_error("Tried to thaw after a status change");
		}
		set_party(party);
		set_expected(party, StatusName::clear);
		m_status_change = StatusChange::thaw_or_awaken;
	}
	void confuse(Party const party) {
		set_used_flag(party, "Tried to confuse a Pokemon twice", &UsedMoveBuilder::confuse);
	}
	void critical_hit(Party const party) {
		set_used_flag(party, "Tried to critical hit a Pokemon twice", &UsedMoveBuilder::critical_hit);
	}
	void flinch(Party const party) {
		set_move_state(party, Flinched());
	}
	void frozen_solid(Party const party) {
		set_move_state(party, FrozenSolid());
	}
	void fully_paralyze(Party const party) {
		set_move_state(party, FullyParalyzed());
	}
	void recharge(Party const party) {
		set_move_state(party, Recharging());
	}
	void set_expected(Party const party, VisibleHP const hp) {
		if (!m_party) {
			return;
		}
		auto & target = (*m_party == party) ? m_user : m_other;
		insert(target.hp, hp);
	}
	void miss(Party const party) {
		set_used_flag(party, "Tried to miss a Pokemon twice", &UsedMoveBuilder::miss);
	}
	void phaze_index(Party const party, auto const & phazed_pokemon_collection, Species const species) {
		auto & move = validated(party);
		if (!is_phaze(move.executed)) {
			throw std::runtime_error("We used a non-phazing move, but we were given phazing data");
		}
		if (move.phaze_index) {
			throw std::runtime_error("Tried to phaze twice");
		}
		auto const pokemon_index = phazed_pokemon_collection.index();
		auto const new_index = find_required_index(phazed_pokemon_collection, species);
		if (new_index == pokemon_index) {
			throw PhazingInSamePokemon(new_index);
		}
		move.phaze_index = new_index;
	}
	void recoil(Party const party) {
		set_used_flag(party, "Tried to recoil a Pokemon twice", &UsedMoveBuilder::recoil);
	}
	void status_from_move(Party const party, StatusName const status);
	auto status_from_contact_ability(Party const party, Ability const ability, StatusName const status) & -> void;

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
		constexpr auto type = bounded::types<UsedMoveBuilder>();
		if (m_move.index() != type) {
			return bounded::none;
		}
		auto & move = m_move[type];
		return
			is_switch(move.executed) ? to_replacement(move.executed) :
			move.phaze_index ? move.phaze_index :
			bounded::none;
	}

	template<Generation generation>
	auto complete(Party ai_party, KnownTeam<generation> const & ai, SeenTeam<generation> const & foe, Weather const weather) -> CompleteResult<generation>;

private:
	struct Initial {};
	struct Awakening {};
	struct Flinched {};
	struct FrozenSolid {};
	struct FullyParalyzed {};
	struct Recharging {};
	struct UsedMoveBuilder {
		MoveName selected;
		MoveName executed = selected;
		Damage damage = Damage(NoDamage());
		bounded::optional<StatusName> status = bounded::none;
		bounded::optional<TeamIndex> phaze_index = bounded::none;
		ContactAbilityEffect contact_ability_effect = ContactAbilityEffect::nothing;
		bool critical_hit = false;
		bool miss = false;
		bool confuse = false;
		bool recoil = false;
	};
	using Builder = bounded::variant<
		Initial,
		Awakening,
		Flinched,
		FrozenSolid,
		FullyParalyzed,
		Recharging,
		UsedMoveBuilder
	>;

	enum class StatusChange {
		nothing_relevant,
		still_asleep,
		thaw_or_awaken
	};

	auto error() const -> std::runtime_error {
		return std::runtime_error("Received battle messages out of order");
	}

	auto check_party(Party const party) const -> void {
		if (m_party != party) {
			throw std::runtime_error("Inconsistent party");
		}
	}
	auto check_is_used() const -> void {
		if (m_move.index() != bounded::types<UsedMoveBuilder>()) {
			throw error();
		}
	}
	auto validated() const & -> UsedMoveBuilder const & {
		check_is_used();
		return m_move[bounded::types<UsedMoveBuilder>()];
	}
	auto validated() & -> UsedMoveBuilder & {
		check_is_used();
		return m_move[bounded::types<UsedMoveBuilder>()];
	}
	auto validated(Party const party) & -> UsedMoveBuilder & {
		check_party(party);
		return validated();
	}
	auto set_party(Party const party) & -> void {
		if (m_party) {
			check_party(party);
		} else {
			insert(m_party, party);
		}
	}

	auto set_used_flag(Party const party, char const * const message, auto const member) & -> void {
		auto & flag = std::invoke(member, validated(party));
		if (flag) {
			throw std::runtime_error(message);
		}
		flag = true;
	}

	auto set_move_state(Party const party, auto state) & -> void {
		if (m_party or m_move.index() != bounded::types<Initial>()) {
			throw error();
		}
		insert(m_party, party);
		m_move = state;
	}


	bounded::optional<Party> m_party;
	Builder m_move = Builder(Initial());
	OptionalHPAndStatus m_user;
	OptionalHPAndStatus m_other;
	StatusChange m_status_change = StatusChange::nothing_relevant;
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
