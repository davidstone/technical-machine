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

#include <tm/ability.hpp>
#include <tm/item.hpp>
#include <tm/team.hpp>

#include <tm/move/move.hpp>

#include <tm/pokemon/pokemon.hpp>
#include <tm/pokemon/species_forward.hpp>

#include <containers/integer_range.hpp>

#include <operators/arrow.hpp>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include <string>

namespace technicalmachine {
enum class Moves : std::uint16_t;
namespace po {
namespace {
using boost::property_tree::ptree;

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

auto load_moves(Generation const generation, CheckedIterator it) {
	struct Parsed {
		RegularMoves moves;
		CheckedIterator it;
	};
	auto moves = RegularMoves();
	for (auto const n [[maybe_unused]] : containers::integer_range(4_bi)) {
		auto const & value = it.advance("Move");
		// TODO: return optional
		using ReadMoveID = bounded::checked_integer<0, static_cast<int>(bounded::max_value<MoveID>)>;
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
		constexpr auto operator[](PermanentStat const stat_name) const {
			return index_stat(*this, stat_name);
		}

		Type hp;
		Type atk;
		Type def;
		Type spa;
		Type spd;
		Type spe;

		CheckedIterator it;
	};
	auto get_next = [&]{
		auto const & value = it.advance(name);
		return Type(value.get_value<typename Type::value_type>());
	};
	return Parsed{
		get_next(),
		get_next(),
		get_next(),
		get_next(),
		get_next(),
		get_next(),
		it
	};
}

auto load_pokemon(ptree const & pt, Generation const generation, SpeciesIDs::ID) -> Pokemon {
	auto const species = id_to_species({ pt.get<SpeciesIDs::ID>("<xmlattr>.Num"), pt.get<SpeciesIDs::Forme>("<xmlattr>.Forme")} );
	// auto const nickname = pt.get<std::string>("<xmlattr>.Nickname");
	auto const gender = Gender(id_to_gender(pt.get<GenderID>("<xmlattr>.Gender")));
	auto const level = Level(pt.get<Level::value_type>("<xmlattr>.Lvl"));
	auto const happiness = Happiness(pt.get<Happiness::value_type>("<xmlattr>.Happiness"));
	auto const item = id_to_item(pt.get<ItemID>("<xmlattr>.Item"));
	auto const ability = id_to_ability(pt.get<AbilityID>("<xmlattr>.Ability"));
	auto const nature = id_to_nature(pt.get<NatureID>("<xmlattr>.Nature"));

	auto const it = CheckedIterator(pt.get_child(""));
	auto const parsed_moves = load_moves(generation, it);
	auto ivs = load_stats<IV>("DV", parsed_moves.it);
	auto evs = load_stats<EV>("EV", ivs.it);

	auto pokemon = Pokemon(
		generation,
		species,
		level,
		gender,
		item,
		ability,
		nature,
		parsed_moves.moves,
		happiness
	);
	for (auto const stat_name : containers::enum_range<PermanentStat>()) {
		auto const iv = ivs[stat_name];
		auto const ev = evs[stat_name];
		pokemon.set_ev(generation, stat_name, iv, ev);
	}

	return pokemon;
}

} // namespace

Team load_team(Generation const expected_generation, std::filesystem::path const & team_file) {
	ptree pt;
	read_xml(team_file.string(), pt);
	
	auto const all_pokemon = pt.get_child("Team");
	using GenerationInteger = bounded::checked_integer<1, 7, InvalidTeamFile>;
	auto const generation = static_cast<Generation>(all_pokemon.get<GenerationInteger>("<xmlattr>.gen"));
	if (generation != expected_generation) {
		throw std::runtime_error("Generation mismatch in team file vs. battle.");
	}
	constexpr bool is_me = true;
	auto team = Team(number_of_pokemon(all_pokemon), is_me);
	for (auto const & value : all_pokemon) {
		if (value.first != "Pokemon") {
			continue;
		}
		if (auto const species = load_species(value.second)) {
			team.add_pokemon(load_pokemon(value.second, generation, *species));
		}
	}
	team.all_pokemon().reset_index();
	return team;
}

}	// namespace po
}	// namespace technicalmachine
