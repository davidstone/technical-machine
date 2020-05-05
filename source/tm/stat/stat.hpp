// Stat data structure (Attack, Defense, Special Attack, Special Defense, Speed)
// Copyright (C) 2015 David Stone
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

#include <tm/stat/ev.hpp>
#include <tm/stat/iv.hpp>
#include <tm/stat/stat_names.hpp>
#include <tm/pokemon/species_forward.hpp>

#include <bounded/integer.hpp>

namespace technicalmachine {
using namespace bounded::literal;

enum class Generation : std::uint8_t;

struct Stat {
	using base_type = bounded::checked_integer<5, 230>;

	Stat(Generation generation, Species name, StatNames stat, EV ev, IV iv = IV(31_bi));
	Stat(Stat other, EV ev);
	Stat(Stat other, EV ev, IV vi);
	
	auto base() const {
		return m_base;
	}
	auto ev() const {
		return m_ev;
	}
	auto iv() const {
		return m_iv;
	}

	friend auto operator==(Stat const &, Stat const &) -> bool = default;
private:
	base_type m_base;
	EV m_ev;
	IV m_iv;
};

}	// namespace technicalmachine
