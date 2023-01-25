// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.string_conversions.from_string;

import std_module;

namespace technicalmachine {

export template<typename T>
constexpr auto from_string(std::string_view str) -> T;

} // namespace technicalmachine
