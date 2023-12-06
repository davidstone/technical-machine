// Copyright David Stone 2022.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.clients.make_client_battle;

import tm.clients.client_battle;
import tm.clients.teams;

import tm.generation_generic;

import std_module;

namespace technicalmachine {

export auto make_client_battle(GenerationGeneric<Teams> teams) -> std::unique_ptr<ClientBattle>;

} // namespace technicalmachine
