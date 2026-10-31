export module tm.clients.make_client_battle;

import tm.clients.client_battle;
import tm.clients.teams;

import tm.generation_generic;

import std_module;

namespace technicalmachine {

export auto make_client_battle(GenerationGeneric<Teams> teams) -> std::unique_ptr<ClientBattle>;

} // namespace technicalmachine
