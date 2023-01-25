// Write Pokemon Lab teams
// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.clients.pl.write_team_file;

import tm.move.move;

import tm.pokemon.any_pokemon;
import tm.pokemon.species;

import tm.stat.calculate_ivs_and_evs;
import tm.stat.ev;
import tm.stat.stat_names;

import tm.string_conversions.ability;
import tm.string_conversions.gender;
import tm.string_conversions.item;
import tm.string_conversions.move_name;
import tm.string_conversions.nature;
import tm.string_conversions.species;

import tm.any_team;
import tm.gender;
import tm.generation;
import tm.property_tree;

import bounded;
import std_module;

namespace technicalmachine::pl {
using namespace std::string_view_literals;

constexpr auto to_simulator_string(SplitSpecialPermanentStat const stat) -> std::string_view {
	switch (stat) {
		case SplitSpecialPermanentStat::hp: return "HP";
		case SplitSpecialPermanentStat::atk: return "Atk";
		case SplitSpecialPermanentStat::def: return "Def";
		case SplitSpecialPermanentStat::spa: return "SpAtk";
		case SplitSpecialPermanentStat::spd: return "SpDef";
		case SplitSpecialPermanentStat::spe: return "Spd";
	}
}

constexpr auto to_simulator_string(Species const species) -> std::string_view {
	switch (species) {
		case Species::Deoxys_Normal: return "Deoxys";
		case Species::Deoxys_Attack: return "Deoxys-f";
		case Species::Deoxys_Defense: return "Deoxys-l";
		case Species::Deoxys_Speed: return "Deoxys-e";
		case Species::Giratina_Altered: return "Giratina";
		case Species::Giratina_Origin: return "Giratina-o";
		case Species::Rotom_Mow: return "Rotom-c";
		case Species::Rotom_Frost: return "Rotom-f";
		case Species::Rotom_Heat: return "Rotom-h";
		case Species::Rotom_Fan: return "Rotom-s";
		case Species::Rotom_Wash: return "Rotom-w";
		case Species::Shaymin_Land: return "Shaymin";
		case Species::Shaymin_Sky: return "Shaymin-s";
		case Species::Wormadam_Plant: return "Wormadam";
		case Species::Wormadam_Sandy: return "Wormadam-g";
		case Species::Wormadam_Trash: return "Wormadam-s";
		default: return to_string(species);
	}
}

constexpr auto to_simulator_string(Gender const gender) -> std::string_view {
	switch (gender) {
		case Gender::female: return "Female";
		case Gender::genderless: return "No Gender";
		case Gender::male: return "Male";
	}
}

auto write_move(Move const move, property_tree::ptree_writer pt) -> void {
	property_tree::ptree_writer m = pt.add("moveset.move", to_string(move.name()));
	// TODO: replace with real PP ups logic
	m.put("<xmlattr>.pp-up", 3);
}

auto write_stat(std::string_view const name, auto const dv_or_iv, EV const ev, property_tree::ptree_writer pt) -> void {
	property_tree::ptree_writer s = pt.add("stats.stat", "");
	s.put("<xmlattr>.name", name);
	s.put("<xmlattr>.iv", dv_or_iv.value());
	s.put("<xmlattr>.ev", ev.value());
}

auto write_stats(any_pokemon auto const & pokemon, property_tree::ptree_writer pt) -> void {
	auto const stats = calculate_ivs_and_evs(pokemon);
	for (auto const stat_name : {SplitSpecialPermanentStat::hp, SplitSpecialPermanentStat::atk, SplitSpecialPermanentStat::def, SplitSpecialPermanentStat::spe, SplitSpecialPermanentStat::spa, SplitSpecialPermanentStat::spd}) {
		write_stat(to_simulator_string(stat_name), stats.dvs_or_ivs[stat_name], stats.evs[stat_name], pt);
	}
}

auto write_pokemon(any_pokemon auto const & pokemon, property_tree::ptree_writer pt) -> void {
	property_tree::ptree_writer member = pt.add("pokemon", "");
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

export template<any_team TeamType>
auto write_team(TeamType const & team, std::filesystem::path const & file_name) -> void {
	auto owner = property_tree::ptree();
	auto pt = property_tree::ptree_writer(owner);
	property_tree::ptree_writer t = pt.add("shoddybattle", "");
	// The original format did not include a generation.
	t.put("<xmlattr>.generation", static_cast<int>(generation_from<TeamType>));
	for (auto const & pokemon : team.all_pokemon()) {
		write_pokemon(pokemon, t);
	}
	pt.write_xml(file_name);
}

} // namespace technicalmachine::pl
