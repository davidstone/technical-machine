// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.test.team_predictor.ev_optimizer.pad_evs;

import tm.stat.ev;
import tm.stat.evs;

import tm.team_predictor.ev_optimizer.pad_evs;

import bounded;

namespace technicalmachine {
namespace {
using namespace bounded::literal;

constexpr auto full_old_gen_evs = OldGenEVs(
	EV(252_bi),
	EV(252_bi),
	EV(252_bi),
	EV(252_bi),
	EV(252_bi)
);

constexpr auto no_atk_old_gen_evs = OldGenEVs(
	EV(252_bi),
	EV(0_bi),
	EV(252_bi),
	EV(252_bi),
	EV(252_bi)
);

constexpr auto empty_old_gen_evs = OldGenEVs(
	EV(0_bi),
	EV(0_bi),
	EV(0_bi),
	EV(0_bi),
	EV(0_bi)
);

static_assert(
	pad_evs(full_old_gen_evs, true, true) ==
	full_old_gen_evs
);

static_assert(
	pad_evs(full_old_gen_evs, false, true) ==
	no_atk_old_gen_evs
);

static_assert(
	pad_evs(empty_old_gen_evs, true, true) ==
	full_old_gen_evs
);

static_assert(
	pad_evs(empty_old_gen_evs, false, true) ==
	no_atk_old_gen_evs
);


constexpr auto full_evs = EVs(
	EV(88_bi),
	EV(84_bi),
	EV(84_bi),
	EV(84_bi),
	EV(84_bi),
	EV(84_bi)
);

static_assert(
	pad_evs(full_evs, true, true) ==
	full_evs
);
static_assert(
	pad_evs(full_evs, false, false) ==
	full_evs
);

constexpr auto empty_evs = EVs(
	EV(0_bi),
	EV(0_bi),
	EV(0_bi),
	EV(0_bi),
	EV(0_bi),
	EV(0_bi)
);
static_assert(
	pad_evs(empty_evs, true, true) ==
	EVs(
		EV(84_bi),
		EV(84_bi),
		EV(84_bi),
		EV(84_bi),
		EV(84_bi),
		EV(88_bi)
	)
);
static_assert(
	pad_evs(empty_evs, false, false) ==
	EVs(
		EV(128_bi),
		EV(0_bi),
		EV(128_bi),
		EV(0_bi),
		EV(124_bi),
		EV(128_bi)
	)
);

static_assert(
	pad_evs(
		EVs(
			EV(252_bi),
			EV(0_bi),
			EV(0_bi),
			EV(0_bi),
			EV(0_bi),
			EV(0_bi)
		),
		true,
		false
	) ==
	EVs(
		EV(252_bi),
		EV(64_bi),
		EV(64_bi),
		EV(0_bi),
		EV(64_bi),
		EV(64_bi)
	)
);

static_assert(
	pad_evs(
		EVs(
			EV(4_bi),
			EV(0_bi),
			EV(0_bi),
			EV(0_bi),
			EV(0_bi),
			EV(0_bi)
		),
		true,
		false
	) ==
	EVs(
		EV(252_bi),
		EV(64_bi),
		EV(64_bi),
		EV(0_bi),
		EV(64_bi),
		EV(64_bi)
	)
);

static_assert(
	pad_evs(
		EVs(
			EV(252_bi),
			EV(252_bi),
			EV(0_bi),
			EV(0_bi),
			EV(0_bi),
			EV(0_bi)
		),
		true,
		false
	) ==
	EVs(
		EV(252_bi),
		EV(252_bi),
		EV(0_bi),
		EV(0_bi),
		EV(0_bi),
		EV(4_bi)
	)
);

static_assert(
	pad_evs(
		EVs(
			EV(252_bi),
			EV(0_bi),
			EV(252_bi),
			EV(0_bi),
			EV(0_bi),
			EV(0_bi)
		),
		false,
		false
	) ==
	EVs(
		EV(252_bi),
		EV(0_bi),
		EV(252_bi),
		EV(0_bi),
		EV(0_bi),
		EV(4_bi)
	)
);

static_assert(
	pad_evs(
		EVs(
			EV(8_bi),
			EV(8_bi),
			EV(8_bi),
			EV(0_bi),
			EV(8_bi),
			EV(8_bi)
		),
		true,
		false
	) ==
	EVs(
		EV(104_bi),
		EV(100_bi),
		EV(100_bi),
		EV(0_bi),
		EV(100_bi),
		EV(104_bi)
	)
);

} // namespace
} // namespace technicalmachine
