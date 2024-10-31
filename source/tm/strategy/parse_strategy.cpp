// Copyright David Stone 2024.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

module;

#include <std_module/prelude.hpp>
#include <string_view>

export module tm.strategy.parse_strategy;

import tm.evaluate.depth;

import tm.strategy.expectimax;
import tm.strategy.max_damage;
import tm.strategy.net_hp;
import tm.strategy.random_selection;
import tm.strategy.statistical;
import tm.strategy.strategy;

import bounded;
import std_module;

namespace technicalmachine {
using namespace std::string_view_literals;

export auto parse_strategy(int argc, char const * const * argv) -> Strategy {
	auto const name = std::string_view(argv[0]);
	if (name == "random"sv) {
		if (argc != 1) {
			throw std::runtime_error("random strategy accepts no arguments");
		}
		return make_random_selection();
	} else if (name == "random_weighted"sv) {
		if (argc != 2) {
			throw std::runtime_error("random_weighted strategy requires probability argument");
		}
		return make_random_selection(std::stod(argv[1]));
	} else if (name == "max_damage"sv) {
		if (argc != 1) {
			throw std::runtime_error("max_damage strategy accepts no arguments");
		}
		return make_max_damage();
	} else if (name == "net_hp"sv) {
		if (argc != 1) {
			throw std::runtime_error("net_hp strategy accepts no arguments");
		}
		return make_net_hp();
	} else if (name == "statistical"sv) {
		if (argc != 1) {
			throw std::runtime_error("statistical strategy accepts no arguments");
		}
		return make_statistical();
	} else if (name == "expectimax"sv) {
		if (argc < 4) {
			throw std::runtime_error("expectimax strategy requires two depth arguments followed by a foe strategy");
		}
		return make_expectimax(
			Depth(
				bounded::to_integer<DepthInt>(argv[1]),
				bounded::to_integer<DepthInt>(argv[2])
			),
			parse_strategy(argc - 3, argv + 3)
		);
	} else {
		throw std::runtime_error("Selection strategy must be one of random, random_weighted, max_damage, net_hp, statistical, expectimax");
	}
}

} // namespace technicalmachine
