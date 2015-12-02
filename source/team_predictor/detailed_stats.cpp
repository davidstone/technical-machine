// Detailed Pokemon stats
// Copyright (C) 2015 David Stone
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

#include "detailed_stats.hpp"

#include "../ability.hpp"
#include "../item.hpp"

#include "../move/moves.hpp"

#include "../pokemon/species.hpp"

#include "../stat/nature.hpp"

#include "../string_conversions/conversion.hpp"

#include <bounded_integer/bounded_integer.hpp>

#include <containers/iterator_adapter.hpp>
#include <containers/static_vector/static_vector.hpp>
#include <containers/vector/vector.hpp>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <string>

namespace technicalmachine {

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
	auto const most_likely = std::max_element(std::begin(data), std::end(data));
	return from_string<T>(most_likely->second);
}

auto top_sub_elements(boost::property_tree::ptree const & pt) {
	auto data = all_sub_elements(pt);
	auto const middle = (size(data) >= max_moves_per_pokemon) ? data.begin() + max_moves_per_pokemon : data.end();
	std::partial_sort(data.begin(), middle, data.end(), std::greater<>());
	auto adapt = [](auto const & value) { return from_string<Moves>(value.second); };
	return DetailedStats::UsedMoves(containers::iterator_adapter(data.begin(), adapt), middle);
}

}	// namespace

DetailedStats::DetailedStats():
	item(),
	ability(),
	nature() {
	boost::property_tree::ptree pt;
	read_xml("settings/Generation 4/OU/detailed.xml", pt);
	
	auto const all_stats = pt.get_child("stats");
	for (auto const & value : all_stats) {
		assert(value.first == "pokemon");
		auto const pokemon = value.second;
		auto const species = from_string<Species>(pokemon.get<std::string>("species"));
		ability[species] = most_likely_sub_elements<Ability::Abilities>(pokemon.get_child("abilities"));
		item[species] = most_likely_sub_elements<Item>(pokemon.get_child("items"));
		nature[species] = most_likely_sub_elements<Nature>(pokemon.get_child("natures"));
		move[species] = top_sub_elements(pokemon.get_child("moves"));
	}
}

template<>
auto DetailedStats::get<Ability::Abilities>(Species const species) const -> Ability::Abilities const & {
	return ability[species];
}

template<>
auto DetailedStats::get<Item>(Species const species) const -> Item const & {
	return item[species];
}

template<>
auto DetailedStats::get<Nature>(Species const species) const -> Nature const & {
	return nature[species];
}

template<>
auto DetailedStats::get<DetailedStats::UsedMoves>(Species const species) const -> UsedMoves const & {
	return move[species];
}


}	// namespace technicalmachine
