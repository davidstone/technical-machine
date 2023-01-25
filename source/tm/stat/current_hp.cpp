// Copyright David Stone 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.stat.current_hp;

import tm.stat.max_hp;

import bounded;

namespace technicalmachine {

export using CurrentHP = bounded::integer<0, bounded::builtin_max_value<MaxHP>>;

} // namespace technicalmachine