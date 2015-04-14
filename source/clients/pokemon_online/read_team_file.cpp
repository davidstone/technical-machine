// Load Pokemon Online teams
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

#include "read_team_file.hpp"

#include <string>

#include "conversion.hpp"
#include "invalid_team_file.hpp"
#include "stat_order.hpp"

#include "../../ability.hpp"
#include "../../item.hpp"
#include "../../team.hpp"

#include "../../move/move.hpp"
#include "../../move/moves.hpp"

#include "../../pokemon/pokemon.hpp"
#include "../../pokemon/species_forward.hpp"

#include "../../stat/stat.hpp"

#include <bounded_integer/integer_range.hpp>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

namespace technicalmachine {
namespace po {
namespace {
using boost::property_tree::ptree;

Moves load_move(ptree const & pt) {
	return id_to_move(pt.get_value<unsigned>());
}

bool is_real_pokemon(ptree const & pt) {
	// Pokemon Online gives Missingno. the ID 0, and uses that to represent the
	// empty slots in teams smaller than 6 Pokemon.
	return pt.get<unsigned>("<xmlattr>.Num") != 0;
}

TeamSize number_of_pokemon(ptree const & pt) {
	TeamSize pokemon_count = 0_bi;
	for (auto const & value : pt) {
		if (value.first == "Pokemon" and is_real_pokemon(value.second))
			++pokemon_count;
	}
	return pokemon_count;
}

ptree::const_iterator load_moves(Pokemon & pokemon, ptree::const_iterator it) {
	for (auto const n : bounded::integer_range(4_bi)) {
		static_cast<void>(n);
		if (it->first != "Move") {
			throw InvalidTeamFile("Move", it->first);
		}
		Moves const move(load_move(it->second));
		if (move != Moves::END) {
			all_moves(pokemon).add(move);
		}
		++it;
	}
	return it;
}

template<typename Field>
class StatType;

template<>
class StatType<EV> {
public:
	static std::string const & name() {
		static std::string const str = "EV";
		return str;
	}
	template<typename Type>
	static EV & get(Type & stat) {
		return stat.ev;
	}
};
template<>
class StatType<IV> {
public:
	static std::string const & name() {
		static std::string const str = "DV";
		return str;
	}
	template<typename Type>
	static IV & get(Type & stat) {
		return stat.iv;
	}
};

template<typename Type>
ptree::const_iterator load_stats(Pokemon & pokemon, ptree::const_iterator it) {
	std::string const & name = StatType<Type>::name();
	if (it->first != name) {
		throw InvalidTeamFile(name, it->first);
	}
	HP & hp = get_hp(pokemon);
	StatType<Type>::get(hp) = Type(it->second.get_value<typename Type::value_type>());
	++it;
	for (auto const & stat_name : stat_order) {
		if (it->first != name) {
			throw InvalidTeamFile(name, it->first);
		}
		Stat & stat = get_stat(pokemon, stat_name);
		StatType<Type>::get(stat) = Type(it->second.get_value<typename Type::value_type>());
		++it;
	}
	return it;
}

void load_pokemon(ptree const & pt, Team & team) {
	auto const id = pt.get<unsigned>("<xmlattr>.Num");
	auto const forme = pt.get<unsigned>("<xmlattr>.Forme");
	auto const species = id_to_species(id, forme);
	auto const nickname = pt.get<std::string>("<xmlattr>.Nickname");
	Gender const gender(id_to_gender(pt.get<unsigned>("<xmlattr>.Gender")));
	Level const level(pt.get<bounded::checked_integer<Level::min, Level::max>>("<xmlattr>.Lvl"));
	Happiness const happiness(pt.get<Happiness::value_type>("<xmlattr>.Happiness"));
	team.add_pokemon(species, level, gender, nickname, happiness);
	Pokemon & pokemon = team.replacement();

	get_item(pokemon) = id_to_item(pt.get<unsigned>("<xmlattr>.Item"));
	get_ability(pokemon) = id_to_ability(pt.get<unsigned>("<xmlattr>.Ability"));
	get_nature(pokemon) = id_to_nature(pt.get<unsigned>("<xmlattr>.Nature"));

	// Get past the xml attributes
	auto it = ++pt.get_child("").begin();
	it = load_moves(pokemon, it);
	it = load_stats<IV>(pokemon, it);
	load_stats<EV>(pokemon, it);
}

}	// anonymous namespace

void load_team(Team & team, boost::filesystem::path const & team_file) {
	ptree pt;
	read_xml(team_file.string(), pt);
	
	auto const all_pokemon = pt.get_child("Team");
	team.all_pokemon().initialize_size(number_of_pokemon(all_pokemon));
	for (auto const & value : all_pokemon) {
		if (value.first == "Pokemon" and is_real_pokemon(value.second))
			load_pokemon(value.second, team);
	}
	team.all_pokemon().reset_index();
}

}	// namespace po
}	// namespace technicalmachine
