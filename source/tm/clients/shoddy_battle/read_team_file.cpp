// Copyright David Stone 2021.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.clients.sb.read_team_file;

import tm.generation_generic;
import tm.team;

import std_module;

namespace technicalmachine::sb {

export auto read_team_file(std::span<std::byte const>) -> GenerationGeneric<KnownTeam>;

} // namespace technicalmachine::sb