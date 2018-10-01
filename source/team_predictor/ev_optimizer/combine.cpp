// Combine all optimized EVs and correct the Pokemon
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

#include "combine.hpp"

#include "defensive.hpp"
#include "offensive.hpp"
#include "speed.hpp"

#include <bounded/integer.hpp>

#include <containers/algorithms/find.hpp>

#include <cassert>

namespace technicalmachine {
namespace {

constexpr auto sum(Combined const combined) {
	return combined.hp.value() + combined.attack.value() + combined.defense.value() + combined.special_attack.value() + combined.special_defense.value() + combined.speed.value();
}

}	// namespace

auto combine(OffensiveEVs const & o, DefensiveEVs const & d, SpeedEVs const & speed_container) -> Combined {
	auto best = bounded::optional<Combined>{};
	for (auto const & speed : speed_container) {
		// Small enough container that a linear search is fine
		auto const offensive = containers::find_if(o.container, [=](auto value) { return value.nature == speed.nature; });
		if (offensive == end(o.container)) {
			continue;
		}
		auto const defensive = d.find(speed.nature);
		if (defensive == end(d)) {
			continue;
		}
		auto candidate = Combined{
			speed.nature,
			defensive->hp,
			offensive->attack,
			defensive->defense,
			offensive->special_attack,
			defensive->special_defense,
			speed.ev
		};
		if (!best or sum(candidate) < sum(*best)) {
			best.emplace(candidate);
		}
	}
	assert(best);
	assert(sum(*best) <= EV::max_total);
	return *best;
}

}	// namespace technicalmachine
