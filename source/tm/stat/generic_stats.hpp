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

#include <bounded/detail/tuple.hpp>
#include <bounded/unreachable.hpp>

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
	
	friend auto operator==(GenericStats const &, GenericStats const &) -> bool = default;

private:
	static constexpr auto && operator_bracket(auto && generic, StatNames const index) {
		switch (index) {
			case StatNames::ATK: return generic.attack;
			case StatNames::DEF: return generic.defense;
			case StatNames::SPA: return generic.special_attack;
			case StatNames::SPD: return generic.special_defense;
			case StatNames::SPE: return generic.speed;
			default: bounded::assert_or_assume_unreachable();
		}
	}
};

template<typename HP, typename Stat>
GenericStats(HP, Stat, Stat, Stat, Stat, Stat) -> GenericStats<HP, Stat>;

}	// namespace technicalmachine
