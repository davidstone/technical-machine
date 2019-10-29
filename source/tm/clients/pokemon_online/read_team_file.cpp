// Load Pokemon Online teams
// Copyright (C) 2018 David Stone
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

#include <tm/clients/pokemon_online/read_team_file.hpp>

#include <tm/clients/pokemon_online/conversion.hpp>
#include <tm/clients/pokemon_online/invalid_team_file.hpp>
#include <tm/clients/pokemon_online/stat_order.hpp>

#include <tm/ability.hpp>
#include <tm/item.hpp>
#include <tm/team.hpp>

#include <tm/move/move.hpp>

#include <tm/pokemon/pokemon.hpp>
#include <tm/pokemon/species_forward.hpp>

#include <tm/stat/stat.hpp>

#include <containers/integer_range.hpp>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include <string>

namespace technicalmachine {
enum class Moves : std::uint16_t;
namespace po {
namespace {
using boost::property_tree::ptree;

// TODO
constexpr auto generation = Generation::four;

auto load_species(ptree const & pt) -> bounded::optional<SpeciesIDs::ID> {
	// Pokemon Online gives Missingno. the ID 0, and uses that to represent the
	// empty slots in teams smaller than 6 Pokemon.
	try {
		return pt.get<SpeciesIDs::ID>("<xmlattr>.Num");
	} catch (std::range_error const &) {
		return bounded::none;
	}
}

TeamSize number_of_pokemon(ptree const & pt) {
	TeamSize pokemon_count = 0_bi;
	for (auto const & value : pt) {
		if (value.first == "Pokemon" and load_species(value.second))
			++pokemon_count;
	}
	return pokemon_count;
}

ptree::const_iterator load_moves(Pokemon & pokemon, ptree::const_iterator it) {
	for (auto const n [[maybe_unused]] : containers::integer_range(4_bi)) {
		if (it->first != "Move") {
			throw InvalidTeamFile("Move", it->first);
		}
		// TODO: return optional
		using ReadMoveID = bounded::checked_integer<0, static_cast<int>(bounded::max_value<MoveID>)>;
		auto const move_id = it->second.get_value<ReadMoveID>();
		if (move_id != 0_bi) {
			// TODO: Throw an exception if we attempt to add the same move twice
			add_seen_move(all_moves(pokemon), generation, id_to_move(MoveID(move_id)));
		}
		++it;
	}
	return it;
}

template<typename Field>
struct StatType;

template<>
struct StatType<EV> {
	static auto const & name() {
		static std::string const str = "EV";
		return str;
	}
	static auto get(auto & stat) {
		return stat.ev();
	}
};
template<>
struct StatType<IV> {
	static auto const & name() {
		static std::string const str = "DV";
		return str;
	}
	static auto get(auto & stat) {
		return stat.iv();
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

void load_pokemon(ptree const & pt, Team & team, SpeciesIDs::ID) {
	auto const species = id_to_species({ pt.get<SpeciesIDs::ID>("<xmlattr>.Num"), pt.get<SpeciesIDs::Forme>("<xmlattr>.Forme")} );
	// auto const nickname = pt.get<std::string>("<xmlattr>.Nickname");
	auto const gender = Gender(id_to_gender(pt.get<GenderID>("<xmlattr>.Gender")));
	auto const level = Level(pt.get<bounded::checked_integer<Level::min, Level::max>>("<xmlattr>.Lvl"));
	auto const happiness = Happiness(pt.get<Happiness::value_type>("<xmlattr>.Happiness"));
	auto const item = id_to_item(pt.get<ItemID>("<xmlattr>.Item"));
	auto const ability = id_to_ability(pt.get<AbilityID>("<xmlattr>.Ability"));
	auto const nature = id_to_nature(pt.get<NatureID>("<xmlattr>.Nature"));

	auto & pokemon = team.add_pokemon(generation, species, level, gender, item, ability, nature, happiness);

	// Get past the xml attributes
	auto it = ++pt.get_child("").begin();
	it = load_moves(pokemon, it);
	it = load_stats<IV>(pokemon, it);
	load_stats<EV>(pokemon, it);
}

}	// anonymous namespace

Team load_team(std::filesystem::path const & team_file) {
	ptree pt;
	read_xml(team_file.string(), pt);
	
	auto const all_pokemon = pt.get_child("Team");
	constexpr bool is_me = true;
	auto team = Team(number_of_pokemon(all_pokemon), is_me);
	for (auto const & value : all_pokemon) {
		if (value.first == "Pokemon") {
			if (auto const species = load_species(value.second)) {
				load_pokemon(value.second, team, *species);
			}
		}
	}
	team.all_pokemon().reset_index();
	return team;
}

}	// namespace po
}	// namespace technicalmachine
