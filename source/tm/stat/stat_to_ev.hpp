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

#include <tm/stat/ev.hpp>
#include <tm/stat/hp.hpp>
#include <tm/stat/iv.hpp>
#include <tm/stat/nature.hpp>
#include <tm/stat/stat.hpp>
#include <tm/stat/stat_names.hpp>

#include <tm/pokemon/level.hpp>

#include <containers/algorithms/transform.hpp>
#include <containers/legacy_iterator.hpp>

#include <stdexcept>

namespace technicalmachine {
using namespace bounded::literal;

template<typename LHS, typename RHS>
constexpr auto round_up_divide(LHS const lhs, RHS const rhs) {
	return lhs / rhs + BOUNDED_CONDITIONAL(lhs % rhs == 0_bi, 0_bi, 1_bi);
}

constexpr auto hp_to_ev(Species const species, Level const level, HP::max_type const stat) {
	auto const stat_range = containers::transform(ev_range(), [=](EV const ev) { return HP(species, level, ev).max(); });
	auto const it = std::lower_bound(containers::legacy_iterator(begin(stat_range)), containers::legacy_iterator(end(stat_range)), stat);
	if (it.base() == end(stat_range)) {
		throw std::runtime_error("No valid HP EV for a given stat value");
	}
	return *it.base().base();
}

// `target` is not just bounded::integer<4, 614> because this function is also
// used in the EV optimizer, where values outside the legal range are regularly
// encountered as part of speculative computation.
template<typename Integer>
inline auto stat_to_ev(Integer const target, Nature const nature, StatNames const stat_name, Stat::base_type const base, IV const iv, Level const level) {
	return bounded::max(0_bi, (round_up_divide((round_up_divide(target, boost(nature, stat_name)) - 5_bi) * 100_bi, level()) - 2_bi * base - iv.value()) * 4_bi);
}

}	// namespace technicalmachine
