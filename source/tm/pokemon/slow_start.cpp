export module tm.pokemon.slow_start;

import tm.pokemon.end_of_turn_counter;

import tm.generation;

namespace technicalmachine {

export template<Generation generation>
using SlowStart = EndOfTurnCounter<
	generation >= Generation::four,
	5
>;

} // namespace technicalmachine
