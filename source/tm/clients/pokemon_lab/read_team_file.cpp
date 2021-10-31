// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <tm/clients/pokemon_lab/read_team_file.hpp>

#include <tm/move/move.hpp>

#include <tm/pokemon/pokemon.hpp>
#include <tm/pokemon/species.hpp>

#include <tm/stat/ev.hpp>
#include <tm/stat/iv.hpp>
#include <tm/stat/iv_and_ev.hpp>

#include <tm/string_conversions/ability.hpp>
#include <tm/string_conversions/gender.hpp>
#include <tm/string_conversions/item.hpp>
#include <tm/string_conversions/move.hpp>
#include <tm/string_conversions/nature.hpp>
#include <tm/string_conversions/species.hpp>

#include <tm/constant_generation.hpp>
#include <tm/generation.hpp>
#include <tm/team.hpp>

#include <containers/algorithms/concatenate.hpp>
#include <containers/array.hpp>
#include <containers/flat_map.hpp>
#include <containers/lookup.hpp>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include <filesystem>
#include <string>
#include <string_view>
#include <stdexcept>

namespace technicalmachine::pl {
namespace {
using namespace std::string_view_literals;

constexpr auto parse_species(std::string_view const str) {
	using Storage = containers::array<containers::map_value_type<std::string_view, Species>, 16>;
	constexpr auto converter = containers::basic_flat_map<Storage>(
		containers::assume_sorted_unique,
		Storage{{
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
		}}
	);
	auto const result = containers::lookup(converter, str);
	return result ? *result : from_string<Species>(str);
}

auto parse_moves(Generation const generation, boost::property_tree::ptree const & pt) {
	auto moves = RegularMoves();
	for (boost::property_tree::ptree::value_type const & value : pt) {
		auto const name = from_string<Moves>(value.second.get_value<std::string>());
		auto const pp_ups = value.second.get<PP::pp_ups_type>("<xmlattr>.pp-up");
		moves.push_back(Move(generation, name, pp_ups));
	}
	return moves;
}

template<Generation generation>
auto parse_stats(boost::property_tree::ptree const & pt) {
	using StatType = std::conditional_t<generation <= Generation::two, DVAndEV, IVAndEV>;
	auto hp = bounded::optional<StatType>();
	auto atk = bounded::optional<StatType>();
	auto def = bounded::optional<StatType>();
	auto spa = bounded::optional<StatType>();
	auto spd = bounded::optional<StatType>();
	auto spe = bounded::optional<StatType>();

	for (auto const & value : pt.get_child("stats")) {
		auto const & stats = value.second;
		auto const & stat_name = stats.get<std::string>("<xmlattr>.name");
		using IVType = std::conditional_t<generation <= Generation::two, DV, IV>;
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

	if constexpr (generation <= Generation::two) {
		// I'm not sure if Pokemon Lab ever supported older generations, but if
		// it did this is probably what the file would look like?
		if (*spa != *spd) {
			throw std::runtime_error("Mismatched Special DV / EV");
		}
		auto const dvs = DVs(atk->dv, def->dv, spe->dv, spa->dv);
		if (dvs.hp() != hp->dv) {
			throw std::runtime_error("Invalid HP DV");
		}
		return CombinedStats<generation>{
			Nature::Hardy,
			dvs,
			OldGenEVs(hp->ev, atk->ev, def->ev, spe->ev, spa->ev)
		};
	} else {
		return CombinedStats<generation>{
			from_string<Nature>(pt.get<std::string>("nature")),
			IVs(hp->iv, atk->iv, def->iv, spa->iv, spd->iv, spe->iv),
			EVs(hp->ev, atk->ev, def->ev, spa->ev, spd->ev, spe->ev)
		};
	}
}

template<Generation generation>
auto parse_pokemon(boost::property_tree::ptree const & pt) {
	// auto const given_nickname = pt.get<std::string>("nickname");
	// auto const nickname = nickname_temp.empty() ? species_str : given_nickname;
	return Pokemon<generation>(
		parse_species(pt.get<std::string>("<xmlattr>.species")),
		Level(pt.get<Level::value_type>("level")),
		Gender(from_string<Gender>(pt.get<std::string>("gender"))),
		from_string<Item>(pt.get<std::string>("item")),
		Ability(from_string<Ability>(pt.get<std::string>("ability"))),
		parse_stats<generation>(pt),
		parse_moves(generation, pt.get_child("moveset")),
		Happiness(pt.get<Happiness::value_type>("happiness"))
	);
}

auto number_of_pokemon(boost::property_tree::ptree const & pt) -> TeamSize {
	auto pokemon_count = TeamSize(0_bi);
	for (auto const & value : pt) {
		if (value.first == "pokemon") {
			if (pokemon_count == numeric_traits::max_value<TeamSize>) {
				throw std::runtime_error("Attempted to add too many Pokemon");
			}
			++pokemon_count;
		}
	}
	return pokemon_count;
}

template<Generation generation>
auto parse_team(boost::property_tree::ptree const & all_pokemon) {
	constexpr bool is_me = true;
	auto const team_size = number_of_pokemon(all_pokemon);
	auto team = Team<generation>(team_size, is_me);
	for (auto const & value : all_pokemon) {
		if (value.first == "pokemon") {
			team.add_pokemon(parse_pokemon<generation>(value.second));
		}
	}
	team.all_pokemon().set_index(0_bi);
	return team;
}

} // namespace

auto read_team_file(std::filesystem::path const & team_file) -> GenerationGeneric<Team> {
	try {
		boost::property_tree::ptree pt;
		read_xml(team_file.string(), pt);
		
		auto const all_pokemon = pt.get_child("shoddybattle");
		using GenerationInteger = bounded::integer<1, 7>;
		// The original format did not include a generation. Require users to add
		// this field.
		auto const parsed_generation = static_cast<Generation>(all_pokemon.get<GenerationInteger>("<xmlattr>.generation"));
		return constant_generation(parsed_generation, [&]<Generation generation>(constant_gen_t<generation>) {
			return GenerationGeneric<Team>(parse_team<generation>(all_pokemon));
		});
	} catch (std::exception const & ex) {
		throw std::runtime_error(containers::concatenate<std::string>("Failed to parse Pokemon Lab team file \""sv, team_file.string(), "\" -- "sv, std::string_view(ex.what())));
	}
}

} // namespace technicalmachine::pl