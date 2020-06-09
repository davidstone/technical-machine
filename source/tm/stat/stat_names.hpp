// Stats data structures
// Copyright (C) 2018 David Stone
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

#include <bounded/integer.hpp>

namespace technicalmachine {

// All three of these can convert to each other
enum class BoostableStat {
	atk,
	def,
	spa,
	spd,
	spe,
	acc,
	eva,
};

enum class PermanentStat {
	hp = -1,
	atk,
	def,
	spa,
	spd,
	spe,
};

enum class RegularStat {
	atk,
	def,
	spa,
	spd,
	spe,
};

}	// namespace technicalmachine
namespace bounded {

template<>
inline constexpr auto min_value<technicalmachine::BoostableStat> = technicalmachine::BoostableStat::atk;

template<>
inline constexpr auto max_value<technicalmachine::BoostableStat> = technicalmachine::BoostableStat::eva;

template<>
inline constexpr auto min_value<technicalmachine::PermanentStat> = technicalmachine::PermanentStat::hp;

template<>
inline constexpr auto max_value<technicalmachine::PermanentStat> = technicalmachine::PermanentStat::spe;

template<>
inline constexpr auto min_value<technicalmachine::RegularStat> = technicalmachine::RegularStat::atk;

template<>
inline constexpr auto max_value<technicalmachine::RegularStat> = technicalmachine::RegularStat::spe;

}	// namespace bounded
