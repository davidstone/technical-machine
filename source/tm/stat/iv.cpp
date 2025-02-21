// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

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
	using value_type = bounded::integer<0, 31>;

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

	constexpr explicit IV(bounded::tombstone_tag, auto const make):
		m_value(make())
	{
	}
	friend bounded::tombstone<IV>;
	friend bounded::tombstone_member<&IV::m_value>;
};

export constexpr auto default_iv(Generation const generation) {
	return generation <= Generation::two ? IV(30_bi) : IV(31_bi);
}

export struct DV {
	using value_type = bounded::integer<0, 15>;

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

	constexpr explicit DV(bounded::tombstone_tag, auto const make):
		m_value(make())
	{
	}
	friend bounded::tombstone<DV>;
	friend bounded::tombstone_member<&DV::m_value>;
};

export using IVs = GenericStats<IV>;
export using DVs = GenericDVStats<DV>;

export constexpr auto get_hp(IVs const ivs) -> IV {
	return ivs.hp;
}
export constexpr auto get_hp(DVs const dvs) -> DV {
	return DV(
		((dvs.atk.value() % 2_bi) << 3_bi) +
		((dvs.def.value() % 2_bi) << 2_bi) +
		((dvs.spe.value() % 2_bi) << 1_bi) +
		((dvs.spc.value() % 2_bi) << 0_bi)
	);
}

constexpr auto to_ivs(DVs const dvs) -> IVs {
	return IVs(
		IV(get_hp(dvs)),
		IV(dvs.atk),
		IV(dvs.def),
		IV(dvs.spc),
		IV(dvs.spc),
		IV(dvs.spe)
	);
}

export constexpr auto to_dvs_using_spa_as_spc(GenericStats<DV> const stats) -> DVs {
	auto const result = DVs(stats.atk, stats.def, stats.spe, stats.spa);
	auto const hp_dv = get_hp(result);
	if (hp_dv != stats.hp) {
		throw std::runtime_error(containers::concatenate<std::string>(
			"Invalid DVs. Calculated HP DV of "sv,
			containers::to_string(hp_dv.value()),
			" but received "sv,
			containers::to_string(stats.hp.value())
		));
	}
	return result;
}

export constexpr auto to_dvs(GenericStats<DV> const stats) -> DVs {
	if (stats.spa != stats.spd) {
		throw std::runtime_error("Mismatched SpA and SpD DVs in old generation");
	}
	return to_dvs_using_spa_as_spc(stats);
}

export template<SpecialInputStyle style>
constexpr auto max_dvs_or_ivs = [] {
	if constexpr (style == SpecialInputStyle::combined) {
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

export template<SpecialInputStyle style>
using DVsOrIVs = std::conditional_t<style == SpecialInputStyle::combined, DVs, IVs>;

static_assert(get_hp(DVs(DV(15_bi), DV(15_bi), DV(15_bi), DV(15_bi))) == DV(15_bi));
static_assert(get_hp(DVs(DV(15_bi), DV(14_bi), DV(15_bi), DV(15_bi))) == DV(11_bi));
static_assert(get_hp(DVs(DV(15_bi), DV(13_bi), DV(15_bi), DV(15_bi))) == DV(15_bi));
static_assert(get_hp(DVs(DV(14_bi), DV(14_bi), DV(14_bi), DV(14_bi))) == DV(0_bi));

} // namespace technicalmachine

template<>
struct bounded::tombstone<technicalmachine::IV> : bounded::tombstone_member<&technicalmachine::IV::m_value> {
};

template<>
struct bounded::tombstone<technicalmachine::DV> : bounded::tombstone_member<&technicalmachine::DV::m_value> {
};
