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

#include <tm/stat/stage.hpp>

#include <tm/rational.hpp>

namespace technicalmachine {
using namespace bounded::literal;

auto boost_regular(Stage & stage, Stage::boost_type const number_of_stages) -> void {
	for (auto const stat : containers::enum_range(BoostableStat::spe)) {
		stage[stat] += number_of_stages;
	}
}

auto boost_physical(Stage & stage, Stage::boost_type const number_of_stages) -> void {
	for (auto const stat : {BoostableStat::atk, BoostableStat::def}) {
		stage[stat] += number_of_stages;
	}
}
auto boost_special(Stage & stage, Stage::boost_type const number_of_stages) -> void {
	for (auto const stat : {BoostableStat::spa, BoostableStat::spd}) {
		stage[stat] += number_of_stages;
	}
}
auto boost_defensive(Stage & stage, Stage::boost_type const number_of_stages) -> void {
	for (auto const stat : {BoostableStat::def, BoostableStat::spd}) {
		stage[stat] += number_of_stages;
	}
}
auto boost_offensive(Stage & stage, Stage::boost_type const number_of_stages) -> void {
	for (auto const stat : {BoostableStat::atk, BoostableStat::spa}) {
		stage[stat] += number_of_stages;
	}
}

auto swap_defensive(Stage & lhs, Stage & rhs) -> void {
	for (auto const stat : {BoostableStat::def, BoostableStat::spd}) {
		std::swap(lhs[stat], rhs[stat]);
	}
}
auto swap_offensive(Stage & lhs, Stage & rhs) -> void {
	for (auto const stat : {BoostableStat::atk, BoostableStat::spa}) {
		std::swap(lhs[stat], rhs[stat]);
	}
}

} // namespace technicalmachine
