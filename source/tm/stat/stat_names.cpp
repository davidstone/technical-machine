// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <tm/stat/stat_names.hpp>

namespace technicalmachine {
namespace {

constexpr auto same_int(BoostableStat const boostable, SplitSpecialPermanentStat const permanent, SplitSpecialRegularStat const regular) {
	return static_cast<int>(boostable) == static_cast<int>(permanent) and static_cast<int>(boostable) == static_cast<int>(regular);
}

static_assert(same_int(BoostableStat::atk, SplitSpecialPermanentStat::atk, SplitSpecialRegularStat::atk));
static_assert(same_int(BoostableStat::def, SplitSpecialPermanentStat::def, SplitSpecialRegularStat::def));
static_assert(same_int(BoostableStat::spa, SplitSpecialPermanentStat::spa, SplitSpecialRegularStat::spa));
static_assert(same_int(BoostableStat::spd, SplitSpecialPermanentStat::spd, SplitSpecialRegularStat::spd));
static_assert(same_int(BoostableStat::spe, SplitSpecialPermanentStat::spe, SplitSpecialRegularStat::spe));

} // namespace
} // namespace technicalmachine