// Copyright (C) 2020 David Stone
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

#include <tm/evaluate/transposition.hpp>

#include <tm/team.hpp>

namespace technicalmachine {
namespace {

template<bounded::bounded_integer Output>
constexpr auto update_hash(Output & output, bounded::bounded_integer auto input) {
	constexpr auto size = bounded::max_value<Output> + 1_bi;
	while (input != 0_bi) {
		auto const temp = input % size;
		input /= size;
		output = Output(output.value() xor temp.value());
	}
}

template<std::size_t... indexes>
constexpr auto hash(auto const & compressed_battle, auto const size, std::index_sequence<indexes...>) {
	static_assert(bounded::pow(2_bi, bounded::log(size, 2_bi)) == size, "Size must be a power of 2");
	auto result = bounded::integer<0, bounded::detail::normalize<(size - 1_bi).value()>>(0_bi);
	(..., update_hash(result, compressed_battle[bounded::constant<indexes>]));
	return result;
}

auto compress_battle(Team const & ai, Team const & foe, Weather const weather) {
	auto const compressed_ai = compress(ai);
	auto const compressed_foe = compress(foe);
	static_assert(bounded::tuple_size<decltype(compressed_ai)> == 4_bi);
	static_assert(bounded::tuple_size<decltype(compressed_foe)> == 4_bi);
	return bounded::tuple(
		compressed_ai[0_bi],
		compressed_ai[1_bi],
		compress_combine(
			compressed_ai[2_bi],
			weather
		),
		compressed_ai[3_bi],
		compressed_foe[0_bi],
		compressed_foe[1_bi],
		compressed_foe[2_bi],
		compressed_foe[3_bi]
	);
}

} // namespace

auto TranspositionTable::index(CompressedBattle const & compressed_battle) -> Index {
	return hash(
		compressed_battle,
		size,
		bounded::make_index_sequence(bounded::tuple_size<CompressedBattle>)
	);
}

auto TranspositionTable::add_score(Team const & ai, Team const & foe, Weather weather, DepthValues const depth, BestMove best_move) -> void {
	auto const compressed_battle = compress_battle(ai, foe, weather);
	auto & value = m_data[index(compressed_battle)];
	value.compressed_battle = compressed_battle;
	value.depth = depth;
	value.move = best_move.name;
	value.score = best_move.score;
}

auto TranspositionTable::get_score(Team const & ai, Team const & foe, Weather weather, DepthValues const depth) const -> bounded::optional<BestMove> {
	auto const compressed_battle = compress_battle(ai, foe, weather);
	auto const & value = m_data[index(compressed_battle)];
	if (value.depth >= depth and value.compressed_battle == compressed_battle) {
		return BestMove{value.move, value.score};
	}
	return bounded::none;
}

} // namespace technicalmachine
