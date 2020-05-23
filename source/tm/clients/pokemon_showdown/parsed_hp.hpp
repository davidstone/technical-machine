// Copyright (C) 2019 David Stone
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

#include <tm/clients/pokemon_showdown/inmessage.hpp>

#include <tm/stat/hp.hpp>

#include <bounded/integer.hpp>
#include <bounded/to_integer.hpp>

#include <stdexcept>

namespace technicalmachine {
namespace ps {

struct ParsedHP {
public:
	constexpr explicit ParsedHP(std::string_view const str):
		ParsedHP(split_view(str, '/'))
	{
	}
	
	HP::current_type current;
	HP::max_type max;
private:
	constexpr explicit ParsedHP(std::pair<std::string_view, std::string_view> const hp):
		current(bounded::to_integer<HP::current_type>(hp.first)),
		max(hp.second.empty() ? 100_bi : bounded::to_integer<HP::max_type>(hp.second))
	{
		if (current > max) {
			throw std::runtime_error("Received a current HP greater than max HP");
		}
	}
};

}	// namespace ps
}	// namespace technicalmachine
