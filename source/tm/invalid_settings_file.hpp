// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <bounded/integer.hpp>

#include <filesystem>
#include <stdexcept>

namespace technicalmachine {

struct InvalidSettingsFile : std::runtime_error {
	enum Problem {
		does_not_exist,
		too_long,
		too_short,
		invalid_data
	};
	InvalidSettingsFile(std::filesystem::path const & file_name, Problem const problem);
};

}	// namespace technicalmachine
namespace bounded {

template<>
inline constexpr auto min_value<technicalmachine::InvalidSettingsFile::Problem> = technicalmachine::InvalidSettingsFile::Problem();

template<>
inline constexpr auto max_value<technicalmachine::InvalidSettingsFile::Problem> = technicalmachine::InvalidSettingsFile::Problem::invalid_data;

}	// namespace bounded
