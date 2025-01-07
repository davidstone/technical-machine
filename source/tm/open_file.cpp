// Copyright David Stone 2022.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.open_file;

import containers;
import std_module;

namespace technicalmachine {

using namespace std::string_view_literals;

template<typename Stream = std::fstream>
constexpr auto open_file(std::filesystem::path const & path, std::ios_base::openmode const mode) -> Stream {
	auto file = Stream(path, mode);
	try {
		file.exceptions(std::ios_base::badbit | std::ios_base::failbit);
	} catch (std::exception const & ex) {
		throw std::runtime_error(containers::concatenate<std::string>(
			"Could not open "sv,
			path.string(),
			": "sv,
			std::string_view(ex.what())
		));
	}
	return file;
}
	
auto open_file_for_writing(std::filesystem::path const & path, std::ios_base::openmode const mode) -> std::ofstream {
	std::filesystem::create_directories(path.parent_path());
	return open_file<std::ofstream>(path, mode);
}


export auto open_binary_file_for_reading(std::filesystem::path const & path) -> std::ifstream {
	return open_file<std::ifstream>(path, std::ios_base::binary);
}

export auto open_text_file_for_reading(std::filesystem::path const & path) -> std::ifstream {
	return open_file<std::ifstream>(path, {});
}

export auto open_binary_file_for_writing(std::filesystem::path const & path) -> std::ofstream {
	return open_file_for_writing(path, std::ios_base::binary);
}

export auto open_text_file_for_writing(std::filesystem::path const & path) -> std::ofstream {
	return open_file_for_writing(path, {});
}

export auto open_text_file(std::filesystem::path const & path) -> std::fstream {
	std::filesystem::create_directories(path.parent_path());
	return open_file<std::fstream>(path, std::ios_base::in | std::ios_base::out | std::ios_base::app);
}

} // namespace technicalmachine
