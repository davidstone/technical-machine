// Copyright David Stone 2021.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <tm/stat/iv.hpp>

#include <bounded/integer.hpp>

#include <containers/algorithms/reverse_iterator.hpp>
#include <containers/static_vector.hpp>
#include <containers/take.hpp>

namespace technicalmachine {
using namespace bounded::literal;

inline constexpr auto max_possible_optimized_ivs = 2_bi;
using PossibleOptimizedIVs = containers::static_vector<IV, max_possible_optimized_ivs.value()>;

constexpr auto possible_optimized_ivs(containers::range auto const ivs) {
	return containers::static_vector<IV, max_possible_optimized_ivs.value()>(containers::take(containers::reversed(ivs), max_possible_optimized_ivs));
};

} // namespace technicalmachine
