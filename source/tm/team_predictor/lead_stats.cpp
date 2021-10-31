// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <tm/team_predictor/lead_stats.hpp>

#include <containers/array.hpp>

namespace technicalmachine {

struct UsageStats;

auto LeadStats::get(UsageStats const & usage_stats) const -> containers::array<float, number_of_species> const & {
	static constexpr auto unused_lead = containers::make_array_n(bounded::constant<number_of_species>, 1.0F);
	static_cast<void>(usage_stats);
	static_cast<void>(m_use_lead_stats);
	return unused_lead;
}

} // namespace technicalmachine
