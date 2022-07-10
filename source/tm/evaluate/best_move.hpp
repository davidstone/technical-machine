// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <tm/move/move_name.hpp>

namespace technicalmachine {

struct BestMove {
	MoveName name;
	double score;
};

} // namespace technicalmachine
