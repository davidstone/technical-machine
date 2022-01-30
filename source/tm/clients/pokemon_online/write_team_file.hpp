// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <tm/clients/pokemon_online/conversion.hpp>

#include <tm/pokemon/any_pokemon.hpp>

#include <tm/stat/calculate_ivs_and_evs.hpp>
#include <tm/stat/ev.hpp>
#include <tm/stat/ingame_id_to_nature.hpp>

#include <tm/string_conversions/species.hpp>

#include <tm/any_team.hpp>

#include <containers/integer_range.hpp>
#include <containers/size.hpp>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include <filesystem>

namespace technicalmachine {

namespace po {

inline void write_blank_stats(Generation const generation, boost::property_tree::ptree & pt) {
	constexpr auto stat_names = containers::enum_range<SplitSpecialPermanentStat>();
	for ([[maybe_unused]] auto const stat : stat_names) {
		auto const dv = generation <= Generation::two ? 15 : 31;
		pt.add("DV", dv);
	}
	for ([[maybe_unused]] auto const stat : stat_names) {
		pt.add("EV", 0);
	}
}

void write_stats(any_pokemon auto const & pokemon, boost::property_tree::ptree & pt) {
	auto const stats = calculate_ivs_and_evs(pokemon);
	constexpr auto stat_names = containers::enum_range<SplitSpecialPermanentStat>();
	// In older generations, Spc is written into the SpA slot. The SpD slot is
	// written as a 15 but unused in the official client, but here we instead
	// just write Spc into both.
	for (auto const stat : stat_names) {
		pt.add("DV", stats.dvs_or_ivs[stat].value());
	}
	for (auto const stat : stat_names) {
		pt.add("EV", stats.evs[stat].value());
	}
}

inline void write_blank_move(boost::property_tree::ptree & pt) {
	pt.add("Move", 0);
}

inline void write_move(Move const move, boost::property_tree::ptree & pt) {
	pt.add("Move", move_to_id(move.name()));
}

inline auto write_blank_pokemon(Generation const generation, boost::property_tree::ptree & pt) -> void {
	auto & member = pt.add("Pokemon", "");
	member.put("<xmlattr>.Item", 0);
	member.put("<xmlattr>.Ability", 0);
	member.put("<xmlattr>.Num", 0);
	member.put("<xmlattr>.Nature", 0);
	member.put("<xmlattr>.Shiny", 0);
	member.put("<xmlattr>.Nickname", "Missingno");
	member.put("<xmlattr>.Gen", 4);
	member.put("<xmlattr>.Forme", 0);
	member.put("<xmlattr>.Happiness", 0);
	member.put("<xmlattr>.Lvl", 100);
	member.put("<xmlattr>.Gender", 0);

	for (auto const n [[maybe_unused]] : containers::integer_range(max_moves_per_pokemon)) {
		write_blank_move(member);
	}

	write_blank_stats(generation, member);
}

void write_pokemon(any_pokemon auto const & pokemon, boost::property_tree::ptree & pt) {
	auto & member = pt.add("Pokemon", "");
	member.put("<xmlattr>.Item", item_to_id(pokemon.item(false, false)));
	member.put("<xmlattr>.Ability", ability_to_id (pokemon.initial_ability()));
	auto const species = pokemon.species();
	auto const po_species = species_to_id(species);
	member.put("<xmlattr>.Num", po_species.id);
	member.put("<xmlattr>.Nature", ingame_nature_to_id(pokemon.nature()));
	member.put("<xmlattr>.Shiny", 0);
	member.put("<xmlattr>.Nickname", pokemon.nickname());
	member.put("<xmlattr>.Gen", 4);
	member.put("<xmlattr>.Forme", po_species.forme);
	member.put("<xmlattr>.Happiness", pokemon.happiness());
	member.put("<xmlattr>.Lvl", pokemon.level()());
	member.put("<xmlattr>.Gender", gender_to_id (pokemon.gender()));

	auto const & moves = pokemon.regular_moves();
	for (auto const & move : moves) {
		write_move(move, member);
	}
	for (auto const n [[maybe_unused]] : containers::integer_range(containers::size(moves), max_moves_per_pokemon)) {
		write_blank_move(member);
	}

	write_stats(pokemon, member);
}

template<any_team TeamType>
void write_team(TeamType const & team, std::filesystem::path const & file_name) {
	constexpr auto generation = generation_from<TeamType>;
	auto pt = boost::property_tree::ptree();
	auto settings = boost::property_tree::xml_writer_settings<boost::property_tree::ptree::key_type>('\t', 1);
	auto & t = pt.add("Team", "");
	t.put("<xmlattr>.version", 1);
	t.put("<xmlattr>.gen", static_cast<int>(generation));
	t.put("<xmlattr>.defaultTier", "");
	auto & trainer = t.add("Trainer", "TM");
	trainer.put("<xmlattr>.loseMsg", "");
	trainer.put("<xmlattr>.avatar", 1);
	trainer.put("<xmlattr>.winMsg", "");
	trainer.put("<xmlattr>.infoMsg", "");
	for (auto const & pokemon : team.all_pokemon()) {
		write_pokemon(pokemon, t);
	}
	for (auto const unused [[maybe_unused]] : containers::integer_range(containers::size(team.all_pokemon()), max_pokemon_per_team)) {
		write_blank_pokemon(generation, t);
	}
	write_xml(file_name.string(), pt, std::locale(), settings);
}

} // namespace po
} // namespace technicalmachine
