module;

#include <bounded/conditional.hpp>

export module tm.pokemon.get_hidden_power_type;

import tv;

namespace technicalmachine {

export constexpr auto get_hidden_power_type(auto const pokemon) {
	auto const hidden_power = pokemon.hidden_power();
	return BOUNDED_CONDITIONAL(hidden_power, hidden_power->type(), tv::none);
}

} // namespace technicalmachine