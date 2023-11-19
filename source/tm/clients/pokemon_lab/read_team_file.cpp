// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.clients.pl.read_team_file;

import tm.generation_generic;
export import tm.team;

import std_module;

namespace technicalmachine::pl {

export auto read_team_file(std::span<std::byte const>) -> GenerationGeneric<KnownTeam>;

} // namespace technicalmachine::pl