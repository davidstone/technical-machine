export module tm.to_index;

import bounded;
import numeric_traits;

namespace technicalmachine {

export template<typename T>
constexpr auto to_index(T const value) {
	return bounded::integer(value) - bounded::constant<numeric_traits::min_value<T>>;
}

} // namespace technicalmachine
