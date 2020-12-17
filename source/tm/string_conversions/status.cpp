// Status string functions
// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <tm/string_conversions/status.hpp>
#include <tm/string_conversions/invalid_string_conversion.hpp>
#include <tm/string_conversions/lowercase_alphanumeric.hpp>

#include <tm/status.hpp>

#include <containers/array/array.hpp>
#include <containers/begin_end.hpp>
#include <containers/flat_map.hpp>
#include <containers/lookup.hpp>

namespace technicalmachine {

auto to_string(Statuses const status) -> std::string_view {
	switch (status) {
		case Statuses::clear: return "No status";
		case Statuses::burn: return "Burn";
		case Statuses::freeze: return "Freeze";
		case Statuses::paralysis: return "Paralysis";
		case Statuses::poison: return "Poison";
		case Statuses::toxic: return "Toxic";
		case Statuses::sleep: return "Sleep";
		case Statuses::rest: return "Rest";
	}
}

template<>
auto from_string(std::string_view const str) -> Statuses {
	using Storage = containers::array<containers::map_value_type<std::string_view, Statuses>, 8>;
	static constexpr auto converter = containers::basic_flat_map<Storage>(
		containers::assume_sorted_unique,
		Storage{{
			{ "burn", Statuses::burn },
			{ "freeze", Statuses::freeze },
			{ "nostatus", Statuses::clear },
			{ "paralysis", Statuses::paralysis },
			{ "poison", Statuses::poison },
			{ "rest", Statuses::rest },
			{ "sleep", Statuses::sleep },
			{ "toxic", Statuses::toxic },
		}}
	);
	auto const converted = fixed_capacity_lowercase_and_digit_string<9>(str);
	auto const result = containers::lookup(converter, converted);
	if (!result) {
		throw InvalidFromStringConversion("Status", str);
	}
	return *result;
}

}	// namespace technicalmachine
