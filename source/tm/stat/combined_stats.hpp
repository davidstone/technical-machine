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

#include <tm/stat/ev.hpp>
#include <tm/stat/nature.hpp>

#include <tm/operators.hpp>

namespace technicalmachine {

struct CombinedStats {
	Nature nature;
	EV hp;
	EV attack;
	EV defense;
	EV special_attack;
	EV special_defense;
	EV speed;
};
constexpr auto operator==(CombinedStats const lhs, CombinedStats const rhs) {
	return
		lhs.nature == rhs.nature and
		lhs.hp == rhs.hp and
		lhs.attack == rhs.attack and
		lhs.defense == rhs.defense and
		lhs.special_attack == rhs.special_attack and
		lhs.special_defense == rhs.special_defense and
		lhs.speed == rhs.speed;
}

constexpr auto ev_sum(CombinedStats const stats) {
	constexpr auto impl = [](auto... args) { return (... + args.value()); };
	return impl(stats.hp, stats.attack, stats.defense, stats.special_attack, stats.special_defense, stats.speed);
}

}	// namespace technicalmachine
