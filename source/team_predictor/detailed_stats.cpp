// Detailed Pokemon stats
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

#include "detailed_stats.hpp"

#include <algorithm>
#include <cstddef>
#include <fstream>
#include <string>

#include "../ability.hpp"
#include "../item.hpp"
#include "../invalid_settings_file.hpp"

#include "../move/move.hpp"

#include "../pokemon/pokemon.hpp"
#include "../pokemon/species.hpp"

#include "../stat/nature.hpp"

#include "../string_conversions/conversion.hpp"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

namespace technicalmachine {

namespace {

// Put the double first so that std::max_element works without defining a
// comparison function
typedef std::vector<std::pair<double, std::string>> Probabilities;

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

}	// unnamed namespace

DetailedStats::DetailedStats():
	ability(),
	item(),
	nature() {
	static std::string const file_name = "settings/Generation 4/OU/detailed.xml";
	boost::property_tree::ptree pt;
	read_xml(file_name, pt);
	
	auto const all_stats = pt.get_child("stats");
	for (auto const & value : all_stats) {
		assert(value.first == "pokemon");
		auto const pokemon = value.second;
		auto const species = from_string<Species>(pokemon.get<std::string>("species"));
		ability[static_cast<size_t>(species)] = most_likely_sub_elements<Ability::Abilities>(pokemon.get_child("abilities"));
		item[static_cast<size_t>(species)] = most_likely_sub_elements<Item::Items>(pokemon.get_child("items"));
		nature[static_cast<size_t>(species)] = most_likely_sub_elements<Nature::Natures>(pokemon.get_child("natures"));
		move[static_cast<size_t>(species)] = top_sub_elements<Moves>(pokemon.get_child("moves"));
	}
}

template<>
Ability::Abilities const & DetailedStats::get<Ability::Abilities>(Species const species) const {
	return ability[static_cast<size_t>(species)];
}

template<>
Item::Items const & DetailedStats::get<Item::Items>(Species const species) const {
	return item[static_cast<size_t>(species)];
}

template<>
Nature::Natures const & DetailedStats::get<Nature::Natures>(Species const species) const {
	return nature[static_cast<size_t>(species)];
}

template<>
std::vector<Moves> const & DetailedStats::get<std::vector<Moves>>(Species const species) const {
	return move[static_cast<size_t>(species)];
}


}	// namespace technicalmachine
