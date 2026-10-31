export module tm.pokemon.heal_block;

import tm.pokemon.end_of_turn_counter;

import tm.generation;

namespace technicalmachine {

export template<Generation generation>
using HealBlock = EndOfTurnCounter<
	generation >= Generation::four,
	5
>;

} // namespace technicalmachine
