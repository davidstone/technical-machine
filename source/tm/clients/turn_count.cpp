export module tm.clients.turn_count;

import bounded;

namespace technicalmachine {

export using TurnCount = bounded::integer<0, 1'000'000'000>;

} // namespace technicalmachine
