// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

module;

#include <std_module/prelude.hpp>
#include <string>

export module tm.stat.iv;

import tm.stat.generic_stats;
import tm.stat.stat_names;
import tm.stat.stat_style;

import tm.generation;

import bounded;
import containers;
import std_module;

namespace technicalmachine {

using namespace bounded::literal;
using namespace std::string_view_literals;

export struct IV {
	static constexpr auto max = 31;
	using value_type = bounded::integer<0, max>;

	// So it can be stored in a constexpr static_vector
	IV() = default;

	constexpr explicit IV(value_type const iv):
		m_value(iv) {
	}
	constexpr auto value() const -> value_type {
		return m_value;
	}

	friend auto operator<=>(IV, IV) = default;

private:
	value_type m_value;
};

export constexpr auto default_iv(Generation const generation) {
	return generation <= Generation::two ? IV(30_bi) : IV(31_bi);
}

export using IVs = GenericStats<IV>;

export struct DV {
	static constexpr auto max = 15;
	using value_type = bounded::integer<0, max>;

	// So it can be stored in a constexpr static_vector
	DV() = default;

	constexpr explicit DV(value_type const value):
		m_value(value) {
	}
	constexpr explicit DV(IV const iv):
		m_value(iv.value() / 2_bi)
	{
	}
	constexpr auto value() const -> value_type {
		return m_value;
	}

	constexpr explicit operator IV() const {
		return IV(2_bi * value());
	}

	friend auto operator<=>(DV, DV) = default;

private:
	value_type m_value;
};

export struct DVs {
	constexpr DVs(DV const atk_, DV const def_, DV const spe_, DV const spc_):
		m_atk(atk_),
		m_def(def_),
		m_spe(spe_),
		m_spc(spc_)
	{
	}

	constexpr auto atk() const {
		return m_atk;
	}
	constexpr auto def() const {
		return m_def;
	}
	constexpr auto spe() const {
		return m_spe;
	}
	constexpr auto spc() const {
		return m_spc;
	}
	constexpr auto spa() const {
		return m_spc;
	}
	constexpr auto spd() const {
		return m_spc;
	}
	constexpr auto hp() const {
		return DV(
			((atk().value() % 2_bi) << 3_bi) +
			((def().value() % 2_bi) << 2_bi) +
			((spe().value() % 2_bi) << 1_bi) +
			((spc().value() % 2_bi) << 0_bi)
		);
	}

	constexpr explicit operator IVs() const {
		return IVs(
			IV(hp()),
			IV(atk()),
			IV(def()),
			IV(spc()),
			IV(spc()),
			IV(spe())
		);
	}

	constexpr auto operator[](auto const index) const {
		return index_stat(*this, index);
	}
	friend auto operator==(DVs, DVs) -> bool = default;

private:
	DV m_atk;
	DV m_def;
	DV m_spe;
	DV m_spc;
};

export constexpr auto to_dvs_using_spa_as_spc(GenericStats<DV> const stats) -> DVs {
	auto const result = DVs(stats.atk(), stats.def(), stats.spe(), stats.spa());
	if (result.hp() != stats.hp()) {
		throw std::runtime_error(containers::concatenate<std::string>(
			"Invalid DVs. Calculated HP DV of "sv,
			containers::to_string(result.hp().value()),
			" but received "sv,
			containers::to_string(stats.hp().value())
		));
	}
	return result;
}

export constexpr auto to_dvs(GenericStats<DV> const stats) -> DVs {
	if (stats.spa() != stats.spd()) {
		throw std::runtime_error("Mismatched SpA and SpD DVs in old generation");
	}
	return to_dvs_using_spa_as_spc(stats);
}

export template<Generation generation>
constexpr auto max_dvs_or_ivs = [] {
	if constexpr (generation <= Generation::two) {
		constexpr auto value = DV(15_bi);
		return DVs(
			value,
			value,
			value,
			value
		);
	} else {
		constexpr auto value = IV(31_bi);
		return IVs(
			value,
			value,
			value,
			value,
			value,
			value
		);
	}
}();

export template<SpecialStyle style>
using DVsOrIVs = std::conditional_t<style == SpecialStyle::combined, DVs, IVs>;

static_assert(DVs(DV(15_bi), DV(15_bi), DV(15_bi), DV(15_bi)).hp() == DV(15_bi));
static_assert(DVs(DV(15_bi), DV(14_bi), DV(15_bi), DV(15_bi)).hp() == DV(11_bi));
static_assert(DVs(DV(15_bi), DV(13_bi), DV(15_bi), DV(15_bi)).hp() == DV(15_bi));
static_assert(DVs(DV(14_bi), DV(14_bi), DV(14_bi), DV(14_bi)).hp() == DV(0_bi));

} // namespace technicalmachine