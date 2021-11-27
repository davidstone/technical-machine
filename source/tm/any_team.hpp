// Copyright David Stone.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <tm/generation.hpp>

#include <type_traits>

namespace technicalmachine {

template<Generation generation>
struct KnownTeam;
template<Generation generation>
struct SeenTeam;
template<Generation generation>
struct Team;

template<typename T>
inline constexpr auto is_known_team_impl = false;

template<Generation generation>
inline constexpr auto is_known_team_impl<KnownTeam<generation>> = true;

template<typename T>
concept any_known_team = is_known_team_impl<std::remove_cvref_t<T>>;

template<typename T>
inline constexpr auto is_seen_team_impl = false;

template<Generation generation>
inline constexpr auto is_seen_team_impl<SeenTeam<generation>> = true;

template<typename T>
concept any_seen_team = is_seen_team_impl<std::remove_cvref_t<T>>;

template<typename T>
inline constexpr auto is_real_team_impl = false;

template<Generation generation>
inline constexpr auto is_real_team_impl<Team<generation>> = true;

template<typename T>
concept any_real_team = is_real_team_impl<std::remove_cvref_t<T>>;

template<typename T>
concept any_team = any_known_team<T> or any_seen_team<T> or any_real_team<T>;

} // namespace technicalmachine
