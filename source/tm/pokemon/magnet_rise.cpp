export module tm.pokemon.magnet_rise;

import tm.pokemon.end_of_turn_counter;

import tm.generation;

namespace technicalmachine {

export template<Generation generation>
using MagnetRise = EndOfTurnCounter<
	generation >= Generation::four,
	5
>;

} // namespace technicalmachine
