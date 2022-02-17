// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <tm/clients/log_foe_team.hpp>

#include <tm/clients/random_string.hpp>

#include <containers/algorithms/concatenate.hpp>
#include <containers/string.hpp>

#include <string_view>

namespace technicalmachine {
namespace {

constexpr auto get_extension() -> std::string_view {
	// TODO: add support for other formats
	return ".sbt";
}

} // namespace

std::filesystem::path generate_team_file_name(std::mt19937 & random_engine) {
	// Randomly generates a file name in 8.3 format. It then checks to see if
	// that file name already exists. If it does, it randomly generates a new
	// file name, and continues until it generates a name that does not exist.
	// This limits the potential for a race condition down to a 1 / 36^8 chance
	// (about 1 / 2 ^ 41), assuming that another process / thread is also
	// trying to save an 8 character file name with an identical extension at
	// the same time. The result of this is that a team file would not be saved
	// when it should have been, which is not a major issue.
	constexpr auto file_name_length = 8_bi;
	std::filesystem::path foe_team_file;
	do {
		foe_team_file = team_file_directory();
		foe_team_file /= std::string_view(containers::concatenate<containers::string>(random_string(random_engine, file_name_length), get_extension()));
	} while (std::filesystem::exists(foe_team_file));
	return foe_team_file;
}

} // namespace technicalmachine
