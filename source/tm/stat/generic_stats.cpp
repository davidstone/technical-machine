// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.stat.generic_stats;

import tm.stat.stat_names;

import bounded;

namespace technicalmachine {

export template<typename HP, typename Stat = HP>
struct GenericStats {
	constexpr auto operator[](
		[[clang::lifetimebound]] this auto && stats,
		SplitSpecialPermanentStat const index
	) -> auto && {
		using enum SplitSpecialPermanentStat;
		switch (index) {
			case hp: return stats.hp;
			case atk: return stats.atk;
			case def: return stats.def;
			case spa: return stats.spa;
			case spd: return stats.spd;
			case spe: return stats.spe;
		}
	}
	constexpr auto operator[](
		[[clang::lifetimebound]] this auto && stats,
		SplitSpecialRegularStat const index
	) -> auto && {
		using enum SplitSpecialRegularStat;
		switch (index) {
			case atk: return stats.atk;
			case def: return stats.def;
			case spa: return stats.spa;
			case spd: return stats.spd;
			case spe: return stats.spe;
		}
	}
	friend auto operator==(GenericStats, GenericStats) -> bool = default;

	HP hp;
	Stat atk;
	Stat def;
	Stat spa;
	Stat spd;
	Stat spe;
};

constexpr auto index_stat(
	[[clang::lifetimebound]] auto && stats,
	SpecialRegularStat const index
) -> auto && {
	using enum SpecialRegularStat;
	switch (index) {
		case atk: return stats.atk;
		case def: return stats.def;
		case spe: return stats.spe;
		case spc: return stats.spc;
	}
}

export template<typename HP, typename Stat = HP>
struct GenericCombinedStats {
	constexpr auto operator[](
		[[clang::lifetimebound]] this auto && stats,
		SpecialPermanentStat const index
	) -> auto && {
		using enum SpecialPermanentStat;
		switch (index) {
			case hp: return stats.hp;
			case atk: return stats.atk;
			case def: return stats.def;
			case spe: return stats.spe;
			case spc: return stats.spc;
		}
	}
	constexpr auto operator[](
		[[clang::lifetimebound]] this auto && stats,
		SpecialRegularStat const index
	) -> auto && {
		return index_stat(stats, index);
	}
	friend auto operator==(GenericCombinedStats, GenericCombinedStats) -> bool = default;

	HP hp;
	Stat atk;
	Stat def;
	Stat spe;
	Stat spc;
};

export template<typename Stat>
struct GenericDVStats {
	constexpr auto operator[](
		[[clang::lifetimebound]] this auto && stats,
		SpecialRegularStat const index
	) -> auto && {
		return index_stat(stats, index);
	}
	friend auto operator==(GenericDVStats, GenericDVStats) -> bool = default;

	Stat atk;
	Stat def;
	Stat spe;
	Stat spc;
};

} // namespace technicalmachine

template<typename HP, typename Stat>
struct bounded::tombstone<technicalmachine::GenericStats<HP, Stat>> {
	using base = tombstone<HP>;
	static constexpr auto make(auto const index) noexcept -> technicalmachine::GenericStats<HP, Stat> {
		return technicalmachine::GenericStats<HP, Stat>{
			.hp = base::make(index),
			.atk = {},
			.def = {},
			.spa = {},
			.spd = {},
			.spe = {},
		};
	}
	static constexpr auto index(technicalmachine::GenericStats<HP, Stat> const & value) noexcept {
		return base::index(value.hp);
	}
};

template<typename HP, typename Stat>
struct bounded::tombstone<technicalmachine::GenericCombinedStats<HP, Stat>> {
	using base = tombstone<HP>;
	static constexpr auto make(auto const index) noexcept -> technicalmachine::GenericCombinedStats<HP, Stat> {
		return technicalmachine::GenericCombinedStats<HP, Stat>{
			.hp = base::make(index),
			.atk = {},
			.def = {},
			.spe = {},
			.spc = {},
		};
	}
	static constexpr auto index(technicalmachine::GenericCombinedStats<HP, Stat> const & value) noexcept {
		return base::index(value.hp);
	}
};