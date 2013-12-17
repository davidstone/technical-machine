// Combine all optimized EVs and correct the Pokemon
// Copyright (C) 2013 David Stone
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

#include <bounded_integer/bounded_integer.hpp>

#include "defensive.hpp"
#include "offensive.hpp"
#include "speed.hpp"

#include "../../pokemon/pokemon.hpp"

namespace technicalmachine {

void combine(OffensiveEVs const & o, DefensiveEVs const & d, SpeedEVs const & s, Pokemon & pokemon) {
	typedef std::map<Nature::Natures, EV::total_type> Sums;
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
		auto const sum = speed.second.value() + offensive->second.sum() + defensive->second.sum();
		static_assert(std::numeric_limits<decltype(sum)>::min() == 0_bi, "Minimum EV sum is not 0.");
		if (sum > bounded_integer::make_bounded<EV::max_total>()) {
			continue;
		}
		sums.emplace(speed.first, EV::total_type(sum, bounded_integer::non_check));
	}
	#if 0
	if (sums.empty()) {
		std::cerr << to_string(pokemon) << '\n';
		std::cerr << "Speed:\n";
		for (auto const & value : s.container) {
			std::cerr << '\t' << to_string(value.first) << " : " << value.second.value() << '\n';
		}
		std::cerr << "Offensive:\n";
		for (auto const & value : o.container) {
			std::cerr << '\t' << to_string(value.first) << " : " << value.second.attack.value() << ", " << value.second.attack.value() << '\n';
		}
		std::cerr << "Defensive:\n";
		for (auto const & value : d.container) {
			std::cerr << '\t' << to_string(value.first) << " : " << value.second.hp.value() << ", " << value.second.defense.value() << ", " << value.second.special_defense.value() << '\n';
		}
	}
	#endif
	assert(!sums.empty());
	static constexpr auto lesser_mapped_type = [](Sums::value_type const & lhs, Sums::value_type const & rhs) {
		return lhs.second < rhs.second;
	};
	auto const it = std::min_element(sums.begin(), sums.end(), lesser_mapped_type);
	auto const & defensive = d.container.at(it->first);
	auto const & offensive = o.container.at(it->first);
	auto const & speed = s.container.at(it->first);
	get_stat(pokemon, Stat::HP).ev = defensive.hp;
	calculate_initial_hp(pokemon);
	get_stat(pokemon, Stat::ATK).ev = offensive.attack;
	get_stat(pokemon, Stat::DEF).ev = defensive.defense;
	get_stat(pokemon, Stat::SPA).ev = offensive.special_attack;
	get_stat(pokemon, Stat::SPD).ev = defensive.special_defense;
	get_stat(pokemon, Stat::SPE).ev = speed;
	get_nature(pokemon).name = it->first;
}

}	// namespace technicalmachine
