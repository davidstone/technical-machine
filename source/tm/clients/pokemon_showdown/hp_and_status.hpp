// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <tm/status_name.hpp>
#include <tm/visible_hp.hpp>

namespace technicalmachine {
namespace ps {

struct HPAndStatus {
	VisibleHP hp;
	StatusName status;
};

} // namespace ps
} // namespace technicalmachine
