// Copyright David Stone 2021.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.other_team;

import tm.any_team;

namespace technicalmachine {

export template<typename T>
struct OtherTeamImpl;

export template<any_team T>
using OtherTeam = typename OtherTeamImpl<T>::type;

} // namespace technicalmachine
