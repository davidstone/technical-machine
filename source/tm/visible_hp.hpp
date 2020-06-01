// Copyright (C) 2020 David Stone
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

#include <tm/stat/hp.hpp>

namespace technicalmachine {

// For the AI's Pokemon, this is exact. For the foe's Pokemon, this is a rounded
// fraction out of 42 or 100.
struct VisibleHP {
	HP::current_type current;
	HP::max_type max;
};

struct AllowedHP {
	HP::current_type min;
	HP::current_type value;
	HP::current_type max;
};

auto to_real_hp(bool is_ai, HP, VisibleHP) -> AllowedHP;

} // namespace technicalmachine
