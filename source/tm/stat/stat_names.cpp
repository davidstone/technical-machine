// Verify header can stand on its own
// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <tm/stat/stat_names.hpp>

namespace technicalmachine {
namespace {

constexpr auto same_int(BoostableStat const boostable, PermanentStat const permanent, RegularStat const regular) {
	return static_cast<int>(boostable) == static_cast<int>(permanent) and static_cast<int>(boostable) == static_cast<int>(regular);
}

static_assert(same_int(BoostableStat::atk, PermanentStat::atk, RegularStat::atk));
static_assert(same_int(BoostableStat::def, PermanentStat::def, RegularStat::def));
static_assert(same_int(BoostableStat::spa, PermanentStat::spa, RegularStat::spa));
static_assert(same_int(BoostableStat::spd, PermanentStat::spd, RegularStat::spd));
static_assert(same_int(BoostableStat::spe, PermanentStat::spe, RegularStat::spe));

} // namespace
} // namespace technicalmachine