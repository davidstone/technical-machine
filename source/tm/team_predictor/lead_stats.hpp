// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <tm/pokemon/species.hpp>

#include <tm/number_of.hpp>

#include <containers/array.hpp>

namespace technicalmachine {

struct UsageStats;

struct LeadStats {
	constexpr explicit LeadStats(bool const use_lead_stats):
		m_use_lead_stats(use_lead_stats)
	{
	}
	
	auto get(UsageStats const & usage_stats) const -> containers::array<float, number_of<Species>.value()> const &;

private:
	bool m_use_lead_stats;
};

constexpr auto use_lead_stats = LeadStats(true);
constexpr auto do_not_use_lead_stats = LeadStats(false);

}	// namespace technicalmachine
