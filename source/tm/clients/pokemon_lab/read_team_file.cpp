export module tm.clients.pl.read_team_file;

import tm.initial_team;

import containers;
import std_module;

namespace technicalmachine::pl {

export auto read_team_file(containers::span<std::byte const>) -> AnyInitialTeam;

} // namespace technicalmachine::pl