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
using namespace containers::string_literals;
using namespace ps;

export using PlayerInput = tv::variant<MoveName, BattleResponseSwitch>;

constexpr auto parse_input_for(containers::string_view const player) {
	return [=](nlohmann::json const & json) -> tv::optional<PlayerInput> {
		auto const str = containers::string_view(json.get<std::string_view>());
		if (containers::is_empty(str) or containers::front(str) != '>') {
			throw std::runtime_error(containers::concatenate<std::string>(
				"Selection string "_s,
				str,
				" does not start with >"_s
			));
		}
		auto view = DelimitedBufferView(containers::string_view(containers::drop_exactly(str, 1_bi)), ' ');
		auto const type = view.pop();
		auto matches = [=](auto const ... strs) {
			return (... or (type == strs));
		};
		if (matches("p1"_s, "p2"_s)) {
			if (type != player) {
				return tv::none;
			}
			auto const category = view.pop();
			auto const data = view.pop();
			if (category == "move"_s) {
				return PlayerInput(from_string<MoveName>(data));
			} else if (category == "switch"_s) {
				return PlayerInput(bounded::to_integer<BattleResponseSwitch>(data));
			} else {
				throw std::runtime_error(containers::concatenate<std::string>(
					"Unknown action category "_s,
					category
				));
			}
		} else if (matches("forcelose"_s, "player"_s, "start"_s, "version"_s, "version-origin"_s)) {
			return tv::none;
		} else {
			throw std::runtime_error(containers::concatenate<std::string>(
				"Unknown action type "_s,
				type
			));
		}
	};
}

auto input_for_side(nlohmann::json const & input_log, containers::string_view const player) -> containers::dynamic_array<PlayerInput> {
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
		input_for_side(input_log, "p1"_s),
		input_for_side(input_log, "p2"_s)
	);
}

} // namespace technicalmachine::ps_usage_stats
