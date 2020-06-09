// Verify header can stand on its own
// Copyright (C) 2013 David Stone
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