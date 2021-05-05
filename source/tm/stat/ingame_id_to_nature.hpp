// Copyright David Stone 2021.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <tm/stat/nature.hpp>

#include <bounded/integer.hpp>
#include <bounded/unreachable.hpp>

namespace technicalmachine {

using namespace bounded::literal;

using IngameNatureID = bounded::integer<0, 24>;

constexpr auto ingame_id_to_nature(IngameNatureID const id) -> Nature {
	switch (id.value()) {
		case 0: return Nature::Hardy;
		case 1: return Nature::Lonely;
		case 2: return Nature::Brave;
		case 3: return Nature::Adamant;
		case 4: return Nature::Naughty;
		case 5: return Nature::Bold;
		case 6: return Nature::Docile;
		case 7: return Nature::Relaxed;
		case 8: return Nature::Impish;
		case 9: return Nature::Lax;
		case 10: return Nature::Timid;
		case 11: return Nature::Hasty;
		case 12: return Nature::Serious;
		case 13: return Nature::Jolly;
		case 14: return Nature::Naive;
		case 15: return Nature::Modest;
		case 16: return Nature::Mild;
		case 17: return Nature::Quiet;
		case 18: return Nature::Bashful;
		case 19: return Nature::Rash;
		case 20: return Nature::Calm;
		case 21: return Nature::Gentle;
		case 22: return Nature::Sassy;
		case 23: return Nature::Careful;
		case 24: return Nature::Quirky;
		default: bounded::unreachable();
	}
}

constexpr auto ingame_nature_to_id(Nature const nature) -> IngameNatureID {
	switch (nature) {
		case Nature::Adamant: return 3_bi;
		case Nature::Bashful: return 18_bi;
		case Nature::Bold: return 5_bi;
		case Nature::Brave: return 2_bi;
		case Nature::Calm: return 20_bi;
		case Nature::Careful: return 23_bi;
		case Nature::Docile: return 6_bi;
		case Nature::Gentle: return 21_bi;
		case Nature::Hardy: return 0_bi;
		case Nature::Hasty: return 11_bi;
		case Nature::Impish: return 8_bi;
		case Nature::Jolly: return 13_bi;
		case Nature::Lax: return 9_bi;
		case Nature::Lonely: return 1_bi;
		case Nature::Mild: return 16_bi;
		case Nature::Modest: return 15_bi;
		case Nature::Naive: return 14_bi;
		case Nature::Naughty: return 4_bi;
		case Nature::Quiet: return 17_bi;
		case Nature::Quirky: return 24_bi;
		case Nature::Rash: return 19_bi;
		case Nature::Relaxed: return 7_bi;
		case Nature::Sassy: return 22_bi;
		case Nature::Serious: return 12_bi;
		case Nature::Timid: return 10_bi;
	}
}

} // namespace technicalmachine
