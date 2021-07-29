// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <tm/clients/pokemon_showdown/battle_parser.hpp>

#include <tm/stat/calculate_ivs_and_evs.hpp>
#include <tm/stat/initial_stat.hpp>

#include <tm/string_conversions/ability.hpp>
#include <tm/string_conversions/item.hpp>
#include <tm/string_conversions/move.hpp>
#include <tm/string_conversions/type.hpp>

#include <tm/team.hpp>

#include <bounded/check_in_range.hpp>
#include <bounded/to_integer.hpp>

#include <containers/size.hpp>

#include <nlohmann/json.hpp>

#include <stdexcept>
#include <string>
#include <string_view>

namespace technicalmachine {

namespace ps {

template<Generation generation>
auto parse_stats(HP::max_type const hp, nlohmann::json const & stats) {
	auto get = [&](char const * const str) {
		return bounded::check_in_range<InitialStat<generation>>(bounded::integer(stats.at(str).get<nlohmann::json::number_integer_t>()));
	};
	auto const attack = get("atk");
	auto const defense = get("def");
	auto const special_attack = get("spa");
	auto const special_defense = get("spd");
	auto const speed = get("spe");
	return Stats<generation>(HP(hp), attack, defense, special_attack, special_defense, speed);
}

template<Generation generation>
constexpr auto parse_hidden_power(std::string_view const str) {
	constexpr auto prefix = std::string_view("hiddenpower");
	if (!str.starts_with(prefix)) {
		throw std::runtime_error("Hidden Power doesn't start with \"hiddenpower\" in PS team string");
	}
	// TODO: ???
	constexpr auto const power = [] {
		if constexpr (generation <= Generation::five) {
			return 70_bi;
		} else {
			return 60_bi;
		}
	}();
	auto const type = from_string<Type>(str.substr(prefix.size()));
	return HiddenPower<generation>(power, type);
}

template<Generation generation>
auto parse_moves(nlohmann::json const & moves) {
	struct Result {
		RegularMoves moves;
		bounded::optional<HiddenPower<generation>> hidden_power;
	};
	auto result = Result();
	for (auto const & move : moves) {
		auto const move_str = move.get<std::string_view>();
		auto const move_name = from_string<Moves>(move_str);
		result.moves.push_back(Move(generation, move_name));
		if (move_name == Moves::Hidden_Power) {
			if constexpr (generation == Generation::one) {
				throw std::runtime_error("Generation 1 does not have Hidden Power");
			} else {
				insert(result.hidden_power, parse_hidden_power<generation>(move_str));
			}
		}
	}
	return result;
}

template<Generation generation>
auto parse_pokemon(nlohmann::json const & pokemon_data) {
	auto get = [&](char const * key) { return pokemon_data.at(key).get<std::string_view>(); };

	auto const details = parse_details(get("details"));
	
	auto const condition = get("condition");
	// current_hp/max_hp
	// Presumably also gives me status information? Should be useful for
	// rejoining battles
	// TODO: If we disconnect in a battle when the HP is 0, we might not
	// have a '/'
	auto const hp = bounded::to_integer<HP::max_type>(split_view(condition, '/').first);

	auto const move_data = parse_moves<generation>(pokemon_data.at("moves"));
	auto const stats = calculate_ivs_and_evs(
		details.species,
		details.level,
		parse_stats<generation>(hp, pokemon_data.at("stats")),
		move_data.hidden_power
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
auto parse_team(std::string_view const str) -> Team<generation> {
	auto const json = nlohmann::json::parse(str);
	auto const team_data = json.at("side").at("pokemon");
	constexpr bool is_me = true;
	auto const team_size = bounded::check_in_range<TeamSize>(bounded::integer(containers::detail::linear_size(team_data)));
	auto team = Team<generation>(team_size, is_me);
	for (auto const & pokemon_data : team_data.items()) {
		team.add_pokemon(parse_pokemon<generation>(pokemon_data.value()));
	}
	team.all_pokemon().set_index(0_bi);
	return team;
}

}	// namespace ps
}	// namespace technicalmachine
