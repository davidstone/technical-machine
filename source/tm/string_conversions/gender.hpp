// Gender string conversions
// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <tm/string_conversions/conversion.hpp>

#include <tm/gender.hpp>

namespace technicalmachine {

auto to_string(Gender const gender) -> std::string_view;

template<>
auto from_string<Gender>(std::string_view const str) -> Gender;

} // namespace technicalmachine
