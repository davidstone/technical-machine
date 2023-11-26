// Copyright David Stone 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.test.move.actual_damage;

import tm.move.actual_damage;

import bounded;

namespace technicalmachine {
namespace {
using namespace bounded::literal;

static_assert(!ActualDamage(ActualDamage::Known(0_bi)).did_any_damage());
static_assert(ActualDamage(ActualDamage::Known(3_bi)).did_any_damage());

} // namespace
} // namespace technicalmachine
