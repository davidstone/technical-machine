// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <tm/clients/pokemon_online/read_team_file.hpp>

#include <tm/clients/pokemon_online/conversion.hpp>
#include <tm/clients/pokemon_online/invalid_team_file.hpp>

#include <tm/stat/generic_stats.hpp>
#include <tm/stat/ingame_id_to_nature.hpp>
#include <tm/stat/ev.hpp>
#include <tm/stat/iv.hpp>

#include <tm/constant_generation.hpp>
#include <tm/generation.hpp>
#include <tm/team.hpp>

#include <containers/integer_range.hpp>

#include <numeric_traits/min_max_value.hpp>

#include <operators/arrow.hpp>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include <filesystem>

namespace technicalmachine::po {
namespace {
using namespace std::string_view_literals;

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
			throw InvalidTeamFile(expected_key, value.first);
		}
		return value.second;
	}
private:
	boost::property_tree::ptree::const_iterator m_it;
	boost::property_tree::ptree::const_iterator m_last;
};

auto parse_species(boost::property_tree::ptree const & pt) -> bounded::optional<SpeciesIDs::ID> {
	// Pokemon Online gives Missingno. the ID 0, and uses that to represent the
	// empty slots in teams smaller than 6 Pokemon.
	try {
		return pt.get<SpeciesIDs::ID>("<xmlattr>.Num");
	} catch (std::range_error const &) {
		return bounded::none;
	}
}

auto number_of_pokemon(boost::property_tree::ptree const & pt) -> TeamSize {
	auto pokemon_count = TeamSize(0_bi);
	for (auto const & value : pt) {
		if (value.first == "Pokemon" and parse_species(value.second)) {
			if (pokemon_count == numeric_traits::max_value<TeamSize>) {
				throw std::runtime_error("Attempted to add too many Pokemon");
			}
			++pokemon_count;
		}
	}
	return pokemon_count;
}

auto parse_moves(Generation const generation, CheckedIterator it) {
	struct Parsed {
		RegularMoves moves;
		CheckedIterator it;
	};
	auto moves = RegularMoves();
	for (auto const n [[maybe_unused]] : containers::integer_range(4_bi)) {
		auto const & value = it.advance("Move");
		// TODO: return optional
		using ReadMoveID = bounded::integer<0, static_cast<int>(numeric_traits::max_value<MoveID>)>;
		auto const move_id = value.get_value<ReadMoveID>();
		if (move_id != 0_bi) {
			moves.push_back(Move(generation, id_to_move(MoveID(move_id))));
		}
	}
	return Parsed{moves, it};
}

template<typename Type>
auto parse_stats(std::string_view const name, CheckedIterator it) {
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
auto parse_dvs_or_ivs(CheckedIterator it) {
	constexpr auto name = std::string_view("DV");
	if constexpr (generation <= Generation::two) {
		struct Parsed {
			DVs stats;
			CheckedIterator it;
		};
		auto get_next = [&]{
			auto const & value = it.advance(name);
			return DV(value.get_value<DV::value_type>());
		};
		auto const hp = get_next();
		auto const atk = get_next();
		auto const def = get_next();
		auto const spc = get_next();
		get_next();
		auto const spe = get_next();
		auto const dvs = DVs(atk, def, spe, spc);
		if (dvs.hp() != hp) {
			throw std::runtime_error("Stored HP DV does not match calculated HP DV.");
		}
		return Parsed{dvs, it};
	} else {
		return parse_stats<IV>(name, it);
	}
}

template<Generation generation>
auto parse_evs(CheckedIterator it) {
	constexpr auto name = std::string_view("EV");
	if constexpr (generation <= Generation::two) {
		struct Parsed {
			OldGenEVs stats;
			CheckedIterator it;
		};
		auto get_next = [&]{
			auto const & value = it.advance(name);
			return EV(value.get_value<EV::value_type>());
		};
		auto const hp = get_next();
		auto const atk = get_next();
		auto const def = get_next();
		auto const spc = get_next();
		get_next();
		auto const spe = get_next();
		auto const evs = OldGenEVs(hp, atk, def, spe, spc);
		return Parsed{evs, it};
	} else {
		return parse_stats<EV>(name, it);
	}
}

template<Generation generation>
auto parse_pokemon(boost::property_tree::ptree const & pt, SpeciesIDs::ID species_id) {
	auto const species = id_to_species({ species_id, pt.get<SpeciesIDs::Forme>("<xmlattr>.Forme")} );
	// auto const nickname = pt.get<std::string>("<xmlattr>.Nickname");
	auto const gender = id_to_gender(pt.get<GenderID>("<xmlattr>.Gender"));
	auto const level = Level(pt.get<Level::value_type>("<xmlattr>.Lvl"));
	auto const happiness = Happiness(pt.get<Happiness::value_type>("<xmlattr>.Happiness"));
	auto const item = id_to_item(pt.get<ItemID>("<xmlattr>.Item"));
	auto const ability = id_to_ability(pt.get<AbilityID>("<xmlattr>.Ability"));
	auto const nature = ingame_id_to_nature(pt.get<IngameNatureID>("<xmlattr>.Nature"));

	auto const it = CheckedIterator(pt.get_child(""));
	auto const parsed_moves = parse_moves(generation, it);

	auto const dvs_or_ivs = parse_dvs_or_ivs<generation>(parsed_moves.it);
	auto const evs = parse_evs<generation>(dvs_or_ivs.it);

	return Pokemon<generation>(
		species,
		level,
		gender,
		item,
		ability,
		CombinedStats<generation>{nature, dvs_or_ivs.stats, evs.stats},
		parsed_moves.moves,
		happiness
	);
}

template<Generation generation>
auto parse_team(boost::property_tree::ptree const & pt) {
	constexpr bool is_me = true;
	auto team = Team<generation>(number_of_pokemon(pt), is_me);
	for (auto const & value : pt) {
		if (value.first != "Pokemon") {
			continue;
		}
		if (auto const species = parse_species(value.second)) {
			team.add_pokemon(parse_pokemon<generation>(value.second, *species));
		}
	}
	team.all_pokemon().set_index(0_bi);
	return team;
}

} // namespace

auto read_team_file(std::filesystem::path const & team_file) -> GenerationGeneric<Team> {
	try {
		auto pt = boost::property_tree::ptree();
		read_xml(team_file.string(), pt);
		
		auto const all_pokemon = pt.get_child("Team");
		using GenerationInteger = bounded::integer<1, 7>;
		auto const parsed_generation = static_cast<Generation>(all_pokemon.get<GenerationInteger>("<xmlattr>.gen"));
		return constant_generation(parsed_generation, [&]<Generation generation>(constant_gen_t<generation>) {
			return GenerationGeneric<Team>(parse_team<generation>(all_pokemon));
		});
	} catch (std::exception const & ex) {
		throw std::runtime_error(containers::concatenate<std::string>("Failed to parse Pokemon Online team file \""sv, team_file.string(), "\" -- "sv, std::string_view(ex.what())));
	}
}

} // namespace technicalmachine::po
