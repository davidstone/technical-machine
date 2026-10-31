export module tm.stat.current_hp;

import tm.stat.max_hp;

import bounded;

namespace technicalmachine {

export using CurrentHP = bounded::integer<0, bounded::builtin_max_value<MaxHP>>;

} // namespace technicalmachine