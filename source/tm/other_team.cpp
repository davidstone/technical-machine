export module tm.other_team;

import tm.any_team;

namespace technicalmachine {

export template<typename T>
struct OtherTeamImpl;

export template<any_team T>
using OtherTeam = typename OtherTeamImpl<T>::type;

} // namespace technicalmachine
