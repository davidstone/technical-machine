// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <tm/stat/ev.hpp>
#include <tm/stat/iv.hpp>

namespace technicalmachine {

struct IVAndEV {
	IV iv;
	EV ev;

	friend auto operator==(IVAndEV, IVAndEV) -> bool = default;
};

} // namespace technicalmachine
