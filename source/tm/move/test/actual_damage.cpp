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
