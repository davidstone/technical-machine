// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <tm/clients/pokemon_lab/read_team_file.hpp>

#include <tm/move/move.hpp>

#include <tm/pokemon/pokemon.hpp>
#include <tm/pokemon/species.hpp>

#include <tm/string_conversions/move.hpp>
#include <tm/string_conversions/nature.hpp>
#include <tm/string_conversions/species.hpp>

#include <containers/array/array.hpp>
#include <containers/begin_end.hpp>
#include <containers/flat_map.hpp>
#include <containers/lookup.hpp>

namespace technicalmachine {
namespace pl {

auto stat_from_simulator_string(std::string_view const str) -> PermanentStat {
	using Storage = containers::array<containers::map_value_type<std::string_view, PermanentStat>, 6>;
	constexpr auto converter = containers::basic_flat_map<Storage>(
		containers::assume_sorted_unique,
		Storage{{
			{ "Atk", PermanentStat::atk },
			{ "Def", PermanentStat::def },
			{ "HP", PermanentStat::hp },
			{ "SpAtk", PermanentStat::spa },
			{ "SpDef", PermanentStat::spd },
			{ "Spd", PermanentStat::spe },
		}}
	);
	auto result = containers::lookup(converter, str);
	if (!result) {
		throw std::runtime_error("Invalid stat name " + std::string(str));
	}
	return *result;
}

auto load_stats(boost::property_tree::ptree const & pt) -> CombinedStats<IVAndEV> {
	auto result = CombinedStats<IVAndEV>{
		from_string<Nature>(pt.get<std::string>("nature")),
		{IV(0_bi), EV(0_bi)},
		{IV(0_bi), EV(0_bi)},
		{IV(0_bi), EV(0_bi)},
		{IV(0_bi), EV(0_bi)},
		{IV(0_bi), EV(0_bi)},
		{IV(0_bi), EV(0_bi)},
	};
	for (auto const & value : pt.get_child("stats")) {
		auto const & stats = value.second;
		auto const stat_name = stat_from_simulator_string(stats.get<std::string>("<xmlattr>.name"));
		auto const iv = IV(stats.get<IV::value_type>("<xmlattr>.iv"));
		auto const ev = EV(stats.get<EV::value_type>("<xmlattr>.ev"));
		result[stat_name] = {iv, ev};
	}
	return result;
}

auto species_from_simulator_string(std::string_view const str) -> Species {
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

auto load_moves(Generation const generation, boost::property_tree::ptree const & pt) -> RegularMoves {
	auto moves = RegularMoves();
	for (boost::property_tree::ptree::value_type const & value : pt) {
		auto const name = from_string<Moves>(value.second.get_value<std::string>());
		auto const pp_ups = value.second.get<PP::pp_ups_type>("<xmlattr>.pp-up");
		moves.push_back(Move(generation, name, pp_ups));
	}
	return moves;
}

} // namespace pl
} // namespace technicalmachine
