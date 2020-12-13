// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

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
