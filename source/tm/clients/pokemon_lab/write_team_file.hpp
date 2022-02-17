// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <tm/pokemon/any_pokemon.hpp>
#include <tm/pokemon/species_forward.hpp>

#include <tm/stat/calculate_ivs_and_evs.hpp>
#include <tm/stat/ev.hpp>
#include <tm/stat/iv.hpp>

#include <tm/string_conversions/ability.hpp>
#include <tm/string_conversions/gender.hpp>
#include <tm/string_conversions/item.hpp>
#include <tm/string_conversions/nature.hpp>
#include <tm/string_conversions/species.hpp>

#include <tm/any_team.hpp>
#include <tm/generation.hpp>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include <filesystem>

namespace technicalmachine {

namespace pl {

auto write_move(Move, boost::property_tree::ptree &) -> void;
auto to_simulator_string(SplitSpecialPermanentStat) -> std::string_view;
auto to_simulator_string(Species) -> std::string_view;
auto to_simulator_string(Gender) -> std::string_view;

auto write_stat(std::string_view const name, auto const dv_or_iv, EV const ev, boost::property_tree::ptree & pt) -> void {
	boost::property_tree::ptree & s = pt.add("stats.stat", "");
	s.put("<xmlattr>.name", name);
	s.put("<xmlattr>.iv", dv_or_iv.value());
	s.put("<xmlattr>.ev", ev.value());
}

void write_stats(any_pokemon auto const & pokemon, boost::property_tree::ptree & pt) {
	auto const stats = calculate_ivs_and_evs(pokemon);
	for (auto const stat_name : {SplitSpecialPermanentStat::hp, SplitSpecialPermanentStat::atk, SplitSpecialPermanentStat::def, SplitSpecialPermanentStat::spe, SplitSpecialPermanentStat::spa, SplitSpecialPermanentStat::spd}) {
		write_stat(to_simulator_string(stat_name), stats.dvs_or_ivs[stat_name], stats.evs[stat_name], pt);
	}
}

void write_pokemon(any_pokemon auto const & pokemon, boost::property_tree::ptree & pt) {
	auto & member = pt.add("pokemon", "");
	member.put("<xmlattr>.species", to_simulator_string(pokemon.species()));
	auto get_nickname = [&]() -> std::string_view {
		if constexpr (requires { pokemon.nickname(); }) {
			return pokemon.nickname();
		} else {
			return ""sv;
		}
	};
	member.put("nickname", get_nickname());
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

template<any_team TeamType>
void write_team(TeamType const & team, std::filesystem::path const & file_name) {
	auto pt = boost::property_tree::ptree();
	auto settings = boost::property_tree::xml_writer_settings<boost::property_tree::ptree::key_type>('\t', 1);
	auto & t = pt.add("shoddybattle", "");
	// The original format did not include a generation.
	t.put("<xmlattr>.generation", static_cast<int>(generation_from<TeamType>));
	for (auto const & pokemon : team.all_pokemon()) {
		write_pokemon(pokemon, t);
	}
	write_xml(file_name.string(), pt, std::locale(), settings);
}

} // namespace pl
} // namespace technicalmachine
