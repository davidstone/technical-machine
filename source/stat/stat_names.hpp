// Stats data structures
// Copyright (C) 2015 David Stone
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

#include "../enum.hpp"

namespace technicalmachine {

enum class StatNames {
	ATK,
	DEF,
	SPA,
	SPD,
	SPE,
	NORMAL_END,
	ACC = NORMAL_END,
	EVA
};

}	// namespace technicalmachine

namespace std {

template<>
struct numeric_limits<technicalmachine::StatNames> : technicalmachine::enum_numeric_limits<technicalmachine::StatNames::EVA> {};

}	// namespace bounded
