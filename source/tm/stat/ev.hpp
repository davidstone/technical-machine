// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <tm/stat/generic_stats.hpp>

#include <tm/generation.hpp>

#include <bounded/detail/conditional.hpp>
#include <bounded/detail/construct_destroy.hpp>
#include <bounded/integer.hpp>

namespace technicalmachine {
using namespace bounded::literal;

struct EV {
	static constexpr auto max = 255_bi;
	static constexpr auto useful_max = 252_bi;
	using value_type = bounded::integer<0, static_cast<int>(max)>;

	// So it can be stored in a constexpr static_vector
	EV() = default;

	constexpr explicit EV(value_type evs):
		m_value(evs) {
	}
	constexpr auto value() const -> value_type {
		return m_value;
	}

	friend auto operator<=>(EV, EV) = default;
	friend auto operator==(EV, EV) -> bool = default;
	friend constexpr auto operator<=>(EV const lhs, EV::value_type const rhs) {
		return lhs.value() <=> rhs;
	}
	friend constexpr auto operator==(EV const lhs, EV::value_type const rhs) -> bool {
		return lhs.value() == rhs;
	}

private:
	value_type m_value;
};

using EVs = GenericStats<EV>;

struct OldGenEVs {
	constexpr OldGenEVs(EV const hp_, EV const atk_, EV const def_, EV const spe_, EV const spc_):
		m_hp(hp_),
		m_atk(atk_),
		m_def(def_),
		m_spe(spe_),
		m_spc(spc_)
	{
	}

	constexpr auto && hp() const {
		return m_hp;
	}
	constexpr auto && atk() const {
		return m_atk;
	}
	constexpr auto && def() const {
		return m_def;
	}
	constexpr auto && spe() const {
		return m_spe;
	}
	constexpr auto && spc() const {
		return m_spc;
	}
	constexpr auto && spa() const {
		return m_spc;
	}
	constexpr auto && spd() const {
		return m_spc;
	}
	constexpr auto && hp() {
		return m_hp;
	}
	constexpr auto && atk() {
		return m_atk;
	}
	constexpr auto && def() {
		return m_def;
	}
	constexpr auto && spe() {
		return m_spe;
	}
	constexpr auto && spc() {
		return m_spc;
	}
	constexpr auto && spa() {
		return m_spc;
	}
	constexpr auto && spd() {
		return m_spc;
	}

	constexpr auto && operator[](auto const index) const {
		return index_stat(*this, index);
	}
	constexpr auto && operator[](auto const index) {
		return index_stat(*this, index);
	}
	
	friend auto operator==(OldGenEVs, OldGenEVs) -> bool = default;

private:
	EV m_hp;
	EV m_atk;
	EV m_def;
	EV m_spe;
	EV m_spc;
};

constexpr auto max_total_evs(Generation const generation) {
	return
		BOUNDED_CONDITIONAL(generation <= Generation::two, EV::max * 5_bi,
		510_bi
	);
}

constexpr auto ev_sum(EVs const evs) {
	return evs.hp().value() + evs.atk().value() + evs.def().value() + evs.spa().value() + evs.spd().value() + evs.spe().value();
}
constexpr auto ev_sum(OldGenEVs const evs) {
	return evs.hp().value() + evs.atk().value() + evs.def().value() + evs.spe().value() + evs.spc().value();
}

} // namespace technicalmachine
