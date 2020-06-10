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

#include <tm/stat/iv.hpp>

#include <bounded/integer.hpp>

namespace technicalmachine {

using namespace bounded::literal;

struct DV {
	static constexpr auto max = 15;
	using value_type = bounded::checked_integer<0, max>;
	constexpr explicit DV(value_type const iv):
		m_value(iv) {
	}
	constexpr auto value() const -> bounded::integer<0, max> {
		return m_value;
	}

	constexpr explicit operator IV() const {
		return IV(2_bi * value());
	}

	friend auto operator==(DV const &, DV const &) -> bool = default;
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
