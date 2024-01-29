// Copyright David Stone 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

module;

#include <std_module/prelude.hpp>
#include <string_view>

export module tm.ps_usage_stats.parse_input_log;

import tm.clients.ps.battle_response_switch;
import tm.clients.ps.make_party;

import tm.clients.party;

import tm.move.move_name;

import tm.string_conversions.move_name;

import tm.buffer_view;
import tm.nlohmann_json;

import bounded;
import containers;
import std_module;
import tv;

namespace technicalmachine::ps_usage_stats {
using namespace std::string_view_literals;
using namespace ps;

export struct PlayerInput {
	Party party;
	tv::variant<MoveName, BattleResponseSwitch> selection;
	friend auto operator==(PlayerInput, PlayerInput) -> bool = default;
};

constexpr auto parse_input = [](nlohmann::json const & json) -> tv::optional<PlayerInput> {
	auto const str = json.get<std::string_view>();
	if (str.empty() or str.front() != '>') {
		throw std::runtime_error(containers::concatenate<std::string>(
			"Selection string "sv,
			str,
			" does not start with >"sv
		));
	}
	auto view = DelimitedBufferView(str.substr(1), ' ');
	auto const type = view.pop();
	if (type == "version"sv or type == "version-origin"sv) {
		return tv::none;
	} else if (type == "start"sv) {
		return tv::none;
	} else if (type == "player"sv) {
		return tv::none;
	} else if (type == "forcelose"sv) {
		return tv::none;
	} else if (type == "p1"sv or type == "p2"sv) {
		auto const party = make_party(type);
		auto const category = view.pop();
		auto const data = view.pop();
		if (category == "move"sv) {
			return PlayerInput(
				party,
				from_string<MoveName>(data)
			);
		} else if (category == "switch"sv) {
			return PlayerInput(
				party,
				bounded::to_integer<BattleResponseSwitch>(data)
			);
		} else {
			throw std::runtime_error(containers::concatenate<std::string>(
				"Unknown action category "sv,
				category
			));
		}
	} else {
		throw std::runtime_error(containers::concatenate<std::string>(
			"Unknown action type "sv,
			type
		));
	}
};

export auto parse_input_log(nlohmann::json const & input_log) -> containers::dynamic_array<PlayerInput> {
	return containers::dynamic_array(containers::remove_none(containers::transform(
		input_log,
		parse_input
	)));
}

} // namespace technicalmachine::ps_usage_stats
