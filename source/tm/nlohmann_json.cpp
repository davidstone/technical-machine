// Copyright David Stone 2022.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

module;

#include <std_module/prelude.hpp>
#include <variant>

#include <nlohmann/json.hpp>

export module tm.nlohmann_json;

namespace technicalmachine::nlohmann {

export using ::nlohmann::json;

} // namespace technicalmachine::nlohmann
