export module tm.move.damage_type;

import bounded;

namespace technicalmachine {

export using damage_type = bounded::integer<0, 1'000'000'000>;

} // namespace technicalmachine
