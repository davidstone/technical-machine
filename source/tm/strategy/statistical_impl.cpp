// Copyright David Stone 2024.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

module;

#include <bounded/assert.hpp>

module tm.strategy.statistical;

import tm.move.legal_selections;
import tm.move.move_name;
import tm.move.pass;
import tm.move.selection;
import tm.move.switch_;

import tm.pokemon.species;

import tm.strategy.weighted_selection;

import tm.string_conversions.generation;

import tm.binary_file_reader;
import tm.environment;
import tm.generation;
import tm.get_directory;
import tm.open_file;
import tm.team;
import tm.to_index;
import tm.usage_for;
import tm.weight;

import bounded;
import containers;
import numeric_traits;
import std_module;
import tv;

namespace technicalmachine {
namespace {
using namespace bounded::literal;

using MoveData = containers::flat_map<MoveName, Weight<double>>;

constexpr auto minimum_weight = Weight(1.0 / 1'000'000.0);

struct PerMatchup {
	constexpr auto operator[](MoveName const move) const -> Weight<double> {
		auto const ptr = containers::lookup(m_data, move);
		return ptr ? *ptr : Weight(0.2);
	}
	constexpr auto switch_out() const -> Weight<double> {
		return m_switch_out;
	}
	constexpr auto switch_in() const -> Weight<double> {
		return m_switch_in;
	}

	constexpr auto set(
		Weight<double> const switch_out_weight,
		Weight<double> const switch_in_multiplier,
		MoveData move_data
	) -> void {
		m_data = std::move(move_data);
		m_switch_out = switch_out_weight;
		m_switch_in = switch_in_multiplier;
	}
private:
	MoveData m_data;
	Weight<double> m_switch_out = minimum_weight;
	Weight<double> m_switch_in = minimum_weight;
};

using PerOther = UsageFor<Species, PerMatchup>;

constexpr auto with_minimum_weight(Weight<double> const weight) -> Weight<double> {
	return weight == Weight(0.0) ? minimum_weight : weight;
}

struct SelectionWeights {
	explicit SelectionWeights(Generation const generation) {
		auto stream = open_binary_file_for_reading(
			get_usage_stats_directory() / to_string(generation) / "OU.tmsw"
		);
		while (stream.peek() != std::char_traits<char>::eof()) {
			auto const other = read<Species>(stream);
			auto const user = read<Species>(stream);
			auto const switch_out_weight = read_weight(stream);
			auto const switch_in_multiplier = read_weight(stream);
			auto moves = MoveData(map_reader<MoveName>(stream, [&] {
				return with_minimum_weight(read_weight(stream));
			}));
			set(
				other,
				user,
				with_minimum_weight(switch_out_weight),
				with_minimum_weight(switch_in_multiplier),
				std::move(moves)
			);
		}
	}

	constexpr auto move_weight(Species const other, Species const user, MoveName const move) const -> Weight<double> {
		auto const & per_other = m_data[to_index(other)];
		auto const & per_matchup = per_other[to_index(user)];
		return per_matchup[move];
	}
	constexpr auto switch_weight(Species const other, Species const user, Species const switch_) const -> Weight<double> {
		auto const & per_other = m_data[to_index(other)];
		auto const & per_matchup = per_other[to_index(user)];
		return per_matchup.switch_out() * per_other[to_index(switch_)].switch_in();
	}

private:
	constexpr auto set(
		Species const other,
		Species const user,
		Weight<double> const switch_out_weight,
		Weight<double> const switch_in_multiplier,
		MoveData move_data
	) -> void {
		auto & per_other = m_data[to_index(other)];
		auto & per_matchup = per_other[to_index(user)];
		per_matchup.set(switch_out_weight, switch_in_multiplier, std::move(move_data));
	}

	UsageFor<Species, PerOther> m_data{};
};

struct Statistical {
	Statistical():
		m_all{
			SelectionWeights(Generation::one),
			SelectionWeights(Generation::two),
			SelectionWeights(Generation::three),
			SelectionWeights(Generation::four),
			SelectionWeights(Generation::five),
			SelectionWeights(Generation::six),
			SelectionWeights(Generation::seven),
			SelectionWeights(Generation::eight)
		}
	{
		static_assert(numeric_traits::max_value<Generation> == Generation::eight);
	}
	template<Generation generation>
	auto operator()(
		Team<generation> const & user,
		LegalSelections const user_selections,
		Team<generation> const & other,
		[[maybe_unused]] LegalSelections const other_selections,
		[[maybe_unused]] Environment const environment
	) const -> WeightedSelections {
		auto const & weights = m_all[to_index(generation)];
		auto const user_species = user.pokemon().species();
		auto const other_species = other.pokemon().species();
		auto const weighted_selections = containers::make_static_vector(containers::transform(
			user_selections,
			[&](Selection const selection) {
				return tv::visit(selection, tv::overload(
					[&](MoveName const move) -> WeightedSelection {
						return WeightedSelection(
							selection,
							weights.move_weight(other_species, user_species, move)
						);
					},
					[&](Switch const switch_) -> WeightedSelection {
						auto const switch_in_species = user.pokemon(switch_.value()).species();
						return WeightedSelection(
							selection,
							weights.switch_weight(other_species, user_species, switch_in_species)
						);
					},
					[](Pass) -> WeightedSelection {
						return WeightedSelection(pass, Weight(1.0));
					}
				));
			}
		));
		return weighted_selections;
	}

private:
	using Container = containers::array<
		SelectionWeights,
		bounded::number_of<Generation>
	>;
	Container m_all;
};

} // namespace

auto make_statistical() -> Strategy {
	return Strategy(bounded::lazy_init, bounded::construct<Statistical>);
}

} // namespace technicalmachine
