// Copyright David Stone 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.status.toxic_resets_on_switch;

import tm.generation;

namespace technicalmachine {

export constexpr auto toxic_resets_on_switch(Generation const generation) -> bool {
	switch (generation) {
		case Generation::one:
		case Generation::two:
			return true;
		default:
			return false;
	}
}

} // namespace technicalmachine
