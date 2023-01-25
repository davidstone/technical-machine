// Copyright David Stone 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.clients.should_accept_challenge;

import tm.settings_file;

import containers;
import tv;

namespace technicalmachine {

export constexpr auto should_accept_challenge(SettingsFile::Style const & style, auto const & username) {
	return tv::visit(style, tv::overload(
		[](SettingsFile::Ladder const &) { return false; },
		[](SettingsFile::Challenge const &) { return false; },
		[&](SettingsFile::Accept const & accept) { return containers::any_equal(accept.users, username); }
	));
}

} // namespace technicalmachine
