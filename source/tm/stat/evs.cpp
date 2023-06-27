// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

module;

#include <bounded/conditional.hpp>

export module tm.stat.evs;

import tm.stat.generic_stats;
import tm.stat.ev;
import tm.stat.stat_names;

import tm.generation;

import bounded;
import std_module;

namespace technicalmachine {
using namespace bounded::literal;

export using EVs = GenericStats<EV>;

export struct OldGenEVs {
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

export constexpr auto to_old_gen_evs(EVs const evs) -> OldGenEVs {
	if (evs.spa() != evs.spd()) {
		throw std::runtime_error("Mismatched SpA and SpD EVs in old generation");
	}
	return OldGenEVs(evs.hp(), evs.atk(), evs.def(), evs.spe(), evs.spa());
}

export constexpr auto max_total_evs(Generation const generation) {
	return BOUNDED_CONDITIONAL(
		generation <= Generation::two,
		EV::max * 5_bi,
		510_bi
	);
}

export constexpr auto ev_sum(EVs const evs) {
	return evs.hp().value() + evs.atk().value() + evs.def().value() + evs.spa().value() + evs.spd().value() + evs.spe().value();
}
export constexpr auto ev_sum(OldGenEVs const evs) {
	return evs.hp().value() + evs.atk().value() + evs.def().value() + evs.spe().value() + evs.spc().value();
}

} // namespace technicalmachine
