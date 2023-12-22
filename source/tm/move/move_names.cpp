// Copyright David Stone 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.move.move_names;

import tm.move.max_moves_per_pokemon;
import tm.move.move_name;

import containers;

namespace technicalmachine {

export using MoveNames = containers::static_vector<MoveName, max_moves_per_pokemon>;

} // namespace technicalmachine
