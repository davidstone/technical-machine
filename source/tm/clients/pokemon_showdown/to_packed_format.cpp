export module tm.clients.ps.to_packed_format;

import tm.generation_generic;
import tm.team;

import containers;

// https://github.com/smogon/pokemon-showdown/blob/master/sim/TEAMS.md#packed-format

namespace technicalmachine::ps {

export auto to_packed_format(GenerationGeneric<KnownTeam> const & team) -> containers::string;

} // namespace technicalmachine::ps
