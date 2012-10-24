// Combine all optimized EVs and correct the Pokemon
// Copyright (C) 2012 David Stone
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

#include <algorithm>
#include <cassert>
#include <map>

#include "defensive.hpp"
#include "offensive.hpp"
#include "speed.hpp"

#include "../../pokemon/pokemon.hpp"

#include <iostream>

namespace technicalmachine {

void combine(OffensiveEVs const & o, DefensiveEVs const & d, SpeedEVs const & s, Pokemon & pokemon) {
	typedef std::map<Nature::Natures, unsigned> Sums;
	Sums sums;
	for (auto const & speed : s.container) {
		auto const offensive = o.container.find(speed.first);
		if (offensive == o.container.end()) {
			continue;
		}
		auto const defensive = d.container.find(speed.first);
		if (defensive == d.container.end()) {
			continue;
		}
		unsigned const sum = speed.second + offensive->second.sum() + defensive->second.sum();
		if (sum > 510) {
			continue;
		}
		sums.insert(std::make_pair(speed.first, sum));
	}
	static constexpr auto lesser_mapped_type = [](Sums::value_type const & lhs, Sums::value_type const & rhs) {
		return lhs.second < rhs.second;
	};
	assert(!sums.empty());
	for (auto const & value : sums) {
		std::cerr << Nature(value.first).to_string() << ": " << value.second << '\n';
	}
	auto const it = std::min_element(sums.begin(), sums.end(), lesser_mapped_type);
	std::cerr << "\t" << Nature(it->first).to_string() << ": " << it->second << '\n';
	auto const & defensive = d.container.at(it->first);
	auto const & offensive = o.container.at(it->first);
	auto const & speed = s.container.at(it->first);
	pokemon.hp().ev.set_value(defensive.hp);
	pokemon.atk().ev.set_value(offensive.attack);
	pokemon.def().ev.set_value(defensive.defense);
	pokemon.spa().ev.set_value(offensive.special_attack);
	pokemon.spd().ev.set_value(defensive.special_defense);
	pokemon.spe().ev.set_value(speed);
	pokemon.nature().name = it->first;
}

}	// namespace technicalmachine
