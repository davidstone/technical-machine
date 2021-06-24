// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <tm/pokemon/pokemon.hpp>

namespace technicalmachine {

template struct Pokemon<Generation::one>;
template struct Pokemon<Generation::two>;
template struct Pokemon<Generation::three>;
template struct Pokemon<Generation::four>;
template struct Pokemon<Generation::five>;
template struct Pokemon<Generation::six>;
template struct Pokemon<Generation::seven>;
template struct Pokemon<Generation::eight>;

} // namespace technicalmachine