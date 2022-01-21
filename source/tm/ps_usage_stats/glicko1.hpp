// Copyright David Stone 2021.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <tm/ps_usage_stats/battle_result.hpp>
#include <tm/ps_usage_stats/rating.hpp>

#include <bounded/hash.hpp>

#include <containers/lookup.hpp>

#include <nlohmann/json.hpp>

namespace technicalmachine::ps_usage_stats {

// http://www.glicko.net/glicko/glicko.pdf
struct Glicko1 {
	auto add_result(BattleResult::Side::ID const side1, BattleResult::Side::ID const side2, BattleResult::Winner const winner) & -> void;
	auto finalize() & -> void;

	constexpr auto get(BattleResult::Side::ID const id) const -> Rating {
		return containers::lookup(m_map, id)->rating;
	}

private:
	struct FirstPass {
		double rating_sum;
		double reciprocal_of_d_squared;
	};
	union Mapped {
		constexpr explicit Mapped(FirstPass const first_pass_):
			first_pass(first_pass_)
		{
		}
		constexpr explicit Mapped(Rating const rating_):
			rating(rating_)
		{
		}
		FirstPass first_pass;
		Rating rating;
	};
	std::unordered_map<BattleResult::Side::ID, Mapped> m_map;
};

} // namespace technicalmachine::ps_usage_stats
