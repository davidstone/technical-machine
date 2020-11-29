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

#include <tm/stat/stat_to_ev.hpp>

#include <tm/string_conversions/ability.hpp>
#include <tm/string_conversions/gender.hpp>
#include <tm/string_conversions/item.hpp>
#include <tm/string_conversions/move.hpp>
#include <tm/string_conversions/nature.hpp>
#include <tm/string_conversions/species.hpp>

#include <tm/team.hpp>

#include <containers/begin_end.hpp>
#include <containers/integer_range.hpp>
#include <containers/is_empty.hpp>
#include <containers/string.hpp>

#include <string_view>

namespace technicalmachine {

enum class Generation : std::uint8_t;

namespace ps {

template<Generation generation>
auto to_packed_format(Team<generation> const & team) -> containers::string {
	auto result = containers::string();
	for (auto const & pokemon : team.all_pokemon()) {
		if (!containers::is_empty(result)) {
			result += ']';
		}
		result += to_string(pokemon.species());
		result += "||";
		auto const item = pokemon.item(false, false);
		result += item == Item::None ? "" : to_string(item);
		result += '|';
		result += to_string(pokemon.initial_ability());
		result += '|';
		auto const moves = pokemon.regular_moves();
		for (auto it = containers::begin(moves); it != containers::end(moves); ++it) {
			if (it != containers::begin(moves)) {
				result += ',';
			}
			result += to_string(it->name());
		}
		result += '|';
		result += to_string(pokemon.nature());

		result += '|';
		auto const stats = calculate_ivs_and_evs(pokemon);

		for (auto const stat_name : containers::enum_range<PermanentStat>()) {
			if (stat_name != PermanentStat::hp) {
				result += ',';
			}
			result += std::string_view(to_string(stats[stat_name].ev.value()));
		}

		result += '|';
		result += to_string(pokemon.gender());

		result += '|';
		for (auto const stat_name : containers::enum_range<PermanentStat>()) {
			if (stat_name != PermanentStat::hp) {
				result += ',';
			}
			result += std::string_view(to_string(stats[stat_name].iv.value()));
		}

		result += '|';
		// Assume non-shiny
		result += '|';
		result += std::string_view(to_string(pokemon.level()()));
		result += '|';
		// Assume max happiness
		// Assume regular Poke Ball
		// Generations with Hyper Training are not yet supported
	}
	return result;
}

} // namespace ps
} // namespace technicalmachine
