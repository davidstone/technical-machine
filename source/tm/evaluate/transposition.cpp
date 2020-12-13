// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <tm/evaluate/transposition.hpp>

namespace technicalmachine {

template struct TranspositionTable<Generation::one>;
template struct TranspositionTable<Generation::two>;
template struct TranspositionTable<Generation::three>;
template struct TranspositionTable<Generation::four>;
template struct TranspositionTable<Generation::five>;
template struct TranspositionTable<Generation::six>;
template struct TranspositionTable<Generation::seven>;
template struct TranspositionTable<Generation::eight>;

} // namespace technicalmachine