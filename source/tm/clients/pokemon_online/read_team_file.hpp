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

#pragma once

#include <tm/clients/pokemon_online/conversion.hpp>
#include <tm/clients/pokemon_online/invalid_team_file.hpp>

#include <tm/generation.hpp>
#include <tm/team.hpp>

#include <containers/integer_range.hpp>

#include <operators/arrow.hpp>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include <filesystem>

namespace technicalmachine {
namespace po {

struct CheckedIterator : operators::arrow<CheckedIterator> {
	explicit CheckedIterator(boost::property_tree::ptree const & pt):
		m_it(pt.begin()),
		m_last(pt.end())
	{
	}

	auto const & advance(std::string_view const expected_key) & {
		++m_it;
		if (m_it == m_last) {
			throw std::runtime_error("Incomplete PO team file\n");
		}
		auto const & value = *m_it;
		if (value.first != expected_key) {
			throw InvalidTeamFile(std::string(expected_key), value.first);
		}
		return value.second;
	}
private:
	boost::property_tree::ptree::const_iterator m_it;
	boost::property_tree::ptree::const_iterator m_last;
};

inline auto load_species(boost::property_tree::ptree const & pt) -> bounded::optional<SpeciesIDs::ID> {
	// Pokemon Online gives Missingno. the ID 0, and uses that to represent the
	// empty slots in teams smaller than 6 Pokemon.
	try {
		return pt.get<SpeciesIDs::ID>("<xmlattr>.Num");
	} catch (std::range_error const &) {
		return bounded::none;
	}
}

inline auto number_of_pokemon(boost::property_tree::ptree const & pt) -> TeamSize {
	auto pokemon_count = TeamSize(0_bi);
	for (auto const & value : pt) {
		if (value.first == "Pokemon" and load_species(value.second)) {
			if (pokemon_count == bounded::max_value<TeamSize>) {
				throw std::runtime_error("Attempted to add too many Pokemon");
			}
			++pokemon_count;
		}
	}
	return pokemon_count;
}

inline auto load_moves(Generation const generation, CheckedIterator it) {
	struct Parsed {
		RegularMoves moves;
		CheckedIterator it;
	};
	auto moves = RegularMoves();
	for (auto const n [[maybe_unused]] : containers::integer_range(4_bi)) {
		auto const & value = it.advance("Move");
		// TODO: return optional
		using ReadMoveID = bounded::integer<0, static_cast<int>(bounded::max_value<MoveID>)>;
		auto const move_id = value.get_value<ReadMoveID>();
		if (move_id != 0_bi) {
			moves.push_back(Move(generation, id_to_move(MoveID(move_id))));
		}
	}
	return Parsed{moves, it};
}

template<typename Type>
auto load_stats(std::string_view const name, CheckedIterator it) {
	struct Parsed {
		GenericStats<Type> stats;
		CheckedIterator it;
	};
	auto get_next = [&]{
		auto const & value = it.advance(name);
		return Type(value.get_value<typename Type::value_type>());
	};
	return Parsed{
		{
			get_next(),
			get_next(),
			get_next(),
			get_next(),
			get_next(),
			get_next(),
		},
		it
	};
}

template<Generation generation>
auto load_pokemon(boost::property_tree::ptree const & pt, SpeciesIDs::ID species_id) {
	auto const species = id_to_species({ species_id, pt.get<SpeciesIDs::Forme>("<xmlattr>.Forme")} );
	// auto const nickname = pt.get<std::string>("<xmlattr>.Nickname");
	auto const gender = id_to_gender(pt.get<GenderID>("<xmlattr>.Gender"));
	auto const level = Level(pt.get<Level::value_type>("<xmlattr>.Lvl"));
	auto const happiness = Happiness(pt.get<Happiness::value_type>("<xmlattr>.Happiness"));
	auto const item = id_to_item(pt.get<ItemID>("<xmlattr>.Item"));
	auto const ability = id_to_ability(pt.get<AbilityID>("<xmlattr>.Ability"));
	auto const nature = id_to_nature(pt.get<NatureID>("<xmlattr>.Nature"));

	auto const it = CheckedIterator(pt.get_child(""));
	auto const parsed_moves = load_moves(generation, it);
	auto ivs = load_stats<IV>("DV", parsed_moves.it);
	auto evs = load_stats<EV>("EV", ivs.it);

	return Pokemon<generation>(
		species,
		level,
		gender,
		item,
		ability,
		combine(nature, ivs.stats, evs.stats),
		parsed_moves.moves,
		happiness
	);
}

template<Generation generation>
auto load_team(std::filesystem::path const & team_file) {
	auto pt = boost::property_tree::ptree();
	read_xml(team_file.string(), pt);
	
	auto const all_pokemon = pt.get_child("Team");
	using GenerationInteger = bounded::integer<1, 7>;
	auto const parsed_generation = static_cast<Generation>(all_pokemon.get<GenerationInteger>("<xmlattr>.gen"));
	if (parsed_generation != generation) {
		throw std::runtime_error("Generation mismatch in team file vs. battle.");
	}
	constexpr bool is_me = true;
	auto team = Team<generation>(number_of_pokemon(all_pokemon), is_me);
	for (auto const & value : all_pokemon) {
		if (value.first != "Pokemon") {
			continue;
		}
		if (auto const species = load_species(value.second)) {
			team.add_pokemon(load_pokemon<generation>(value.second, *species));
		}
	}
	team.all_pokemon().reset_index();
	return team;
}

} // namespace po
} // namespace technicalmachine
