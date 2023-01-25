// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.clients.ps.parse_hp;

import tm.clients.ps.inmessage;

import tm.stat.current_hp;
import tm.stat.max_hp;

import tm.buffer_view;
import tm.visible_hp;

import bounded;
import std_module;

namespace technicalmachine::ps {
using namespace bounded::literal;

export constexpr auto parse_hp(std::string_view const str) {
	auto const hp = split_view(str, '/');
	auto const current = CurrentVisibleHP(bounded::to_integer<CurrentHP>(hp.first));
	auto const max = MaxVisibleHP(hp.second.empty() ? 100_bi : bounded::to_integer<MaxHP>(hp.second));
	if (current > max) {
		throw std::runtime_error("Received a current HP greater than max HP");
	}
	return VisibleHP{current, max};
}

} // namespace technicalmachine::ps
