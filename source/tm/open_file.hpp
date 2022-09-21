// Copyright David Stone 2022.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <filesystem>
#include <fstream>

namespace technicalmachine {

template<typename Stream = std::fstream>
constexpr auto open_file(std::filesystem::path const & path, std::ios_base::openmode const mode) -> Stream {
	auto file = Stream(path, mode);
	file.exceptions(std::ios_base::badbit | std::ios_base::failbit);
	return file;
}

inline auto open_file_for_writing(std::filesystem::path const & path, std::ios_base::openmode const mode) -> std::ofstream {
	std::filesystem::create_directories(path.parent_path());
	return open_file<std::ofstream>(path, mode);
}


inline auto open_binary_file_for_reading(std::filesystem::path const & path) -> std::ifstream {
	return open_file<std::ifstream>(path, std::ios_base::binary);
}

inline auto open_text_file_for_reading(std::filesystem::path const & path) -> std::ifstream {
	return open_file<std::ifstream>(path, {});
}

inline auto open_binary_file_for_writing(std::filesystem::path const & path) -> std::ofstream {
	return open_file_for_writing(path, std::ios_base::binary);
}

inline auto open_text_file_for_writing(std::filesystem::path const & path) -> std::ofstream {
	return open_file_for_writing(path, {});
}

} // namespace technicalmachine
