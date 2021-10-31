// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <tm/string_conversions/generation.hpp>

#include <tm/string_conversions/conversion.hpp>
#include <tm/string_conversions/invalid_string_conversion.hpp>
#include <tm/string_conversions/lowercase_alphanumeric.hpp>

#include <tm/gender.hpp>

#include <containers/array.hpp>
#include <containers/begin_end.hpp>
#include <containers/flat_map.hpp>
#include <containers/lookup.hpp>

namespace technicalmachine {

auto to_string(Generation const generation) -> std::string_view {
	switch (generation) {
		case Generation::one: return "1";
		case Generation::two: return "2";
		case Generation::three: return "3";
		case Generation::four: return "4";
		case Generation::five: return "5";
		case Generation::six: return "6";
		case Generation::seven: return "7";
		case Generation::eight: return "8";
	}
}

template<>
auto from_string(std::string_view const str) -> Generation {
	using Storage = containers::array<containers::map_value_type<std::string_view, Generation>, 8>;
	constexpr auto converter = containers::basic_flat_map<Storage>(
		containers::assume_sorted_unique,
		Storage{{
			{ "1", Generation::one },
			{ "2", Generation::two },
			{ "3", Generation::three },
			{ "4", Generation::four },
			{ "5", Generation::five },
			{ "6", Generation::six },
			{ "7", Generation::seven },
			{ "8", Generation::eight },
		}}
	);
	auto const result = containers::lookup(converter, str);
	if (!result) {
		throw InvalidFromStringConversion("Generation", str);
	}
	return *result;
}

}	// namespace technicalmachine
