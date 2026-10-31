export module tm.ps_usage_stats.files_in_directory;

import containers;
import std_module;

namespace technicalmachine::ps_usage_stats {

constexpr auto not_directory = [](std::filesystem::path const & p) {
	return !std::filesystem::is_directory(p);
};

export auto files_in_directory(std::filesystem::path const & path) {
	return containers::filter(
		containers::subrange(
			std::filesystem::recursive_directory_iterator(path),
			std::default_sentinel
		),
		not_directory
	);
}

} // namespace technicalmachine::ps_usage_stats
