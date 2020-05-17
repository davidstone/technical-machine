// Copyright (C) 2019 David Stone
//
// This file is part of Technical Machine.
//
// Technical Machine is free software: you can redistribute it and / or modify
// it under the terms of the GNU Affero General Public License as
// published by the Free Software Foundation, either version 3 of the
// License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.

#include <tm/load_team_from_file.hpp>

#include <tm/clients/invalid_team_file_format.hpp>

#include <tm/clients/pokemon_lab/read_team_file.hpp>
#include <tm/clients/pokemon_online/read_team_file.hpp>

#include <tm/files_in_path.hpp>

#include <stdexcept>

namespace technicalmachine {

auto load_team_from_file(std::mt19937 & random_engine, std::filesystem::path const & path) -> Team {
	auto const files = files_in_path(path);
	if (empty(files)) {
		throw std::runtime_error(path.string() + " does not contain any team files.");
	}
	auto const max = (size(files) - 1_bi).value();
	auto distribution = std::uniform_int_distribution(static_cast<decltype(max)>(0), max);
	auto const file = files[containers::index_type<decltype(files)>(distribution(random_engine))];

	auto const extension = file.extension();
	if (extension == ".tp") {
		return po::load_team(file);
	} else if (extension == ".sbt") {
		return pl::load_team(file);
	} else {
		throw InvalidTeamFileFormat(file);
	}
}

}	// namespace technicalmachine
