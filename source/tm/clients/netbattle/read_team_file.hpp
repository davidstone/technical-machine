// Copyright David Stone 2021.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <tm/generation_generic.hpp>
#include <tm/team.hpp>

#include <filesystem>

namespace technicalmachine::nb {

auto read_team_file(std::filesystem::path const & team_file) -> GenerationGeneric<Team>;

} // namespace technicalmachine::nb