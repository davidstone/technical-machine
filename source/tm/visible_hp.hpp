// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <tm/stat/hp.hpp>

namespace technicalmachine {

// For the AI's Pokemon, this is exact. For the foe's Pokemon, this is a rounded
// fraction out of 42 or 100.
struct VisibleHP {
	HP::current_type current;
	HP::max_type max;
};

struct AllowedHP {
	HP::current_type min;
	HP::current_type value;
	HP::current_type max;
};

auto to_real_hp(bool is_ai, HP, VisibleHP) -> AllowedHP;

} // namespace technicalmachine
