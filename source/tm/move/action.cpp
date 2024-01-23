// Copyright David Stone 2024.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.move.action;

import tm.move.move_name;
import tm.move.switch_;

import tv;

namespace technicalmachine {

export using Action = tv::variant<MoveName, Switch>;

} // namespace technicalmachine
