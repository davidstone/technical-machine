// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

module;

#include <std_module/prelude.hpp>
#include <string>

export module tm.clients.po.read_team_file;

import tm.clients.po.conversion;
import tm.clients.po.invalid_team_file;

import tm.move.initial_move;
import tm.move.max_moves_per_pokemon;

import tm.pokemon.happiness;
import tm.pokemon.initial_pokemon;
import tm.pokemon.level;
import tm.pokemon.nickname;

import tm.stat.combined_stats;
import tm.stat.ev;
import tm.stat.evs;
import tm.stat.generic_stats;
import tm.stat.ingame_id_to_nature;
import tm.stat.iv;
import tm.stat.stat_style;

import tm.generation;
import tm.initial_team;
import tm.property_tree;

import bounded;
import containers;
import numeric_traits;
import std_module;
import tv;

namespace technicalmachine::po {
using namespace bounded::literal;
using namespace std::string_view_literals;

struct CheckedIterator {
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

auto parse_moves(CheckedIterator it) {
	struct Parsed {
		InitialMoves moves;
		CheckedIterator it;
	};
	auto moves = InitialMoves(containers::remove_none(
		containers::generate_n(max_moves_per_pokemon, [&] -> tv::optional<InitialMove> {
			auto const & value = it.advance("Move");
			static_assert(numeric_traits::min_value<MoveID> == 1_bi);
			using ReadMoveID = bounded::integer<0, bounded::normalize<numeric_traits::max_value<MoveID>>>;
			auto const move_id = value.get_value<ReadMoveID>();
			if (move_id == 0_bi) {
				return tv::none;
			}
			return InitialMove(
				id_to_move(bounded::assume_in_range<MoveID>(move_id))
			);
		})
	));
	return Parsed(moves, it);
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

template<SpecialStyle style>
auto parse_dvs_or_ivs(CheckedIterator it) {
	constexpr auto name = "DV"sv;
	if constexpr (style == SpecialStyle::combined) {
		auto const parsed = parse_stats<DV>(name, it);
		struct Parsed {
			DVs stats;
			CheckedIterator it;
		};
		return Parsed(to_dvs_using_spa_as_spc(parsed.stats), parsed.it);
	} else {
		return parse_stats<IV>(name, it);
	}
}

template<SpecialStyle style>
auto parse_evs(CheckedIterator it) {
	auto const parsed = parse_stats<EV>("EV"sv, it);
	if constexpr (style == SpecialStyle::combined) {
		struct Parsed {
			OldGenEVs stats;
			CheckedIterator it;
		};
		auto const stored = parsed.stats;
		return Parsed(
			OldGenEVs(stored.hp(), stored.atk(), stored.def(), stored.spe(), stored.spa()),
			parsed.it
		);
	} else {
		return parsed;
	}
}

template<SpecialStyle style>
auto parse_pokemon(property_tree::ptree_reader pt) -> tv::optional<InitialPokemon<style>> {
	auto const species_id = parse_species(pt);
	if (!species_id) {
		return tv::none;
	}
	auto const species = id_to_species({*species_id, pt.get<SpeciesIDs::Forme>("<xmlattr>.Forme")});
	auto const nickname = Nickname(pt.get<std::string>("<xmlattr>.Nickname"));
	auto const gender = id_to_gender(pt.get<GenderID>("<xmlattr>.Gender"));
	auto const level = Level(pt.get<Level::value_type>("<xmlattr>.Lvl"));
	auto const happiness = Happiness(pt.get<Happiness::value_type>("<xmlattr>.Happiness"));
	auto const item = id_to_item(pt.get<ItemID>("<xmlattr>.Item"));
	auto const ability = id_to_ability(pt.get<AbilityID>("<xmlattr>.Ability"));
	auto const nature = ingame_id_to_nature(pt.get<IngameNatureID>("<xmlattr>.Nature"));

	auto const it = CheckedIterator(pt.get_child(""));
	auto const parsed_moves = parse_moves(it);

	auto const dvs_or_ivs = parse_dvs_or_ivs<style>(parsed_moves.it);
	auto const evs = parse_evs<style>(dvs_or_ivs.it);

	return InitialPokemon<style>(
		species,
		nickname,
		level,
		gender,
		item,
		ability,
		{
			nature,
			dvs_or_ivs.stats,
			evs.stats
		},
		parsed_moves.moves,
		happiness
	);
}

template<SpecialStyle style>
auto parse_team(property_tree::ptree_reader pt) -> InitialTeam<style> {
	return InitialTeam<style>(containers::remove_none(
		containers::transform(
			containers::filter(
				pt,
				[](auto const & value) { return value.first == "Pokemon"; }
			),
			[=](auto const & value) { return parse_pokemon<style>(value.second); }
		)
	));
}

export auto read_team_file(std::span<std::byte const> const bytes) -> AnyInitialTeam {
	auto owner = property_tree::ptree();
	auto pt = owner.read_xml(bytes);

	auto const all_pokemon = pt.get_child("Team");
	using GenerationInteger = bounded::integer<1, 7>;
	auto const generation = static_cast<Generation>(all_pokemon.get<GenerationInteger>("<xmlattr>.gen"));
	switch (special_style_for(generation)) {
		case SpecialStyle::combined:
			return parse_team<SpecialStyle::combined>(all_pokemon);
		case SpecialStyle::split:
			return parse_team<SpecialStyle::split>(all_pokemon);
	}
}

} // namespace technicalmachine::po
