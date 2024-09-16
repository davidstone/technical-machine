// Copyright David Stone 2021.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.usage_for;

import bounded;
import containers;

namespace technicalmachine {

export template<typename Key, typename Mapped = double>
using UsageFor = containers::array<Mapped, bounded::number_of<Key>>;

} // namespace technicalmachine
