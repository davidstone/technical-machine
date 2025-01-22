// Copyright David Stone 2021.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.ps_usage_stats.files_in_directory;

import containers;
import std_module;

namespace technicalmachine::ps_usage_stats {

// Work around https://github.com/llvm/llvm-project/issues/59513
struct not_directory {
	static auto operator()(std::filesystem::path const & p) {
		return !std::filesystem::is_directory(p);
	}
};

export auto files_in_directory(std::filesystem::path const & path) {
	return containers::filter(
		containers::subrange(
			std::filesystem::recursive_directory_iterator(path),
			std::default_sentinel
		),
		not_directory()
	);
}

} // namespace technicalmachine::ps_usage_stats
