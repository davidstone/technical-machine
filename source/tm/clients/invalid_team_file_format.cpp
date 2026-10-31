export module tm.clients.invalid_team_file_format;

import containers;
import std_module;

namespace technicalmachine {
using namespace containers::string_literals;

export struct InvalidTeamFileFormat : std::runtime_error {
	explicit InvalidTeamFileFormat(std::filesystem::path const & file):
		std::runtime_error(containers::concatenate<std::string>("Attempted to load "_sv, file.string(), "."_sv))
	{
	}
};

} // namespace technicalmachine
