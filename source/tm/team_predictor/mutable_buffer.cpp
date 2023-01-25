// Copyright David Stone 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.team_predictor.mutable_buffer;

import bounded;
import containers;

namespace technicalmachine {

export using mutable_buffer = decltype(containers::range_view(
	bounded::declval<char *>(),
	bounded::declval<containers::array_size_type<char>>()
));

} // namespace technicalmachine
