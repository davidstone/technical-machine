// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.clients.load_team_from_file;

import tm.clients.invalid_team_file_format;
import tm.clients.nb.read_team_file;
import tm.clients.pl.read_team_file;
import tm.clients.po.read_team_file;
import tm.clients.sb.read_team_file;

import tm.files_in_path;
import tm.generation;
export import tm.team;

import bounded;
import containers;
import std_module;

namespace technicalmachine {
using namespace bounded::literal;

export auto load_team_from_file(std::filesystem::path const & file_name) -> GenerationGeneric<KnownTeam> {
	auto const extension = file_name.extension();
	if (extension == ".tp") {
		return po::read_team_file(file_name);
	} else if (extension == ".sbt") {
		return pl::read_team_file(file_name);
	} else if (extension == "") {
		return sb::read_team_file(file_name);
	} else if (extension == ".pnb" or extension == ".dpnb") {
		return nb::read_team_file(file_name);
	} else {
		throw InvalidTeamFileFormat(file_name);
	}
}

export auto load_random_team_from_directory(std::mt19937 & random_engine, std::filesystem::path const & path) -> GenerationGeneric<KnownTeam> {
	auto const files = files_in_path(path);
	if (containers::is_empty(files)) {
		throw std::runtime_error(path.string() + " does not contain any team files.");
	}
	auto distribution = std::uniform_int_distribution<std::size_t>(0, static_cast<std::size_t>(containers::size(files) - 1_bi));
	auto const file_name = containers::at(files, distribution(random_engine));
	return load_team_from_file(file_name);
}

} // namespace technicalmachine
