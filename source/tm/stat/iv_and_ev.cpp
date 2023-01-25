// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.stat.iv_and_ev;

import tm.stat.ev;
import tm.stat.iv;

namespace technicalmachine {

export struct IVAndEV {
	IV iv;
	EV ev;

	friend auto operator==(IVAndEV, IVAndEV) -> bool = default;
};

export struct DVAndEV {
	DV dv;
	EV ev;

	friend auto operator==(DVAndEV, DVAndEV) -> bool = default;
};

} // namespace technicalmachine
