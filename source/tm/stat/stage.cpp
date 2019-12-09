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
	for (auto const stat : { StatNames::ATK, StatNames::DEF, StatNames::SPA, StatNames::SPD, StatNames::SPE }) {
		stage[stat] += number_of_stages;
	}
}

auto boost_physical(Stage & stage, Stage::boost_type const number_of_stages) -> void {
	for (auto const stat : { StatNames::ATK, StatNames::DEF }) {
		stage[stat] += number_of_stages;
	}
}
auto boost_special(Stage & stage, Stage::boost_type const number_of_stages) -> void {
	for (auto const stat : { StatNames::SPA, StatNames::SPD }) {
		stage[stat] += number_of_stages;
	}
}
auto boost_defensive(Stage & stage, Stage::boost_type const number_of_stages) -> void {
	for (auto const stat : { StatNames::DEF, StatNames::SPD }) {
		stage[stat] += number_of_stages;
	}
}
auto boost_offensive(Stage & stage, Stage::boost_type const number_of_stages) -> void {
	for (auto const stat : { StatNames::ATK, StatNames::SPA }) {
		stage[stat] += number_of_stages;
	}
}

namespace {

void swap_specified(Stage & lhs, Stage & rhs, std::initializer_list<StatNames> const & stats) {
	using std::swap;
	for (auto const stat : stats) {
		swap(lhs[stat], rhs[stat]);
	}
}

}	// namespace

auto swap_defensive(Stage & lhs, Stage & rhs) -> void {
	swap_specified(lhs, rhs, { StatNames::DEF, StatNames::SPD });
}
auto swap_offensive(Stage & lhs, Stage & rhs) -> void {
	swap_specified(lhs, rhs, { StatNames::ATK, StatNames::SPA });
}


} // namespace technicalmachine
