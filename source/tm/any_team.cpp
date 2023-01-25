// Copyright David Stone.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.any_team;

import std_module;

namespace technicalmachine {

export template<typename T>
constexpr auto is_known_team = false;

export template<typename T>
concept any_known_team = is_known_team<std::remove_cvref_t<T>>;

export template<typename T>
constexpr auto is_seen_team = false;

export template<typename T>
concept any_seen_team = is_seen_team<std::remove_cvref_t<T>>;

export template<typename T>
constexpr auto is_real_team = false;

export template<typename T>
concept any_real_team = is_real_team<std::remove_cvref_t<T>>;

export template<typename T>
concept any_team = any_known_team<T> or any_seen_team<T> or any_real_team<T>;

} // namespace technicalmachine
