// Copyright David Stone 2021.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.ps_usage_stats.mode;

import containers;
import std_module;

namespace technicalmachine::ps_usage_stats {
using namespace containers::string_literals;

export enum class Mode {
	unweighted,
	weighted,
	weighted_winner,
	top_players
};

export constexpr auto parse_mode(containers::string_view const str) -> Mode {
	return
		str == "unweighted"_s ? Mode::unweighted :
		str == "weighted"_s ? Mode::weighted :
		str == "weighted_winner"_s ? Mode::weighted_winner :
		str == "top_players"_s ? Mode::top_players :
		throw std::runtime_error("Invalid mode");
}

export constexpr auto weight(Mode const mode, double const p) -> double {
	switch (mode) {
		case Mode::unweighted:
			return 1.0;
		case Mode::weighted:
		case Mode::weighted_winner:
		case Mode::top_players:
			return p / (1.0 - p);
	}
}

} // namespace technicalmachine::ps_usage_stats
