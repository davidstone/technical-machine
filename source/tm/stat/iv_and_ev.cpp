// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.stat.iv_and_ev;

import tm.stat.ev;
import tm.stat.iv;

import bounded;

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

template<>
struct bounded::tombstone_traits<technicalmachine::IVAndEV>  {
	static constexpr auto spare_representations = tombstone_traits<technicalmachine::IV>::spare_representations;

	static constexpr auto make(auto const index) noexcept -> technicalmachine::IVAndEV {
		return technicalmachine::IVAndEV(
			tombstone_traits<technicalmachine::IV>::make(index),
			technicalmachine::EV()
		);
	}
	static constexpr auto index(technicalmachine::IVAndEV const & value) noexcept {
		return tombstone_traits<technicalmachine::IV>::index(value.iv);
	}
};

template<>
struct bounded::tombstone_traits<technicalmachine::DVAndEV>  {
	static constexpr auto spare_representations = tombstone_traits<technicalmachine::DV>::spare_representations;

	static constexpr auto make(auto const index) noexcept -> technicalmachine::DVAndEV {
		return technicalmachine::DVAndEV(
			tombstone_traits<technicalmachine::DV>::make(index),
			technicalmachine::EV()
		);
	}
	static constexpr auto index(technicalmachine::DVAndEV const & value) noexcept {
		return tombstone_traits<technicalmachine::DV>::index(value.dv);
	}
};
