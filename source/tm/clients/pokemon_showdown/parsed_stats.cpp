// Copyright David Stone 2024.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.clients.ps.parsed_stats;

import tm.stat.initial_stat;
import tm.stat.stat_style;

import tm.visible_hp;

import bounded;

namespace technicalmachine::ps {

// PS always sends split-special stats
export using ParsedStat = InitialStat<SpecialStyle::split>;

export struct ParsedStats {
	VisibleHP hp;
	ParsedStat atk;
	ParsedStat def;
	ParsedStat spa;
	ParsedStat spd;
	ParsedStat spe;
	friend auto operator==(ParsedStats, ParsedStats) -> bool = default;
};

} // namespace techniclalmachine::ps

template<>
struct bounded::tombstone<technicalmachine::ps::ParsedStats> {
private:
	using base = tombstone_traits<technicalmachine::VisibleHP>;
public:
	static constexpr auto make(auto const index) noexcept -> technicalmachine::ps::ParsedStats {
		return technicalmachine::ps::ParsedStats{
			.hp = base::make(index),
			.atk = {},
			.def = {},
			.spa = {},
			.spd = {},
			.spe = {}
		};
	}
	static constexpr auto index(technicalmachine::ps::ParsedStats const & value) noexcept {
		return base::index(value.hp);
	}
};
