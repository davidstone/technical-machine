// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <tm/clients/pokemon_showdown/inmessage.hpp>

#include <tm/stat/hp.hpp>

#include <tm/visible_hp.hpp>

#include <bounded/integer.hpp>
#include <bounded/to_integer.hpp>

#include <stdexcept>

namespace technicalmachine {
namespace ps {

constexpr auto parse_hp(std::string_view const str) {
	auto const hp = split_view(str, '/');
	auto const current = bounded::to_integer<HP::current_type>(hp.first);
	auto const max = hp.second.empty() ? 100_bi : bounded::to_integer<HP::max_type>(hp.second);
	if (current > max) {
		throw std::runtime_error("Received a current HP greater than max HP");
	}
	return VisibleHP{current, max};
}

}	// namespace ps
}	// namespace technicalmachine
