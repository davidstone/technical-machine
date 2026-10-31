export module tm.associated_team;

import tm.pokemon.active_pokemon;
import tm.pokemon.any_pokemon;

import tm.generation;
import tm.team;

namespace technicalmachine {

template<typename T>
struct AssociatedTeamImpl {
};

template<any_known_pokemon T>
struct AssociatedTeamImpl<AnyActivePokemon<T>> {
	using type = KnownTeam<generation_from<T>>;
};
template<any_seen_pokemon T>
struct AssociatedTeamImpl<AnyActivePokemon<T>> {
	using type = SeenTeam<generation_from<T>>;
};
template<any_real_pokemon T>
struct AssociatedTeamImpl<AnyActivePokemon<T>> {
	using type = Team<generation_from<T>>;
};

export template<typename T>
using AssociatedTeam = typename AssociatedTeamImpl<T>::type;

} // namespace technicalmachine
