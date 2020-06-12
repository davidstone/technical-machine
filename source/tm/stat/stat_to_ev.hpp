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
#include <tm/stat/iv_and_ev.hpp>
#include <tm/stat/nature.hpp>
#include <tm/stat/initial_stat.hpp>
#include <tm/stat/stat_names.hpp>

#include <tm/pokemon/level.hpp>
#include <tm/pokemon/pokemon.hpp>
#include <tm/pokemon/species_forward.hpp>

#include <bounded/optional.hpp>

namespace technicalmachine {
enum class Generation : std::uint8_t;
using namespace bounded::literal;

constexpr auto round_up_divide(auto const lhs, auto const rhs) {
	return lhs / rhs + BOUNDED_CONDITIONAL(lhs % rhs == 0_bi, 0_bi, 1_bi);
}

inline auto hp_to_ev(BaseStats const base, Level const level, HP::max_type const stat, IV const iv) -> EV {
	if (base.hp() == 1_bi) {
		return EV(0_bi);
	}
	auto const computed = (round_up_divide((stat - level() - 10_bi) * 100_bi, level()) - iv.value() - 2_bi * base.hp()) * 4_bi;
	return EV(EV::value_type(bounded::max(0_bi, computed)));
}

// `target` is not just InitialStat because this function is also used in the EV
// optimizer, where values outside the legal range are regularly encountered as
// part of speculative computation.
constexpr auto stat_to_ev(bounded::bounded_integer auto const target, Nature const nature, RegularStat const stat_name, BaseStats::regular_value_type const base, IV const iv, Level const level) -> bounded::optional<EV> {
	auto const computed = (round_up_divide((round_up_divide(target, boost(nature, stat_name)) - 5_bi) * 100_bi, level()) - 2_bi * base - iv.value()) * 4_bi;
	if (computed > EV::max) {
		return bounded::none;
	}
	return EV(EV::value_type(bounded::max(0_bi, computed), bounded::non_check));
}

auto calculate_ivs_and_evs(Generation, Species, Level, GenericStats<HP::max_type, InitialStat>, bounded::optional<Type> hidden_power_type, bool has_physical_move) -> CombinedStats<IVAndEV>;
auto calculate_ivs_and_evs(Generation, Pokemon) -> CombinedStats<IVAndEV>;

} // namespace technicalmachine
