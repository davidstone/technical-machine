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

import tm.bytes_in_file;
import tm.files_in_path;
import tm.initial_team;

import bounded;
import containers;
import std_module;

namespace technicalmachine {
using namespace bounded::literal;
using namespace std::string_view_literals;

enum class Extension {
	po,
	pl,
	sb,
	nb
};
auto parse_extension(std::filesystem::path const & file_name) -> Extension {
	auto const extension = file_name.extension();
	if (extension == ".tp") {
		return Extension::po;
	} else if (extension == ".sbt") {
		return Extension::pl;
	} else if (extension == "") {
		return Extension::sb;
	} else if (extension == ".pnb" or extension == ".dpnb") {
		return Extension::nb;
	} else {
		throw InvalidTeamFileFormat(file_name);
	}
}

export auto load_team_from_file(std::filesystem::path const & file_name) -> AnyInitialTeam {
	auto const extension = parse_extension(file_name);
	auto const bytes = bytes_in_file(file_name);
	try {
		switch (extension) {
			case Extension::po:
				return po::read_team_file(bytes);
			case Extension::pl:
				return pl::read_team_file(bytes);
			case Extension::sb:
				return sb::read_team_file(bytes);
			case Extension::nb:
				return nb::read_team_file(bytes);
		}
	} catch (std::exception const & ex) {
		throw std::runtime_error(containers::concatenate<std::string>("Failed to parse team file \""sv, file_name.string(), "\" -- "sv, std::string_view(ex.what())));
	}
}

export auto load_random_team_from_directory(std::mt19937 & random_engine, std::filesystem::path const & path) -> AnyInitialTeam {
	auto const files = files_in_path(path);
	if (containers::is_empty(files)) {
		throw std::runtime_error(path.string() + " does not contain any team files.");
	}
	auto distribution = std::uniform_int_distribution<std::size_t>(0, static_cast<std::size_t>(containers::size(files) - 1_bi));
	auto const file_name = containers::at(files, distribution(random_engine));
	return load_team_from_file(file_name);
}

} // namespace technicalmachine
