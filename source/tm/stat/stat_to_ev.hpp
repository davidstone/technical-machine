// Copyright (C) 2019 David Stone
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

#include <tm/stat/combined_stats.hpp>
#include <tm/stat/ev.hpp>
#include <tm/stat/generic_stats.hpp>
#include <tm/stat/hp.hpp>
#include <tm/stat/iv.hpp>
#include <tm/stat/nature.hpp>
#include <tm/stat/stat.hpp>
#include <tm/stat/stat_names.hpp>

#include <tm/pokemon/level.hpp>
#include <tm/pokemon/pokemon.hpp>
#include <tm/pokemon/species_forward.hpp>

namespace technicalmachine {
enum class Generation : std::uint8_t;
using namespace bounded::literal;

constexpr auto round_up_divide(auto const lhs, auto const rhs) {
	return lhs / rhs + BOUNDED_CONDITIONAL(lhs % rhs == 0_bi, 0_bi, 1_bi);
}

auto hp_to_ev(Generation, Species, Level, HP::max_type, IV) -> EV;

// `target` is not just bounded::integer<4, 614> because this function is also
// used in the EV optimizer, where values outside the legal range are regularly
// encountered as part of speculative computation.
constexpr auto stat_to_ev(auto const target, Nature const nature, StatNames const stat_name, Stat::base_type const base, IV const iv, Level const level) {
	return bounded::max(
		0_bi,
		(round_up_divide((round_up_divide(target, boost(nature, stat_name)) - 5_bi) * 100_bi, level()) - 2_bi * base - iv.value()) * 4_bi
	);
}

using StatValue = bounded::integer<4, 614>;
auto calculate_evs(Generation, Species, Level, GenericStats<HP::max_type, StatValue>, IVs) -> CombinedStats<EV>;
auto calculate_evs(Generation, Pokemon) -> CombinedStats<EV>;

} // namespace technicalmachine
