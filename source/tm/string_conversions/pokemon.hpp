// Copyright (C) 2020 David Stone
//
// This file is part of Technical Machine.
//
// Technical Machine is free software: you can redistribute it and / or modify
// it under the terms of the GNU Affero General Public License as
// published by the Free Software Foundation, either version 3 of the
// License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.

#pragma once

#include <tm/generation.hpp>
#include <tm/pokemon/pokemon.hpp>

#include <containers/string.hpp>

#include <string_view>

namespace technicalmachine {

auto to_string(Generation, Pokemon pokemon) -> containers::string;

auto pokemon_from_string(Generation, std::string_view str) -> Pokemon;

} // namespace technicalmachine
