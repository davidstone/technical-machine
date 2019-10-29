// Stat data structure (Attack, Defense, Special Attack, Special Defense, Speed)
// Copyright (C) 2016 David Stone
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

#include <tm/stat/stat.hpp>

#include <tm/stat/base_stats.hpp>
#include <tm/stat/stat_names.hpp>
#include <tm/pokemon/pokemon.hpp>
#include <tm/pokemon/species.hpp>

#include <bounded/assert.hpp>
#include <bounded/unreachable.hpp>

namespace technicalmachine {

auto set_stat_ev(Pokemon & pokemon, StatNames const stat_name, EV const ev) -> void {
	set_stat_ev(pokemon, stat_name, ev, get_stat(pokemon, stat_name).iv());
}
auto set_stat_ev(Pokemon & pokemon, StatNames const stat_name, EV const ev, IV const iv) -> void {
	auto & stat = get_stat(pokemon, stat_name);
	stat = Stat(stat, ev, iv);
}

namespace {

auto get_base(Generation const generation, Species const species, StatNames const stat) -> Stat::base_type {
	auto const base = BaseStats(generation, species);
	switch (stat) {
		case StatNames::ATK: return base.atk();
		case StatNames::DEF: return base.def();
		case StatNames::SPA: return base.spa();
		case StatNames::SPD: return base.spd();
		case StatNames::SPE: return base.spe();
		default: bounded::assert_or_assume_unreachable();
	}
}

}	// namespace

Stat::Stat(Generation const generation, Species const species, StatNames const stat_name, EV const ev_, IV const iv_) :
	m_base(get_base(generation, species, stat_name)),
	m_ev(ev_),
	m_iv(iv_)
	{
}

Stat::Stat(Stat const other, EV const ev_) :
	m_base(other.base()),
	m_ev(ev_),
	m_iv(other.iv())
	{
}

Stat::Stat(Stat const other, EV const ev_, IV const iv_) :
	m_base(other.base()),
	m_ev(ev_),
	m_iv(iv_)
	{
}

}	// namespace technicalmachine
