// Copyright (C) 2020 David Stone
//
// This file is part of Technical Machine.
//
// Technical Machine is free software: you can redistribute it and / or modify
// it under the terms of the GNU Affero General Public License as
// published by the Free Software Foundation, either version 3 of the
// License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.

#include <tm/string_conversions/generation.hpp>

#include <tm/string_conversions/conversion.hpp>
#include <tm/string_conversions/invalid_string_conversion.hpp>
#include <tm/string_conversions/lowercase_alphanumeric.hpp>

#include <tm/gender.hpp>

#include <containers/array/array.hpp>
#include <containers/flat_map.hpp>

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
	auto const it = converter.find(str);
	if (it != end(converter)) {
		return it->mapped();
	} else {
		throw InvalidFromStringConversion("Generation", str);
	}
}

}	// namespace technicalmachine
