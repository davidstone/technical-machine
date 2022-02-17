// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <ostream>
#include <string_view>

namespace technicalmachine {

void generate_team_builder_ui(std::ostream & output, std::string_view query_string, std::string_view generated);

} // namespace technicalmachine
