// Copyright David Stone 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.clients.ps.parse_moves;

import tm.move.move_name;
import tm.move.move_names;

import tm.string_conversions.move_name;
import tm.string_conversions.type;

import tm.type.type;

import tm.nlohmann_json;

import containers;
import tv;
import std_module;

namespace technicalmachine::ps {
using namespace std::string_view_literals;

constexpr auto parse_hidden_power(std::string_view const str) -> Type {
	constexpr auto prefixes = containers::array({
		"hiddenpower"sv,
		"Hidden Power"sv,
		"hp"sv,
	});
	auto const matches = [=](std::string_view const prefix) {
		return str.starts_with(prefix);
	};
	auto const ptr = containers::maybe_find_if(prefixes, matches);
	if (!ptr) {
		throw std::runtime_error(containers::concatenate<std::string>(
			"Unknown Hidden Power string: "sv,
			str
		));
	}
	auto const type_str = str.substr(ptr->size());
	return type_str.empty() ? Type::Dark : from_string<Type>(type_str);
}

struct MovesAndHiddenPower {
	MoveNames names;
	tv::optional<Type> hidden_power_type;
};

export auto parse_moves(nlohmann::json const & moves) -> MovesAndHiddenPower {
	auto result = MovesAndHiddenPower();
	for (auto const & move : moves) {
		auto const move_str = move.get<std::string_view>();
		auto const move_name = from_string<MoveName>(move_str);
		containers::push_back(result.names, move_name);
		if (move_name == MoveName::Hidden_Power) {
			if (result.hidden_power_type) {
				throw std::runtime_error("Got two Hidden Powers");
			}
			result.hidden_power_type = parse_hidden_power(move_str);
		}
	}
	return result;
}

} // namespace technicalmachine::ps
