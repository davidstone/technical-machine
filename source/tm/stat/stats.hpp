// All 'normal' stats that a Pokemon has
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

#include "hp.hpp"
#include "stat.hpp"
#include "../pokemon/species_forward.hpp"

#include <containers/array/array.hpp>

namespace technicalmachine {
struct Level;

struct Stats {
	Stats(Species species, Level level);
	auto hp() const -> HP const &;
	auto hp() -> HP &;
	auto operator[](StatNames stat) const -> Stat const &;
	auto operator[](StatNames stat) -> Stat &;
private:
	HP m_hp;
	containers::array<Stat, 5> m_stats;
};

}	// namespace technicalmachine
