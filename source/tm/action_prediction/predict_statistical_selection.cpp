// Copyright David Stone 2024.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

module;

#include <bounded/assert.hpp>

export module tm.action_prediction.predict_statistical_selection;

import tm.action_prediction.predicted;

import tm.move.legal_selections;
import tm.move.move_name;
import tm.move.pass;
import tm.move.selection;
import tm.move.switch_;

import tm.pokemon.species;

import tm.environment;
import tm.generation;
import tm.get_legal_selections;
import tm.read_bytes;
import tm.team;
import tm.to_index;
import tm.usage_for;

import bounded;
import containers;
import std_module;
import tv;

import tm.string_conversions.team;

namespace technicalmachine {
using namespace bounded::literal;

using MoveData = containers::flat_map<MoveName, double>;

struct PerMatchup {
	constexpr auto operator[](MoveName const move) const -> double {
		auto const ptr = containers::lookup(m_data, move);
		return ptr ? *ptr : 0.2;
	}
	constexpr auto switch_out() const -> double {
		return m_switch_out;
	}
	constexpr auto switch_in() const -> double {
		return m_switch_in;
	}

	constexpr auto set(
		double const switch_out_weight,
		double const switch_in_multiplier,
		MoveData move_data
	) -> void {
		m_data = std::move(move_data);
		m_switch_out = switch_out_weight;
		m_switch_in = switch_in_multiplier;
	}
private:
	MoveData m_data;
	double m_switch_out;
	double m_switch_in;
};

using PerOther = UsageFor<Species, PerMatchup>;

export struct SelectionWeights {
	constexpr auto move_weight(Species const other, Species const user, MoveName const move) const -> double {
		auto const & per_other = m_data[to_index(other)];
		auto const & per_matchup = per_other[to_index(user)];
		return per_matchup[move];
	}
	constexpr auto switch_weight(Species const other, Species const user, Species const switch_) const -> double {
		auto const & per_other = m_data[to_index(other)];
		auto const & per_matchup = per_other[to_index(user)];
		return per_matchup.switch_out() * per_other[to_index(switch_)].switch_in();
	}

	constexpr auto set(
		Species const other,
		Species const user,
		double const switch_out_weight,
		double const switch_in_multiplier,
		MoveData move_data
	) -> void {
		auto & per_other = m_data[to_index(other)];
		auto & per_matchup = per_other[to_index(user)];
		per_matchup.set(switch_out_weight, switch_in_multiplier, std::move(move_data));
	}

private:
	UsageFor<Species, PerOther> m_data;
};

export auto read_selection_weights_from_file(std::istream & stream) -> std::unique_ptr<SelectionWeights> {
	auto weights = std::make_unique<SelectionWeights>();
	while (stream.peek() != std::char_traits<char>::eof()) {
		auto const other = read_bytes<Species>(stream);
		auto const user = read_bytes<Species>(stream);
		auto const switch_out_weight = read_bytes<double>(stream);
		auto const switch_in_multiplier = read_bytes<double>(stream);
		using MoveCount = bounded::integer<0, bounded::normalize<bounded::number_of<MoveName>>>;
		auto const move_count = read_bytes<MoveCount>(stream);
		auto moves = MoveData(containers::generate_n(move_count, [&] {
			auto const name = read_bytes<MoveName>(stream);
			auto const weight = read_bytes<double>(stream);
			return containers::map_value_type<MoveName, double>(name, weight);
		}));
		weights->set(
			other,
			user,
			switch_out_weight,
			switch_in_multiplier,
			std::move(moves)
		);
	}
	return weights;
}

struct WeightedSelection {
	Selection selection;
	double weight;
};

// This models a player that uses moves based on how often the move was used in
// that match-up in the historical logs. So if Tauros uses Body Slam against
// Starmie 26% of the time, then Body Slam is given a weight of 0.26 when up
// against a Starmie.
export template<Generation generation>
constexpr auto predict_statistical_selection(
	Team<generation> const & user,
	Team<generation> const & other,
	Environment const environment,
	SelectionWeights const & weights
) -> AllPredicted {
	auto const legal_selections = get_legal_selections(user, other, environment);

	auto const user_species = user.pokemon().species();
	auto const other_species = other.pokemon().species();
	auto const weighted_selections = containers::make_static_vector(containers::transform(
		legal_selections,
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
					return WeightedSelection(pass, 1.0);
				}
			));
		}
	));

	auto const cummulative_weight = containers::sum(containers::transform(weighted_selections, &WeightedSelection::weight));

	return AllPredicted(containers::transform(weighted_selections, [&](WeightedSelection const value) {
		auto const probability = cummulative_weight != 0.0 ?
			value.weight / cummulative_weight :
			1.0 / double(containers::size(weighted_selections));
		BOUNDED_ASSERT(0.0 <= probability and probability <= 1.0);
		return Predicted(value.selection, probability);
	}));
}

} // namespace technicalmachine
