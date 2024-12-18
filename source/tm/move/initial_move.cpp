// Copyright David Stone 2024.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.move.initial_move;

import tm.move.max_moves_per_pokemon;
import tm.move.move_name;
import tm.move.pp;

import bounded;
import containers;
import numeric_traits;

namespace technicalmachine {

export struct InitialMove {
	// Intentionally implicit
	constexpr InitialMove(
		MoveName const name_,
		PP::pp_ups_type const pp_ups_ = numeric_traits::max_value<PP::pp_ups_type>
	):
		name(name_),
		pp_ups(pp_ups_)
	{
	}
	MoveName name;
	PP::pp_ups_type pp_ups;
	friend auto operator==(InitialMove, InitialMove) -> bool = default;

private:
	constexpr explicit InitialMove(bounded::tombstone_tag, auto const make) noexcept:
		name(),
		pp_ups(make())
	{
	}
	friend bounded::tombstone_member<&InitialMove::pp_ups>;
};

} // namespace technicalmachine

template<>
struct bounded::tombstone<technicalmachine::InitialMove> : bounded::tombstone_member<&technicalmachine::InitialMove::pp_ups> {
};

namespace technicalmachine {

export using InitialMoves = containers::static_vector<InitialMove, max_moves_per_pokemon>;

} // namespace technicalmachine
