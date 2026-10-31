export module tm.usage_for;

import bounded;
import containers;

namespace technicalmachine {

export template<typename Key, typename Mapped = double>
using UsageFor = containers::array<Mapped, bounded::number_of<Key>>;

} // namespace technicalmachine
