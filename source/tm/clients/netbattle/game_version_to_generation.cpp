// Copyright David Stone 2024.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.clients.nb.game_version_to_generation;

import tm.generation;

import bounded;
import std_module;

namespace technicalmachine::nb {

export using GameVersion = bounded::integer<0, 8>;
export constexpr auto game_version_to_generation(GameVersion const game_version) -> Generation {
	switch (game_version.value()) {
		case 0: return Generation::one; // "RBY with Trades"
		case 1: return Generation::two; // "GSC with Trades"
		case 2: return Generation::three; // "Ru/Sa Only"
		case 3: return Generation::three; // "Full Advance"
		case 4: return Generation::three; // "Full Advance + XD Moves"
		case 5: return Generation::one; // "True RBY"
		case 6: return Generation::two; // "True GSC"
		case 7: return Generation::four; // "True Diamond + Pearl"
		case 8: return Generation::four; // "Full Generation 4"
		default: std::unreachable();
	}
}

} // namespace technicalmachine::nb