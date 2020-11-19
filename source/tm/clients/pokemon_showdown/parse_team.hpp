// Copyright (C) 2020 David Stone
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

#include <tm/clients/pokemon_showdown/battle_parser.hpp>
#include <tm/clients/pokemon_showdown/json_parser.hpp>

#include <tm/pokemon/has_physical_or_special_move.hpp>

#include <tm/stat/stat_to_ev.hpp>

#include <tm/string_conversions/ability.hpp>
#include <tm/string_conversions/item.hpp>
#include <tm/string_conversions/move.hpp>
#include <tm/string_conversions/type.hpp>

#include <tm/team.hpp>

#include <bounded/check_in_range.hpp>
#include <bounded/to_integer.hpp>

#include <containers/size.hpp>

#include <boost/property_tree/ptree.hpp>

#include <stdexcept>
#include <string>
#include <string_view>

namespace technicalmachine {

namespace ps {

inline auto parse_stats(HP::max_type const hp, boost::property_tree::ptree const & stats) {
	auto get = [&](char const * str) {
		return stats.get<InitialStat>(str);
	};
	auto const attack = get("atk");
	auto const defense = get("def");
	auto const special_attack = get("spa");
	auto const special_defense = get("spd");
	auto const speed = get("spe");
	return GenericStats{hp, attack, defense, special_attack, special_defense, speed};
}

inline auto hidden_power_type(std::string_view const str) {
	constexpr auto prefix = std::string_view("hiddenpower");
	if (!str.starts_with(prefix)) {
		throw std::runtime_error("Hidden Power doesn't start with \"hiddenpower\" in PS team string");
	}
	return from_string<Type>(str.substr(prefix.size()));
}

inline auto parse_moves(boost::property_tree::ptree const & moves, Generation const generation) {
	struct Result {
		RegularMoves moves;
		bounded::optional<Type> hidden_power_type;
	};
	auto result = Result();
	for (auto const & move : moves) {
		auto const move_str = move.second.get<std::string>("");
		auto const move_name = from_string<Moves>(move_str);
		result.moves.push_back(Move(generation, move_name));
		if (move_name == Moves::Hidden_Power) {
			insert(result.hidden_power_type, hidden_power_type(move_str));
		}
	}
	return result;
}

template<Generation generation>
auto parse_pokemon(boost::property_tree::ptree const & pokemon_data) {
	auto get = [&](auto const & key) { return pokemon_data.get<std::string>(key); };

	auto const details = parse_details(get("details"));
	
	auto const condition = get("condition");
	// current_hp/max_hp
	// Presumably also gives me status information? Should be useful for
	// rejoining battles
	// TODO: If we disconnect in a battle when the HP is 0, we might not
	// have a '/'
	auto const hp = bounded::to_integer<HP::max_type>(split_view(condition, '/').first);

	auto const move_data = parse_moves(pokemon_data.get_child("moves"), generation);
	auto const stats = calculate_ivs_and_evs(
		generation,
		details.species,
		details.level,
		parse_stats(hp, pokemon_data.get_child("stats")),
		move_data.hidden_power_type,
		has_physical_move(generation, move_data.moves, move_data.hidden_power_type)
	);

	auto const ability = from_string<Ability>(get("baseAbility"));
	
	auto const item = from_string<Item>(get("item"));

	return Pokemon<generation>(
		details.species,
		details.level,
		details.gender,
		item,
		ability,
		stats,
		move_data.moves
	);
}

template<Generation generation>
auto parse_team(boost::property_tree::ptree const & pt) -> Team<generation> {
	auto const team_data = containers::range_view(pt.get_child("side").get_child("pokemon").equal_range(""));
	constexpr bool is_me = true;
	auto const team_size = bounded::check_in_range<TeamSize>(bounded::integer(containers::detail::linear_size(team_data)));
	auto team = Team<generation>(team_size, is_me);
	for (auto const & pokemon_data : team_data) {
		team.add_pokemon(parse_pokemon<generation>(pokemon_data.second));
	}
	team.all_pokemon().reset_index();
	return team;
}

template<Generation generation>
auto parse_team(std::string_view const str) -> Team<generation> {
	return parse_team<generation>(JSONParser()(str));
}

}	// namespace ps
}	// namespace technicalmachine
