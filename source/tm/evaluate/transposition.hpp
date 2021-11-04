// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <tm/evaluate/best_move.hpp>
#include <tm/evaluate/compressed_battle.hpp>
#include <tm/evaluate/depth.hpp>

#include <tm/move/moves.hpp>

#include <tm/compress.hpp>
#include <tm/weather.hpp>

#include <bounded/optional.hpp>

#include <containers/array.hpp>
#include <containers/index_type.hpp>

namespace technicalmachine {

template<Generation>
struct Team;

template<Generation generation>
struct TranspositionTable {
	auto add_score(Team<generation> const & ai, Team<generation> const & foe, Weather weather, DepthValues const depth, BestMove best_move) -> void {
		auto const compressed_battle = compress_battle(ai, foe, weather);
		auto & value = m_data[index(compressed_battle)];
		value.compressed_battle = compressed_battle;
		value.depth = depth;
		value.move = best_move.name;
		value.score = best_move.score;
	}

	auto get_score(Team<generation> const & ai, Team<generation> const & foe, Weather weather, DepthValues const depth) const -> bounded::optional<BestMove> {
		auto const compressed_battle = compress_battle(ai, foe, weather);
		auto const & value = m_data[index(compressed_battle)];
		if (value.depth >= depth and value.compressed_battle == compressed_battle) {
			return BestMove{value.move, value.score};
		}
		return bounded::none;
	}

private:
	struct Value {
		CompressedBattle<generation> compressed_battle = {};
		DepthValues depth = {};
		Moves move = {};
		double score = 0.0;
	};
	static constexpr auto size = bounded::constant<1 << 13>;
	using Data = containers::array<Value, size.value()>;
	using Index = containers::index_type<Data>;

	template<bounded::bounded_integer Output>
	static constexpr auto update_hash(Output & output, bounded::bounded_integer auto input) {
		while (input != 0_bi) {
			auto const temp = input % size;
			input /= size;
			output = bounded::assume_in_range<Output>(output.value() xor temp.value());
		}
	}

	template<std::size_t... indexes>
	static constexpr auto hash(CompressedBattle<generation> const & compressed_battle, std::index_sequence<indexes...>) {
		static_assert(bounded::pow(2_bi, bounded::log(size, 2_bi)) == size, "Size must be a power of 2");
		auto result = bounded::integer<0, bounded::normalize<size - 1_bi>>(0_bi);
		(..., update_hash(result, compressed_battle[bounded::constant<indexes>]));
		return result;
	}

	static auto index(CompressedBattle<generation> const & compressed_battle) -> Index {
		return hash(
			compressed_battle,
			bounded::make_index_sequence(bounded::tuple_size<CompressedBattle<generation>>)
		);
	}

	Data m_data;
};

extern template struct TranspositionTable<Generation::one>;
extern template struct TranspositionTable<Generation::two>;
extern template struct TranspositionTable<Generation::three>;
extern template struct TranspositionTable<Generation::four>;
extern template struct TranspositionTable<Generation::five>;
extern template struct TranspositionTable<Generation::six>;
extern template struct TranspositionTable<Generation::seven>;
extern template struct TranspositionTable<Generation::eight>;

} // namespace technicalmachine
