// Copyright David Stone 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.pokemon.hp_ratio;

import tm.rational;

namespace technicalmachine {

export constexpr auto hp_ratio(auto const pokemon) {
	auto const hp = pokemon.hp();
	return rational(hp.current(), hp.max());
}

} // namespace technicalmachine