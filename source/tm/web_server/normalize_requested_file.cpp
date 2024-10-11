// Copyright David Stone 2024.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.web_server.normalize_requested_file;

import containers;
import std_module;

namespace technicalmachine {

auto add_default_file(std::filesystem::path const & file) -> std::filesystem::path {
	return file.has_filename() ? file : file / "index.html";
}

export auto normalize_requested_file(
	std::filesystem::path const & root,
	std::filesystem::path const & file
) -> std::filesystem::path {
	if (!root.is_absolute()) {
		throw std::runtime_error("Root path must be an absolute path");
	}
	auto const normalized_file = add_default_file(file.relative_path().lexically_normal());
	if (*normalized_file.begin() == "..") {
		throw std::runtime_error("Requested file outside of server root");
	}
	return root / normalized_file;
}

} // namespace technicalmachine
