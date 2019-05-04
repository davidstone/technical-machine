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

#include <tm/stat/stat_names.hpp>

#include <bounded/assert.hpp>
#include <bounded/detail/tuple.hpp>

namespace technicalmachine {

template<typename HP, typename Stat = HP>
struct GenericStats {
	HP hp;
	Stat attack;
	Stat defense;
	Stat special_attack;
	Stat special_defense;
	Stat speed;
	
	constexpr auto operator[](StatNames const index) const -> Stat const & {
		return operator_bracket(*this, index);
	}
	constexpr auto operator[](StatNames const index) -> Stat & {
		return operator_bracket(*this, index);
	}
	
	friend constexpr auto operator==(GenericStats const lhs, GenericStats const rhs) {
		auto tie = [](GenericStats const stats) { return bounded::tuple(stats.hp, stats.attack, stats.defense, stats.special_attack, stats.special_defense, stats.speed); };
		return tie(lhs) == tie(rhs);
	}

private:
	template<typename Generic>
	static constexpr auto && operator_bracket(Generic && generic, StatNames const index) {
		switch (index) {
			case StatNames::ATK: return generic.attack;
			case StatNames::DEF: return generic.defense;
			case StatNames::SPA: return generic.special_attack;
			case StatNames::SPD: return generic.special_defense;
			case StatNames::SPE: return generic.speed;
			default: BOUNDED_ASSERT_OR_ASSUME(false);
		}
	}
};

template<typename HP, typename Stat>
GenericStats(HP, Stat, Stat, Stat, Stat, Stat) -> GenericStats<HP, Stat>;

}	// namespace technicalmachine
