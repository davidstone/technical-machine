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
#include <containers/single_element_range.hpp>
#include <containers/string.hpp>

#include <string_view>

namespace technicalmachine {

enum class Generation : std::uint8_t;
using namespace std::string_view_literals;

namespace ps {

template<Generation generation>
auto to_packed_format(Team<generation> const & team) -> containers::string {
	static constexpr auto separator = '|';
	constexpr auto separator_range = containers::single_element_range(separator);
	auto result = containers::string();
	for (auto const & pokemon : team.all_pokemon()) {
		auto const is_first = containers::is_empty(result);
		auto const item = pokemon.item(false, false);
		result = containers::concatenate<containers::string>(
			std::move(result),
			is_first ? ""sv : "]"sv,
			to_string(pokemon.species()),
			separator_range,
			separator_range,
			item == Item::None ? ""sv : to_string(item),
			separator_range,
			to_string(pokemon.initial_ability()),
			separator_range
		);

		auto const moves = pokemon.regular_moves();
		for (auto it = containers::begin(moves); it != containers::end(moves); ++it) {
			if (it != containers::begin(moves)) {
				containers::push_back(result, ',');
			}
			containers::append(result, to_string(it->name()));
		}

		result = containers::concatenate<containers::string>(
			std::move(result),
			separator_range,
			to_string(pokemon.nature()),
			separator_range
		);

		auto const stats = calculate_ivs_and_evs(pokemon);
		// Spc goes in SpA. SpD is ignored.
		for (auto const stat_name : containers::enum_range<SplitSpecialPermanentStat>()) {
			if (stat_name != SplitSpecialPermanentStat::hp) {
				containers::push_back(result, ',');
			}
			containers::append(result, to_string(stats.evs[stat_name].value()));
		}

		result = containers::concatenate<containers::string>(
			std::move(result),
			separator_range,
			to_string(pokemon.gender()),
			separator_range
		);

		// Spc goes in SpA. SpD is ignored.
		for (auto const stat_name : containers::enum_range<SplitSpecialPermanentStat>()) {
			if (stat_name != SplitSpecialPermanentStat::hp) {
				containers::push_back(result, ',');
			}
			containers::append(result, to_string(stats.dvs_or_ivs[stat_name].value()));
		}

		result = containers::concatenate<containers::string>(
			std::move(result),
			separator_range,
			// Assume non-shiny
			separator_range,
			to_string(pokemon.level()()),
			separator_range
			// Assume max happiness
			// Assume regular Poke Ball
			// Generations with Hyper Training are not yet supported
		);
	}
	return result;
}

} // namespace ps
} // namespace technicalmachine
