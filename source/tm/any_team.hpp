// Copyright David Stone.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <tm/generation.hpp>

#include <type_traits>

namespace technicalmachine {

template<Generation generation>
struct Team;

template<typename T>
inline constexpr auto is_team_impl = false;

template<Generation generation>
inline constexpr auto is_team_impl<Team<generation>> = true;

template<typename T>
concept any_team = is_team_impl<std::remove_cvref_t<T>>;

} // namespace technicalmachine
