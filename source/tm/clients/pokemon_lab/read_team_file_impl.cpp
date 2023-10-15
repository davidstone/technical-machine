// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

module;

#include <std_module/prelude.hpp>
#include <string>
#include <string_view>

module tm.clients.pl.read_team_file;

import tm.move.move;
import tm.move.move_name;
import tm.move.pp;
import tm.move.regular_moves;

import tm.pokemon.happiness;
import tm.pokemon.known_pokemon;
import tm.pokemon.level;
import tm.pokemon.max_pokemon_per_team;
import tm.pokemon.nickname;
import tm.pokemon.species;

import tm.stat.combined_stats;
import tm.stat.ev;
import tm.stat.evs;
import tm.stat.iv;
import tm.stat.iv_and_ev;
import tm.stat.nature;
import tm.stat.stat_style;

import tm.string_conversions.ability;
import tm.string_conversions.gender;
import tm.string_conversions.item;
import tm.string_conversions.move_name;
import tm.string_conversions.nature;
import tm.string_conversions.species;

import tm.ability;
import tm.constant_generation;
import tm.gender;
import tm.generation;
import tm.property_tree;
import tm.item;
import tm.team;

import bounded;
import containers;
import numeric_traits;
import tv;
import std_module;

namespace technicalmachine::pl {
using namespace bounded::literal;
using namespace std::string_view_literals;

constexpr auto parse_species(std::string_view const str) {
	constexpr auto converter = containers::basic_flat_map(
		containers::assume_sorted_unique,
		containers::to_array<containers::map_value_type<std::string_view, Species>>({
			{ "Deoxys", Species::Deoxys_Normal },
			{ "Deoxys-e", Species::Deoxys_Speed },
			{ "Deoxys-f", Species::Deoxys_Attack },
			{ "Deoxys-l", Species::Deoxys_Defense },
			{ "Giratina", Species::Giratina_Altered },
			{ "Giratina-o", Species::Giratina_Origin },
			{ "Rotom-c", Species::Rotom_Mow },
			{ "Rotom-f", Species::Rotom_Frost },
			{ "Rotom-h", Species::Rotom_Heat },
			{ "Rotom-s", Species::Rotom_Fan },
			{ "Rotom-w", Species::Rotom_Wash },
			{ "Shaymin", Species::Shaymin_Land },
			{ "Shaymin-s", Species::Shaymin_Sky },
			{ "Wormadam", Species::Wormadam_Plant },
			{ "Wormadam-g", Species::Wormadam_Sandy },
			{ "Wormadam-s", Species::Wormadam_Trash }
		})
	);
	auto const result = containers::lookup(converter, str);
	return result ? *result : from_string<Species>(str);
}

auto parse_moves(Generation const generation, property_tree::ptree_reader pt) {
	return RegularMoves(containers::transform(pt, [=](property_tree::ptree_reader::value_type const & value) {
		auto const name = from_string<MoveName>(value.second.get_value<std::string>());
		auto const pp_ups = value.second.get<PP::pp_ups_type>("<xmlattr>.pp-up");
		return Move(generation, name, pp_ups);
	}));
}

template<SpecialStyle stat_style>
auto parse_stats(property_tree::ptree_reader pt) {
	constexpr auto is_combined = stat_style == SpecialStyle::combined;
	using StatType = std::conditional_t<is_combined, DVAndEV, IVAndEV>;
	auto hp = tv::optional<StatType>();
	auto atk = tv::optional<StatType>();
	auto def = tv::optional<StatType>();
	auto spa = tv::optional<StatType>();
	auto spd = tv::optional<StatType>();
	auto spe = tv::optional<StatType>();

	for (auto const & value : pt.get_child("stats")) {
		auto const & stats = value.second;
		auto const & stat_name = stats.get<std::string>("<xmlattr>.name");
		using IVType = std::conditional_t<is_combined, DV, IV>;
		auto const iv_and_ev = StatType{
			IVType(stats.get<typename IVType::value_type>("<xmlattr>.iv")),
			EV(stats.get<EV::value_type>("<xmlattr>.ev"))
		};
		if (stat_name == "HP") {
			hp = iv_and_ev;
		} else if (stat_name == "Atk") {
			atk = iv_and_ev;
		} else if (stat_name == "Def") {
			def = iv_and_ev;
		} else if (stat_name == "SpAtk") {
			spa = iv_and_ev;
		} else if (stat_name == "SpDef") {
			spd = iv_and_ev;
		} else if (stat_name == "Spd") {
			spe = iv_and_ev;
		} else {
			throw std::runtime_error(containers::concatenate<std::string>(std::string_view("Invalid stat name "), stat_name));
		}
	}
	if (!hp or !atk or !def or !spa or !spd or !spe) {
		throw std::runtime_error("IV or EV is missing");
	}

	if constexpr (is_combined) {
		// I'm not sure if Pokemon Lab ever supported older generations, but if
		// it did this is probably what the file would look like?
		if (*spa != *spd) {
			throw std::runtime_error("Mismatched Special DV / EV");
		}
		auto const dvs = DVs(atk->dv, def->dv, spe->dv, spa->dv);
		if (dvs.hp() != hp->dv) {
			throw std::runtime_error("Invalid HP DV");
		}
		return CombinedStats<stat_style>{
			Nature::Hardy,
			dvs,
			OldGenEVs(hp->ev, atk->ev, def->ev, spe->ev, spa->ev)
		};
	} else {
		return CombinedStats<stat_style>{
			from_string<Nature>(pt.get<std::string>("nature")),
			IVs(hp->iv, atk->iv, def->iv, spa->iv, spd->iv, spe->iv),
			EVs(hp->ev, atk->ev, def->ev, spa->ev, spd->ev, spe->ev)
		};
	}
}

template<Generation generation>
auto parse_pokemon(property_tree::ptree_reader pt) {
	auto const species_str = pt.get<std::string>("<xmlattr>.species");
	auto const given_nickname = pt.get<std::string>("nickname");
	return KnownPokemon<generation>(
		parse_species(species_str),
		Nickname(containers::is_empty(given_nickname) ? species_str : given_nickname),
		Level(pt.get<Level::value_type>("level")),
		Gender(from_string<Gender>(pt.get<std::string>("gender"))),
		from_string<Item>(pt.get<std::string>("item")),
		Ability(from_string<Ability>(pt.get<std::string>("ability"))),
		parse_stats<special_style_for(generation)>(pt),
		parse_moves(generation, pt.get_child("moveset")),
		Happiness(pt.get<Happiness::value_type>("happiness"))
	);
}

template<Generation generation>
auto parse_team(property_tree::ptree_reader ptree) {
	return KnownTeam<generation>(
		containers::transform(
			containers::check_size_not_greater_than(
				containers::filter(ptree, [](auto const & value) { return value.first == "pokemon"; }),
				numeric_traits::max_value<TeamSize>
			),
			[](auto const & value) { return parse_pokemon<generation>(value.second); }
		)
	);
}

auto read_team_file(std::span<std::byte const> const bytes) -> GenerationGeneric<KnownTeam> {
	auto owner = property_tree::ptree();
	auto pt = owner.read_xml(bytes);

	auto const all_pokemon = pt.get_child("shoddybattle");
	using GenerationInteger = bounded::integer<1, 7>;
	// The original format did not include a generation. Allow users to add
	// this field.
	auto const parsed_generation = static_cast<Generation>(all_pokemon.get("<xmlattr>.generation", GenerationInteger(4_bi)));
	return constant_generation(parsed_generation, [&]<Generation generation>(constant_gen_t<generation>) {
		return GenerationGeneric<KnownTeam>(parse_team<generation>(all_pokemon));
	});
}

} // namespace technicalmachine::pl