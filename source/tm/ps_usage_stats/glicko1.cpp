// Copyright David Stone 2021.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <tm/ps_usage_stats/glicko1.hpp>

#include <containers/algorithms/accumulate.hpp>

#include <cmath>

namespace technicalmachine::ps_usage_stats {
namespace {

// ln(10.0) / 400.0
constexpr auto q = 0.00575646273248511421004;

auto g(double const deviation) -> double {
	return 1.0 / std::sqrt(1.0 + 3.0 * square(q) * square(deviation) / square(pi));
}

auto E(Rating const rating) -> double {
	return 1.0 / (1.0 + std::pow(10.0, -g(rating.deviation) * (initial_rating.value - rating.value) / 400.0));
}

auto reciprocal_of_d_squared_delta(Rating const rating) -> double {
	return square(q) * square(g(rating.deviation)) * E(rating) * (1.0 - E(rating));
};

} // namespace

auto Glicko1::add_result(BattleResult::Side::ID const id1, BattleResult::Side::ID const id2, BattleResult::Winner const winner) & -> void {
	auto to_score = [=](BattleResult::Side::ID const id) {
		constexpr auto lose = 0.0;
		constexpr auto tie = 0.5;
		constexpr auto win = 1.0;
		switch (winner) {
			case BattleResult::Winner::side1: return id == id1 ? win : lose;
			case BattleResult::Winner::side2: return id == id2 ? win : lose;
			case BattleResult::Winner::tie: return tie;
		}
	};

	auto update = [&](BattleResult::Side::ID const id) {
		auto & mapped = m_map.try_emplace(id, Mapped(FirstPass{0.0, 0.0})).first->second;
		mapped.first_pass.rating_sum += g(initial_rating.deviation) * (to_score(id) - E(initial_rating));
		mapped.first_pass.reciprocal_of_d_squared += reciprocal_of_d_squared_delta(initial_rating);
	};
	update(id1);
	update(id2);
}

auto Glicko1::finalize() & -> void {
	for (auto & element : m_map) {
		auto & mapped = element.second;
		auto const common = 1.0 / (1.0 / square(initial_rating.deviation) + mapped.first_pass.reciprocal_of_d_squared);
		mapped = Mapped(Rating(
			initial_rating.value + q * common * mapped.first_pass.rating_sum,
			std::sqrt(common)
		));
	}
}

} // namespace technicalmachine::ps_usage_stats