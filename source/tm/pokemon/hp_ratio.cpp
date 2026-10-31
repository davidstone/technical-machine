export module tm.pokemon.hp_ratio;

import tm.rational;

namespace technicalmachine {

export constexpr auto hp_ratio(auto const pokemon) {
	auto const hp = pokemon.hp();
	return rational(hp.current(), hp.max());
}

} // namespace technicalmachine