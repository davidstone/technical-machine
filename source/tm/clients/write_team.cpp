// Copyright David Stone 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.clients.write_team;

import tm.generation_generic;
import tm.team;

import containers;
import std_module;

namespace technicalmachine {

using WriteSignature = void(GenerationGeneric<Team> const &, std::filesystem::path const &) const;
export struct WriteTeam {
	containers::string extension;
	containers::trivial_inplace_function<WriteSignature, 0> function;
};

} // namespace technicalmachine
