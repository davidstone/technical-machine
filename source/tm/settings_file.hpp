// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <bounded/detail/variant/variant.hpp>

#include <containers/string.hpp>
#include <containers/vector.hpp>

#include <filesystem>

namespace technicalmachine {

struct SettingsFile {
	containers::string host;
	containers::string port;
	containers::string resource;
	containers::string username;
	containers::string password;

	struct NoTeam {};
	struct GenerateTeam {};
	using Team = bounded::variant<NoTeam, GenerateTeam, std::filesystem::path>;
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
	using Style = bounded::variant<Ladder, Challenge, Accept>;
	Style style;
};

auto load_settings_file(std::filesystem::path const & path) -> SettingsFile;

} // namespace technicalmachine
