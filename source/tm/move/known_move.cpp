// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.move.known_move;

import tm.move.move_name;
import tm.type.type;

namespace technicalmachine {

export struct KnownMove {
	MoveName name;
	Type type;
};

} // namespace technicalmachine
