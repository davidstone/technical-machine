// Detailed Pokemon stats
// Copyright (C) 2014 David Stone
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
using Probabilities = std::vector<std::pair<double, std::string>>;

Probabilities all_sub_elements(boost::property_tree::ptree const & pt) {
	Probabilities data;
	for (auto const & value : pt) {
		auto const name = value.second.get<std::string>("name");
		auto const probability = value.second.get<double>("probability");
		data.emplace_back(std::make_pair(probability, name));
	}
	return data;
}

template<typename T>
T most_likely_sub_elements(boost::property_tree::ptree const & pt) {
	auto const data = all_sub_elements(pt);
	auto const most_likely = std::max_element(std::begin(data), std::end(data));
	return from_string<T>(most_likely->second);
}

template<typename T>
typename std::vector<T> top_sub_elements(boost::property_tree::ptree const & pt) {
	auto data = all_sub_elements(pt);
	std::sort(std::begin(data), std::end(data), std::greater<Probabilities::value_type>());
	typename std::vector<T> result;
	for (auto const & element : data) {
		result.emplace_back(from_string<T>(element.second));
		if (result.size() == 4) {
			break;
		}
	}
	return result;
}

using SpeciesIndex = bounded::integer<0, number_of_species - 1>;

}	// unnamed namespace

DetailedStats::DetailedStats():
	item(),
	ability(),
	nature() {
	static std::string const file_name = "settings/Generation 4/OU/detailed.xml";
	boost::property_tree::ptree pt;
	read_xml(file_name, pt);
	
	auto const all_stats = pt.get_child("stats");
	for (auto const & value : all_stats) {
		assert(value.first == "pokemon");
		auto const pokemon = value.second;
		auto const species = from_string<Species>(pokemon.get<std::string>("species"));
		SpeciesIndex const species_index(species, bounded::non_check);
		ability[species_index] = most_likely_sub_elements<Ability::Abilities>(pokemon.get_child("abilities"));
		item[species_index] = most_likely_sub_elements<Item>(pokemon.get_child("items"));
		nature[species_index] = most_likely_sub_elements<Nature>(pokemon.get_child("natures"));
		move[species_index] = top_sub_elements<Moves>(pokemon.get_child("moves"));
	}
}

template<>
Ability::Abilities const & DetailedStats::get<Ability::Abilities>(Species const species) const {
	return ability.at(species);
}

template<>
Item const & DetailedStats::get<Item>(Species const species) const {
	return item.at(species);
}

template<>
Nature const & DetailedStats::get<Nature>(Species const species) const {
	return nature.at(species);
}

template<>
std::vector<Moves> const & DetailedStats::get<std::vector<Moves>>(Species const species) const {
	return move.at(species);
}


}	// namespace technicalmachine
