export module tm.clients.ps.battle_response_switch;

import tm.pokemon.max_pokemon_per_team;

import bounded;

namespace technicalmachine::ps {

export using BattleResponseSwitch = bounded::integer<1, bounded::normalize<max_pokemon_per_team>>;

} // namespace technicalmachine::ps
