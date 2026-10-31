export module tm.evaluate.victory;

import tm.evaluate.evaluate;
import tm.evaluate.score;

import tm.generation;
import tm.team;

import bounded;
import numeric_traits;

namespace technicalmachine {
using namespace bounded::literal;

// 100% chance to win
export template<Generation generation>
constexpr auto victory = Score(double(
	numeric_traits::max_value<decltype(
		bounded::declval<Evaluate<generation>>()(
			bounded::declval<Team<generation>>(),
			bounded::declval<Team<generation>>()
		)
	)> + 1_bi
));

} // namespace technicalmachine
