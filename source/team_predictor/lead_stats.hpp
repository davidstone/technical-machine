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

#include "../pokemon/species.hpp"

#include <containers/array/array.hpp>

namespace technicalmachine {

struct UsageStats;

struct LeadStats {
	constexpr explicit LeadStats(bool const use_lead_stats):
		m_use_lead_stats(use_lead_stats)
	{
	}
	
	auto get(UsageStats const & usage_stats) const -> containers::array<float, number_of_species> const &;

private:
	bool m_use_lead_stats;
};

constexpr auto use_lead_stats = LeadStats(true);
constexpr auto do_not_use_lead_stats = LeadStats(false);

}	// namespace technicalmachine
