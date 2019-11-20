// Optimize offensive EVs and nature to remove waste
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

#include <tm/pokemon/level.hpp>
#include <tm/pokemon/species_forward.hpp>

#include <tm/stat/ev.hpp>
#include <tm/stat/nature.hpp>

#include <bounded/integer.hpp>

#include <containers/static_vector/static_vector.hpp>

namespace technicalmachine {
using namespace bounded::literal;

enum class Generation : std::uint8_t;
struct Stat;

struct OffensiveStats {
	constexpr explicit OffensiveStats(Nature const nature_):
		nature(nature_)
	{
	}

	Nature nature;
	EV attack = EV(0_bi);
	EV special_attack = EV(0_bi);
};

struct OffensiveEVs {
	OffensiveEVs(Generation, Species, Level, Nature, Stat attack, Stat special_attack, bool include_attack_evs, bool include_special_attack_evs);

	auto find(Nature const nature) const -> OffensiveStats const *;
private:
	struct OffensiveData;
	auto equal_stats(Generation, OffensiveData initial, Species species, Level level) -> void;
	containers::static_vector<OffensiveStats, size(containers::enum_range<Nature>()).value()> m_container;
};

}	// namespace technicalmachine
