// Copyright David Stone 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.ps_usage_stats.parse_input_log;

import tm.clients.ps.battle_response_switch;

import tm.move.move_name;

import tm.string_conversions.move_name;

import tm.buffer_view;
import tm.nlohmann_json;

import bounded;
import containers;
import std_module;
import tv;

namespace technicalmachine::ps_usage_stats {
using namespace bounded::literal;
using namespace std::string_view_literals;
using namespace ps;

export using PlayerInput = tv::variant<MoveName, BattleResponseSwitch>;

constexpr auto parse_input_for(std::string_view const player) {
	return [=](nlohmann::json const & json) -> tv::optional<PlayerInput> {
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
		auto matches = [=](auto const ... strs) {
			return (... or (type == strs));
		};
		if (matches("p1"sv, "p2"sv)) {
			if (type != player) {
				return tv::none;
			}
			auto const category = view.pop();
			auto const data = view.pop();
			if (category == "move"sv) {
				return PlayerInput(from_string<MoveName>(data));
			} else if (category == "switch"sv) {
				return PlayerInput(bounded::to_integer<BattleResponseSwitch>(data));
			} else {
				throw std::runtime_error(containers::concatenate<std::string>(
					"Unknown action category "sv,
					category
				));
			}
		} else if (matches("forcelose"sv, "player"sv, "start"sv, "version"sv, "version-origin"sv)) {
			return tv::none;
		} else {
			throw std::runtime_error(containers::concatenate<std::string>(
				"Unknown action type "sv,
				type
			));
		}
	};
}

auto input_for_side(nlohmann::json const & input_log, std::string_view const player) -> containers::dynamic_array<PlayerInput> {
	return containers::dynamic_array(
		containers::remove_none(containers::transform(
			input_log,
			parse_input_for(player)
		))
	);
};

export struct InputLog {
	containers::dynamic_array<PlayerInput> side1;
	containers::dynamic_array<PlayerInput> side2;
	friend auto operator==(InputLog const &, InputLog const &) -> bool = default;
};
export auto parse_input_log(nlohmann::json const & input_log) -> InputLog {
	return InputLog(
		input_for_side(input_log, "p1"sv),
		input_for_side(input_log, "p2"sv)
	);
}

} // namespace technicalmachine::ps_usage_stats
