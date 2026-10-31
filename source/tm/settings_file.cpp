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
		containers::dynamic_array<containers::string> users;
	};
	using Style = tv::variant<Ladder, Challenge, Accept>;
	Style style;
};

} // namespace technicalmachine
