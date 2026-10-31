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
