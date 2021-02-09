// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <tm/string_conversions/conversion.hpp>

#include <tm/generation.hpp>

namespace technicalmachine {

auto to_string(Generation) -> std::string_view;

template<>
auto from_string<Generation>(std::string_view) -> Generation;

} // namespace technicalmachine
