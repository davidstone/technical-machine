// IVs
// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <tm/stat/iv.hpp>

namespace technicalmachine {

using namespace bounded::literal;

static_assert(DVs(DV(15_bi), DV(15_bi), DV(15_bi), DV(15_bi)).hp() == DV(15_bi));
static_assert(DVs(DV(15_bi), DV(14_bi), DV(15_bi), DV(15_bi)).hp() == DV(11_bi));
static_assert(DVs(DV(15_bi), DV(13_bi), DV(15_bi), DV(15_bi)).hp() == DV(15_bi));
static_assert(DVs(DV(14_bi), DV(14_bi), DV(14_bi), DV(14_bi)).hp() == DV(0_bi));

} // namespace technicalmachine