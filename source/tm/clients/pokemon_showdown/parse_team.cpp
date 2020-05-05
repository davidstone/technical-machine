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

#include <tm/clients/pokemon_showdown/parse_team.hpp>

#include <tm/clients/pokemon_showdown/battle_parser.hpp>
#include <tm/clients/pokemon_showdown/json_parser.hpp>

#include <tm/stat/stat_to_ev.hpp>

#include <tm/string_conversions/ability.hpp>
#include <tm/string_conversions/item.hpp>
#include <tm/string_conversions/move.hpp>

#include <iostream>

namespace technicalmachine {
namespace ps {

namespace {

auto parse_stats(HP::max_type const hp, boost::property_tree::ptree const & stats) {
	auto get = [&](char const * str) {
		using CheckedStatValue = bounded::change_policy<StatValue, bounded::checked_integer>;
		return StatValue(stats.get<CheckedStatValue>(str));
	};
	auto const attack = get("atk");
	auto const defense = get("def");
	auto const special_attack = get("spa");
	auto const special_defense = get("spd");
	auto const speed = get("spe");
	return GenericStats{hp, attack, defense, special_attack, special_defense, speed};
}

} // namespace

auto parse_team(boost::property_tree::ptree const & pt, Generation const generation) -> Team {
	auto const team_data = containers::range_view(pt.get_child("side").get_child("pokemon").equal_range(""));
	constexpr bool is_me = true;
	auto team = Team(TeamSize(containers::distance(begin(team_data), end(team_data))), is_me);
	for (auto const & pokemon_data : team_data) {
		auto get = [&](auto const & key) { return pokemon_data.second.get<std::string>(key); };

		auto const details = parse_details(get("details"));
		
		auto const condition = get("condition");
		// current_hp/max_hp
		// Presumably also gives me status information? Should be useful for
		// rejoining battles
		// TODO: If we disconnect in a battle when the HP is 0, we might not
		// have a '/'
		auto const hp = bounded::to_integer<HP::max_type>(split(condition, '/').first);
		
		auto const evs = calculate_evs(generation, details.species, details.level, parse_stats(hp, pokemon_data.second.get_child("stats")));

		auto const ability = from_string<Ability>(get("baseAbility"));
		
		auto const item = from_string<Item>(get("item"));
		
		Pokemon & pokemon = team.add_pokemon(generation, details.species, details.level, details.gender, item, ability, evs.nature);
		
		for (auto const & move : pokemon_data.second.get_child("moves")) {
			 add_seen_move(all_moves(pokemon), generation, from_string<Moves>(move.second.get<std::string>("")));
		}

		set_hp_ev(generation, pokemon, evs.hp);
		set_stat_ev(pokemon, StatNames::ATK, evs.attack);
		set_stat_ev(pokemon, StatNames::DEF, evs.defense);
		set_stat_ev(pokemon, StatNames::SPA, evs.special_attack);
		set_stat_ev(pokemon, StatNames::SPD, evs.special_defense);
		set_stat_ev(pokemon, StatNames::SPE, evs.speed);
	}
	team.all_pokemon().reset_index();
	return team;
}

auto parse_team(std::string_view const str, Generation const generation) -> Team {
	return parse_team(JSONParser()(str), generation);
}

}	// namespace ps
}	// namespace technicalmachine
