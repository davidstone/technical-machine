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

#include <bounded/integer.hpp>
#include "ev.hpp"
#include "iv.hpp"
#include "stat_names.hpp"
#include "../pokemon/species_forward.hpp"

namespace technicalmachine {
using namespace bounded::literal;

struct Pokemon;

struct Stat {
	using base_type = bounded::checked_integer<5, 230>;

	Stat(Species name, StatNames stat, EV ev = EV(0_bi), IV iv = IV(31_bi));
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
private:
	base_type m_base;
	EV m_ev;
	IV m_iv;
};

auto set_stat_ev(Pokemon & pokemon, StatNames stat_name, EV ev) -> void;
auto set_stat_ev(Pokemon & pokemon, StatNames stat_name, EV ev, IV iv) -> void;

}	// namespace technicalmachine
