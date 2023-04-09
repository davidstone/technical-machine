// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

module;

#include <compare>
#include <string>

export module tm.clients.po.read_team_file;

import tm.clients.po.conversion;
import tm.clients.po.invalid_team_file;

import tm.move.move;
import tm.move.regular_moves;

import tm.pokemon.happiness;
import tm.pokemon.known_pokemon;
import tm.pokemon.level;
import tm.pokemon.max_pokemon_per_team;
import tm.pokemon.nickname;

import tm.stat.combined_stats;
import tm.stat.generic_stats;
import tm.stat.ingame_id_to_nature;
import tm.stat.ev;
import tm.stat.iv;

import tm.constant_generation;
import tm.generation;
import tm.property_tree;
export import tm.team;

import bounded;
import containers;
import numeric_traits;
import operators;
import tv;
import std_module;

namespace technicalmachine::po {
using namespace bounded::literal;
using namespace std::string_view_literals;

struct CheckedIterator : operators::arrow<CheckedIterator> {
	explicit CheckedIterator(property_tree::ptree_reader pt):
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
	property_tree::ptree_reader::const_iterator m_it;
	property_tree::ptree_reader::const_iterator m_last;
};

auto parse_species(property_tree::ptree_reader pt) -> tv::optional<SpeciesIDs::ID> {
	// Pokemon Online gives Missingno. the ID 0, and uses that to represent the
	// empty slots in teams smaller than 6 Pokemon.
	try {
		return pt.get<SpeciesIDs::ID>("<xmlattr>.Num");
	} catch (std::range_error const &) {
		return tv::none;
	}
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
		using ReadMoveID = bounded::integer<0, bounded::normalize<numeric_traits::max_value<MoveID>>>;
		auto const move_id = value.get_value<ReadMoveID>();
		if (move_id != 0_bi) {
			moves.push_back(Move(generation, id_to_move(bounded::assume_in_range<MoveID>(move_id))));
		}
	}
	return Parsed{moves, it};
}

template<typename T>
auto parse_stats(std::string_view const name, CheckedIterator it) {
	struct Parsed {
		GenericStats<T> stats;
		CheckedIterator it;
	};
	auto get_next = [&] {
		auto const & value = it.advance(name);
		return T(value.get_value<typename T::value_type>());
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
		auto get_next = [&] {
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
		auto get_next = [&] {
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
auto parse_pokemon(property_tree::ptree_reader pt, SpeciesIDs::ID species_id) {
	auto const species = id_to_species({species_id, pt.get<SpeciesIDs::Forme>("<xmlattr>.Forme")});
	auto const nickname = Nickname(pt.get<std::string>("<xmlattr>.Nickname"));
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

	return KnownPokemon<generation>(
		species,
		nickname,
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
auto parse_team(property_tree::ptree_reader pt) {
	auto all_pokemon = containers::static_vector<KnownPokemon<generation>, max_pokemon_per_team>();
	for (auto const & value : pt) {
		if (value.first != "Pokemon") {
			continue;
		}
		auto const species = parse_species(value.second);
		if (!species) {
			continue;
		}
		containers::push_back(all_pokemon, parse_pokemon<generation>(value.second, *species));
	}
	return KnownTeam<generation>(std::move(all_pokemon));
}

export auto read_team_file(std::filesystem::path const & team_file) -> GenerationGeneric<KnownTeam> {
	try {
		auto owner = property_tree::ptree();
		auto pt = owner.read_xml(team_file);

		auto const all_pokemon = pt.get_child("Team");
		using GenerationInteger = bounded::integer<1, 7>;
		auto const parsed_generation = static_cast<Generation>(all_pokemon.get<GenerationInteger>("<xmlattr>.gen"));
		return constant_generation(parsed_generation, [&]<Generation generation>(constant_gen_t<generation>) {
			return GenerationGeneric<KnownTeam>(parse_team<generation>(all_pokemon));
		});
	} catch (std::exception const & ex) {
		throw std::runtime_error(containers::concatenate<std::string>("Failed to parse Pokemon Online team file \""sv, team_file.string(), "\" -- "sv, std::string_view(ex.what())));
	}
}

} // namespace technicalmachine::po
