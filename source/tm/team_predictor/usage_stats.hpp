// Copyright (C) 2018 David Stone
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

#pragma once

#include <tm/team_predictor/detailed_stats.hpp>
#include <tm/team_predictor/multiplier.hpp>

#include <tm/pokemon/species.hpp>

#include <containers/array/array.hpp>

#include <filesystem>

namespace technicalmachine {

struct UsageStats {
	explicit UsageStats(std::filesystem::path const & usage_stats_directory);
	auto const & overall() const { return m_overall; }
	auto const & lead() const { return m_lead; }
	auto const & detailed() const { return m_detailed; }
	auto const & multiplier() const { return m_multiplier; }
private:
	OverallStats m_overall;
	// Multiplier for Pokemon after you've seen the lead
	containers::array<float, number_of_species> m_lead;
	DetailedStats m_detailed;
	Multiplier m_multiplier;
};

}	// namespace technicalmachine
