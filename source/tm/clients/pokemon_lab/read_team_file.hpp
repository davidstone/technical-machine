// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <tm/string_conversions/ability.hpp>
#include <tm/string_conversions/gender.hpp>
#include <tm/string_conversions/item.hpp>

#include <tm/generation.hpp>
#include <tm/team.hpp>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include <filesystem>
#include <string>
#include <string_view>

namespace technicalmachine {
namespace pl {

auto stat_from_simulator_string(std::string_view) -> PermanentStat;
auto load_stats(boost::property_tree::ptree const &) -> CombinedStats<IVAndEV>;
auto species_from_simulator_string(std::string_view) -> Species;
auto load_moves(Generation, boost::property_tree::ptree const &) -> RegularMoves;

template<Generation generation>
auto load_pokemon(boost::property_tree::ptree const & pt) {
	// auto const given_nickname = pt.get<std::string>("nickname");
	// auto const nickname = nickname_temp.empty() ? species_str : given_nickname;
	return Pokemon<generation>(
		species_from_simulator_string(pt.get<std::string>("<xmlattr>.species")),
		Level(pt.get<Level::value_type>("level")),
		Gender(from_string<Gender>(pt.get<std::string>("gender"))),
		from_string<Item>(pt.get<std::string>("item")),
		Ability(from_string<Ability>(pt.get<std::string>("ability"))),
		load_stats(pt),
		load_moves(generation, pt.get_child("moveset")),
		Happiness(pt.get<Happiness::value_type>("happiness"))
	);
}

template<Generation generation>
auto load_team(std::filesystem::path const & team_file) {
	boost::property_tree::ptree pt;
	read_xml(team_file.string(), pt);
	
	auto const all_pokemon = pt.get_child("shoddybattle");
	constexpr bool is_me = true;
	auto const team_size = bounded::check_in_range<TeamSize>(bounded::integer(all_pokemon.size()));
	auto team = Team<generation>(team_size, is_me);
	for (auto const & value : all_pokemon) {
		team.add_pokemon(load_pokemon<generation>(value.second));
	}
	team.all_pokemon().reset_index();
	return team;
}

} // namespace pl
} // namespace technicalmachine
