// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <tm/stat/generic_stats.hpp>
#include <tm/stat/iv.hpp>

#include <bounded/integer.hpp>

namespace technicalmachine {

using namespace bounded::literal;

struct DV {
	static constexpr auto max = 15;
	using value_type = bounded::integer<0, max>;
	constexpr explicit DV(value_type const iv):
		m_value(iv) {
	}
	constexpr auto value() const -> value_type {
		return m_value;
	}

	constexpr explicit operator IV() const {
		return IV(2_bi * value());
	}

	friend auto operator==(DV, DV) -> bool = default;
private:
	value_type m_value;
};

struct DVs {
	DV atk;
	DV def;
	DV spe;
	DV spc;

	constexpr explicit operator IVs() const {
		return IVs{
			IV(hp()),
			IV(atk),
			IV(def),
			IV(spc),
			IV(spc),
			IV(spe)
		};
	}

private:
	constexpr auto hp() const -> DV {
		return DV(
			((atk.value() % 2_bi) << 3_bi) +
			((def.value() % 2_bi) << 2_bi) +
			((spe.value() % 2_bi) << 1_bi) +
			((spc.value() % 2_bi) << 0_bi)
		);
	}
};

}	// namespace technicalmachine
