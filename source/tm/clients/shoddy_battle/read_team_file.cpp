export module tm.clients.sb.read_team_file;

import tm.stat.stat_style;

import tm.initial_team;

import containers;
import std_module;

namespace technicalmachine::sb {

using SBTeam = InitialTeam<SpecialInputStyle::split>;
export auto read_team_file(containers::span<std::byte const>) -> SBTeam;

} // namespace technicalmachine::sb