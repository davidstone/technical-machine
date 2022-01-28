// Copyright David Stone 2021.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <tm/string_conversions/weather.hpp>

#include <tm/string_conversions/conversion.hpp>
#include <tm/string_conversions/invalid_string_conversion.hpp>
#include <tm/string_conversions/lowercase_alphanumeric.hpp>

#include <containers/array.hpp>
#include <containers/begin_end.hpp>
#include <containers/flat_map.hpp>
#include <containers/lookup.hpp>

namespace technicalmachine {

auto to_string(NormalWeather const weather) -> std::string_view {
	switch (weather) {
		case NormalWeather::clear: return "Clear";
		case NormalWeather::hail: return "Hail";
		case NormalWeather::sand: return "Sand";
		case NormalWeather::sun: return "Sun";
		case NormalWeather::rain: return "Rain";
	}
}

template<>
auto from_string(std::string_view const str) -> NormalWeather {
	static constexpr auto converter = containers::basic_flat_map(
		containers::assume_sorted_unique,
		containers::to_array<containers::map_value_type<std::string_view, NormalWeather>>({
			{ "clear", NormalWeather::clear },
			{ "hail", NormalWeather::hail },
			{ "none", NormalWeather::clear },
			{ "rain", NormalWeather::rain },
			{ "raindance", NormalWeather::rain },
			{ "sand", NormalWeather::sand },
			{ "sandstorm", NormalWeather::sand },
			{ "sun", NormalWeather::sun },
			{ "sunnyday", NormalWeather::sun },
		})
	);
	auto const converted = fixed_capacity_lowercase_and_digit_string<9_bi>(str);
	auto const result = containers::lookup(converter, converted);
	if (!result) {
		throw InvalidFromStringConversion("NormalWeather", str);
	}
	return *result;
}

} // namespace technicalmachine
