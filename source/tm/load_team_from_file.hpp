// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <tm/clients/invalid_team_file_format.hpp>
#include <tm/clients/pokemon_lab/read_team_file.hpp>
#include <tm/clients/pokemon_online/read_team_file.hpp>

#include <tm/files_in_path.hpp>
#include <tm/generation.hpp>
#include <tm/team.hpp>

#include <containers/at.hpp>
#include <containers/is_empty.hpp>
#include <containers/size.hpp>

#include <filesystem>
#include <random>
#include <stdexcept>

namespace technicalmachine {

inline auto load_team_from_file(std::mt19937 & random_engine, std::filesystem::path const & path) {
	auto const files = files_in_path(path);
	if (containers::is_empty(files)) {
		throw std::runtime_error(path.string() + " does not contain any team files.");
	}
	auto const max = (containers::size(files) - 1_bi).value();
	auto distribution = std::uniform_int_distribution(static_cast<decltype(max)>(0), max);
	auto const file_name = containers::at(files, distribution(random_engine));

	auto const extension = file_name.extension();
	if (extension == ".tp") {
		return po::read_team_file(file_name);
	} else if (extension == ".sbt") {
		return pl::read_team_file(file_name);
	} else {
		throw InvalidTeamFileFormat(file_name);
	}
}

}	// namespace technicalmachine
