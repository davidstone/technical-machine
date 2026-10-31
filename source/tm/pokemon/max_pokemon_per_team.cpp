export module tm.pokemon.max_pokemon_per_team;

import bounded;

namespace technicalmachine {
using namespace bounded::literal;

export constexpr auto max_pokemon_per_team = 6_bi;
export using TeamSize = bounded::integer<
	0,
	bounded::normalize<max_pokemon_per_team>
>;
export using TeamIndex = bounded::integer<
	0,
	bounded::normalize<max_pokemon_per_team - 1_bi>
>;

} // namespace technicalmachine
