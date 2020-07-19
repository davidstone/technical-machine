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

#pragma once

#include <tm/stat/stat_to_ev.hpp>

#include <tm/string_conversions/ability.hpp>
#include <tm/string_conversions/gender.hpp>
#include <tm/string_conversions/item.hpp>
#include <tm/string_conversions/nature.hpp>
#include <tm/string_conversions/species.hpp>

#include <tm/generation.hpp>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include <filesystem>

namespace technicalmachine {

template<Generation>
struct Team;

namespace pl {

auto write_move(Move, boost::property_tree::ptree &) -> void;
auto write_stat(std::string_view, IV, EV, boost::property_tree::ptree &) -> void;
auto to_simulator_string(PermanentStat) -> std::string_view;
auto to_simulator_string(Species) -> std::string_view;
auto to_simulator_string(Gender) -> std::string_view;

template<Generation generation>
void write_stats(Pokemon<generation> const & pokemon, boost::property_tree::ptree & pt) {
	auto const stats = calculate_ivs_and_evs(pokemon);
	for (auto const stat_name : {PermanentStat::hp, PermanentStat::atk, PermanentStat::def, PermanentStat::spe, PermanentStat::spa, PermanentStat::spd}) {
		auto const stat = stats[stat_name];
		write_stat(to_simulator_string(stat_name), stat.iv, stat.ev, pt);
	}
}

template<Generation generation>
void write_pokemon(Pokemon<generation> const & pokemon, boost::property_tree::ptree & pt) {
	auto & member = pt.add("pokemon", "");
	member.put("<xmlattr>.species", to_simulator_string(pokemon.species()));
	member.put("nickname", "");
	member.put("level", pokemon.level()());
	member.put("happiness", pokemon.happiness());
	member.put("gender", to_simulator_string(pokemon.gender()));
	member.put("nature", to_string(pokemon.nature()));
	member.put("item", to_string(pokemon.item(false, false)));
	member.put("ability", to_string(pokemon.initial_ability()));
	for (auto const & move : pokemon.regular_moves()) {
		write_move(move, member);
	}
	write_stats(pokemon, member);
}

template<Generation generation>
void write_team(Team<generation> const & team, std::filesystem::path const & file_name) {
	auto pt = boost::property_tree::ptree();
	auto settings = boost::property_tree::xml_writer_settings<boost::property_tree::ptree::key_type>('\t', 1);
	auto & t = pt.add("shoddybattle", "");
	for (auto const & pokemon : team.all_pokemon()) {
		write_pokemon(pokemon, t);
	}
	write_xml(file_name.string(), pt, std::locale(), settings);
}

}	// namespace pl
}	// namespace technicalmachine
