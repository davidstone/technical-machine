// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.test.stat.calculate_ivs_and_evs;

import tm.pokemon.hidden_power;
import tm.pokemon.level;
import tm.pokemon.species;

import tm.stat.calculate_ivs_and_evs;
import tm.stat.combined_stats;
import tm.stat.ev;
import tm.stat.evs;
import tm.stat.iv;
import tm.stat.hp;
import tm.stat.nature;
import tm.stat.stat_style;
import tm.stat.stats;

import tm.type.type;

import tm.generation;

import bounded;
import containers;
import tv;

namespace technicalmachine {
using namespace bounded::literal;

// Generation 1 min special
static_assert([]{
	constexpr auto generation = Generation::one;
	constexpr auto species = Species::Snorlax;
	constexpr auto level = Level(100_bi);
	constexpr auto stats = Stats<stat_style_for(generation)>{
		.hp = HP(521_bi),
		.atk = 318_bi,
		.def = 228_bi,
		.spe = 158_bi,
		.spc = 135_bi,
	};
	constexpr auto hidden_power = tv::optional<HiddenPower<generation>>();

	constexpr auto expected = CombinedStatsFor<generation>{
		.dvs_or_ivs = DVs{
			.atk = DV(15_bi),
			.def = DV(15_bi),
			.spe = DV(15_bi),
			.spc = DV(0_bi),
		},
		.evs = OldGenEVs{
			.hp = EV(252_bi),
			.atk = EV(252_bi),
			.def = EV(252_bi),
			.spe = EV(252_bi),
			.spc = EV(0_bi),
		}
	};

	constexpr auto calculated = calculate_ivs_and_evs(species, level, stats, hidden_power);

	return calculated == expected;
}());


// Calculate low Attack EVs
static_assert([]{
	constexpr auto generation = Generation::four;
	constexpr auto species = Species::Tentacruel;
	constexpr auto level = Level(100_bi);
	constexpr auto stats = Stats<stat_style_for(generation)>{
		.hp = HP(364_bi),
		.atk = 133_bi,
		.def = 177_bi,
		.spa = 196_bi,
		.spd = 352_bi,
		.spe = 245_bi,
	};
	constexpr auto hidden_power = tv::optional<HiddenPower<generation>>();
	constexpr auto nature = Nature::Calm;

	auto const calculated = calculate_ivs_and_evs(species, level, stats, hidden_power, containers::enum_range(nature, nature));

	constexpr auto expected = CombinedStatsFor<generation>{
		nature,
		IVs{
			.hp = IV(31_bi),
			.atk = IV(3_bi),
			.def = IV(31_bi),
			.spa = IV(31_bi),
			.spd = IV(31_bi),
			.spe = IV(31_bi),
		},
		EVs{
			.hp = EV(252_bi),
			.atk = EV(0_bi),
			.def = EV(44_bi),
			.spa = EV(0_bi),
			.spd = EV(176_bi),
			.spe = EV(36_bi),
		}
	};
	return calculated == expected;
}());


// calculate_ivs_and_evs in generation 2 with Hidden Power
static_assert([]{
	constexpr auto generation = Generation::two;
	constexpr auto species = Species::Sunflora;
	constexpr auto level = Level(80_bi);
	constexpr auto stats = Stats<stat_style_for(generation)>{
		.hp = HP(284_bi),
		.atk = 199_bi,
		.def = 164_bi,
		.spa = 247_bi,
		.spd = 215_bi,
		.spe = 127_bi,
	};
	constexpr auto hidden_power = tv::optional(HiddenPower<generation>(70_bi, Type::Ice));

	auto const calculated = calculate_ivs_and_evs(species, level, stats, hidden_power);

	constexpr auto expected = CombinedStatsFor<generation>{
		Nature::Hardy,
		DVs{
			.atk = DV(15_bi),
			.def = DV(13_bi),
			.spe = DV(15_bi),
			.spc = DV(15_bi),
		},
		OldGenEVs{
			.hp = EV(252_bi),
			.atk = EV(252_bi),
			.def = EV(252_bi),
			.spe = EV(252_bi),
			.spc = EV(252_bi),
		}
	};
	return calculated == expected;
}());

// calculate_ivs_and_evs in generation 3 with Hidden Power
static_assert([]{
	constexpr auto generation = Generation::three;
	constexpr auto species = Species::Kingler;
	constexpr auto level = Level(100_bi);
	constexpr auto nature = Nature::Jolly;
	constexpr auto stats = Stats<stat_style_for(generation)>{
		.hp = HP(251_bi),
		.atk = 359_bi,
		.def = 265_bi,
		.spa = 122_bi,
		.spd = 136_bi,
		.spe = 273_bi,
	};
	constexpr auto hidden_power = tv::optional(HiddenPower<generation>(70_bi, Type::Ghost));

	auto const calculated = calculate_ivs_and_evs(species, level, stats, hidden_power, containers::enum_range(nature, nature));

	constexpr auto expected = CombinedStatsFor<generation>{
		nature,
		IVs{
			.hp = IV(31_bi),
			.atk = IV(31_bi),
			.def = IV(30_bi),
			.spa = IV(31_bi),
			.spd = IV(30_bi),
			.spe = IV(31_bi),
		},
		EVs{
			.hp = EV(0_bi),
			.atk = EV(252_bi),
			.def = EV(0_bi),
			.spa = EV(0_bi),
			.spd = EV(4_bi),
			.spe = EV(252_bi),
		}
	};
	return calculated == expected;
}());

// calculate_ivs_and_evs at level 1
static_assert([]{
	constexpr auto generation = Generation::three;
	constexpr auto species = Species::Gengar;
	constexpr auto level = Level(1_bi);
	constexpr auto stats = Stats<stat_style_for(generation)>{
		.hp = HP(12_bi),
		.atk = 6_bi,
		.def = 7_bi,
		.spa = 8_bi,
		.spd = 7_bi,
		.spe = 8_bi,
	};
	constexpr auto hidden_power = tv::optional<HiddenPower<generation>>(tv::none);
	constexpr auto nature = Nature::Hardy;

	auto const calculated = calculate_ivs_and_evs(species, level, stats, hidden_power, containers::enum_range(nature, nature));

	constexpr auto expected = CombinedStatsFor<generation>{
		nature,
		IVs{
			.hp = IV(31_bi),
			.atk = IV(31_bi),
			.def = IV(31_bi),
			.spa = IV(31_bi),
			.spd = IV(31_bi),
			.spe = IV(31_bi),
		},
		EVs{
			.hp = EV(0_bi),
			.atk = EV(0_bi),
			.def = EV(196_bi),
			.spa = EV(36_bi),
			.spd = EV(76_bi),
			.spe = EV(196_bi),
		}
	};
	return calculated == expected;
}());

// calculate_ivs_and_evs in generation 4 with Hidden Power
static_assert([]{
	constexpr auto generation = Generation::four;
	constexpr auto species = Species::Roserade;
	constexpr auto level = Level(75_bi);
	constexpr auto stats = Stats<stat_style_for(generation)>{
		.hp = HP(245_bi),
		.atk = 100_bi,
		.def = 131_bi,
		.spa = 236_bi,
		.spd = 191_bi,
		.spe = 185_bi,
	};
	constexpr auto hidden_power = tv::optional(HiddenPower<generation>(70_bi, Type::Dark));

	auto const calculated = calculate_ivs_and_evs(species, level, stats, hidden_power, containers::enum_range(Nature::Modest, Nature::Modest));

	constexpr auto expected = CombinedStatsFor<generation>{
		Nature::Modest,
		IVs{
			.hp = IV(31_bi),
			.atk = IV(3_bi),
			.def = IV(31_bi),
			.spa = IV(31_bi),
			.spd = IV(31_bi),
			.spe = IV(31_bi),
		},
		EVs{
			.hp = EV(252_bi),
			.atk = EV(0_bi),
			.def = EV(108_bi),
			.spa = EV(0_bi),
			.spd = EV(28_bi),
			.spe = EV(116_bi),
		}
	};
	return calculated == expected;
}());

} // namespace technicalmachine
