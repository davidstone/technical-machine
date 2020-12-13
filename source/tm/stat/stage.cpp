// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <tm/stat/stage.hpp>

#include <tm/rational.hpp>
#include <tm/saturating_add.hpp>

#include <containers/integer_range.hpp>

namespace technicalmachine {
using namespace bounded::literal;

auto boost_regular(Stage & stage, Stage::boost_type const number_of_stages) -> void {
	for (auto const stat : containers::enum_range(BoostableStat::spe)) {
		saturating_add(stage[stat], number_of_stages);
	}
}

auto boost_physical(Stage & stage, Stage::boost_type const number_of_stages) -> void {
	for (auto const stat : {BoostableStat::atk, BoostableStat::def}) {
		saturating_add(stage[stat], number_of_stages);
	}
}
auto boost_special(Stage & stage, Stage::boost_type const number_of_stages) -> void {
	for (auto const stat : {BoostableStat::spa, BoostableStat::spd}) {
		saturating_add(stage[stat], number_of_stages);
	}
}
auto boost_defensive(Stage & stage, Stage::boost_type const number_of_stages) -> void {
	for (auto const stat : {BoostableStat::def, BoostableStat::spd}) {
		saturating_add(stage[stat], number_of_stages);
	}
}
auto boost_offensive(Stage & stage, Stage::boost_type const number_of_stages) -> void {
	for (auto const stat : {BoostableStat::atk, BoostableStat::spa}) {
		saturating_add(stage[stat], number_of_stages);
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
