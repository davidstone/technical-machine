// Copyright David Stone 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.settings_file;

import containers;
import tv;
import std_module;

namespace technicalmachine {

export struct SettingsFile {
	containers::string host;
	containers::string port;
	containers::string resource;
	containers::string username;
	containers::string password;

	struct NoTeam {};
	struct GenerateTeam {};
	using Team = tv::variant<NoTeam, GenerateTeam, std::filesystem::path>;
	Team team;

	struct Ladder {
		containers::string format;
	};
	struct Challenge {
		containers::string user;
		containers::string format;
	};
	struct Accept {
		// TODO: How does this line up with allowed teams?
		containers::vector<containers::string> users;
	};
	using Style = tv::variant<Ladder, Challenge, Accept>;
	Style style;
};

} // namespace technicalmachine
