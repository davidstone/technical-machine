export module tm.move.move_names;

import tm.move.max_moves_per_pokemon;
import tm.move.move_name;

import containers;

namespace technicalmachine {

export using MoveNames = containers::static_vector<MoveName, max_moves_per_pokemon>;

} // namespace technicalmachine
