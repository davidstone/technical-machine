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

auto boost_regular(Stages & stages, Stage::boost_type const number_of_stages) -> void {
	for (auto const stat : containers::enum_range(BoostableStat::spe)) {
		stages[stat] += number_of_stages;
	}
}

auto boost_physical(Stages & stages, Stage::boost_type const number_of_stages) -> void {
	for (auto const stat : {BoostableStat::atk, BoostableStat::def}) {
		stages[stat] += number_of_stages;
	}
}
auto boost_special(Stages & stages, Stage::boost_type const number_of_stages) -> void {
	for (auto const stat : {BoostableStat::spa, BoostableStat::spd}) {
		stages[stat] += number_of_stages;
	}
}
auto boost_defensive(Stages & stages, Stage::boost_type const number_of_stages) -> void {
	for (auto const stat : {BoostableStat::def, BoostableStat::spd}) {
		stages[stat] += number_of_stages;
	}
}
auto boost_offensive(Stages & stages, Stage::boost_type const number_of_stages) -> void {
	for (auto const stat : {BoostableStat::atk, BoostableStat::spa}) {
		stages[stat] += number_of_stages;
	}
}

auto swap_defensive(Stages & lhs, Stages & rhs) -> void {
	for (auto const stat : {BoostableStat::def, BoostableStat::spd}) {
		std::swap(lhs[stat], rhs[stat]);
	}
}
auto swap_offensive(Stages & lhs, Stages & rhs) -> void {
	for (auto const stat : {BoostableStat::atk, BoostableStat::spa}) {
		std::swap(lhs[stat], rhs[stat]);
	}
}

namespace {

static_assert([] {
	auto stage = Stage();
	BOUNDED_ASSERT(stage == 0_bi);

	stage += 2_bi;
	BOUNDED_ASSERT(stage == 2_bi);

	stage += 2_bi;
	BOUNDED_ASSERT(stage == 4_bi);

	stage += 2_bi;
	BOUNDED_ASSERT(stage == 6_bi);

	stage += 2_bi;
	BOUNDED_ASSERT(stage == 6_bi);

	return true;
}());

} // namespace

} // namespace technicalmachine
