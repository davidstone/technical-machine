// Detailed Pokemon stats
// Copyright (C) 2016 David Stone
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

#include <tm/team_predictor/detailed_stats.hpp>

#include <tm/ability.hpp>
#include <tm/item.hpp>

#include <tm/pokemon/species.hpp>

#include <tm/stat/nature.hpp>

#include <tm/string_conversions/ability.hpp>
#include <tm/string_conversions/item.hpp>
#include <tm/string_conversions/nature.hpp>
#include <tm/string_conversions/move.hpp>
#include <tm/string_conversions/pokemon.hpp>

#include <bounded/assert.hpp>
#include <bounded/integer.hpp>

#include <containers/adapt.hpp>
#include <containers/range_view.hpp>
#include <containers/static_vector/static_vector.hpp>
#include <containers/vector.hpp>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include <algorithm>
#include <cstddef>
#include <string>

namespace technicalmachine {
enum class Moves : std::uint16_t;

namespace {

// Put the double first so that std::max_element works without defining a
// comparison function
using Probabilities = containers::vector<std::pair<double, std::string>>;

Probabilities all_sub_elements(boost::property_tree::ptree const & pt) {
	Probabilities data;
	for (auto const & value : pt) {
		auto const name = value.second.get<std::string>("name");
		auto const probability = value.second.get<double>("probability");
		data.emplace_back(probability, name);
	}
	return data;
}

template<typename T>
T most_likely_sub_elements(boost::property_tree::ptree const & pt) {
	auto const data = all_sub_elements(pt);
	auto const most_likely = std::max_element(begin(data), end(data));
	return from_string<T>(most_likely->second);
}

auto top_sub_elements(boost::property_tree::ptree const & pt) {
	auto data = all_sub_elements(pt);
	auto const middle = pointer_from((size(data) >= max_moves_per_pokemon) ? begin(data) + max_moves_per_pokemon : end(data));
	std::partial_sort(pointer_from(begin(data)), middle, pointer_from(end(data)), std::greater<>());
	auto const range = containers::transform(
		containers::range_view(pointer_from(begin(data)), middle),
		[](auto const & probability) { return from_string<Moves>(probability.second); }
	);
	return DetailedStats::UsedMoves(range);
}

}	// namespace

DetailedStats::DetailedStats(std::filesystem::path const & path):
	item(),
	ability(),
	nature() {
	boost::property_tree::ptree pt;
	read_xml(path.string(), pt);
	
	auto const all_stats = pt.get_child("stats");
	for (auto const & value : all_stats) {
		BOUNDED_ASSERT(value.first == "pokemon");
		auto const pokemon = value.second;
		auto const species = from_string<Species>(pokemon.get<std::string>("species"));
		ability[species] = most_likely_sub_elements<Ability>(pokemon.get_child("abilities"));
		item[species] = most_likely_sub_elements<Item>(pokemon.get_child("items"));
		nature[species] = most_likely_sub_elements<Nature>(pokemon.get_child("natures"));
		move[species] = top_sub_elements(pokemon.get_child("moves"));
	}
}

}	// namespace technicalmachine
