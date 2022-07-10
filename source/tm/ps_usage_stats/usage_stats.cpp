// Copyright David Stone 2021.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <tm/ps_usage_stats/usage_stats.hpp>

#include <tm/stat/calculate.hpp>

#include <bounded/detail/variant/visit.hpp>
#include <bounded/number_of.hpp>

#include <containers/algorithms/accumulate.hpp>
#include <containers/algorithms/erase.hpp>
#include <containers/algorithms/keyed_insert.hpp>
#include <containers/at.hpp>
#include <containers/lookup.hpp>

namespace technicalmachine::ps_usage_stats {

auto UsageStats::add(GenerationGeneric<Team> const & t, double const weight) & -> void {
	bounded::visit(t, [&](auto const & team) {
		for (auto const & pokemon : team.all_pokemon()) {
			auto & per_species = m_all[bounded::integer(pokemon.species())];
			per_species.total += weight;
			per_species.abilities[bounded::integer(pokemon.initial_ability())] += weight;
			per_species.items[bounded::integer(pokemon.item(false, false))] += weight;
			for (auto const move : pokemon.regular_moves()) {
				per_species.moves[bounded::integer(move.name())] += weight;
			}
			auto const calculated_speed = calculate_speed(Team({pokemon}), Ability::Honey_Gather, Weather());
			containers::at(per_species.speed, calculated_speed) += weight;
		}
	});
	m_total_teams += weight;
}

namespace {

struct LocalTopMoves {
	MoveName move;
	double value;
};

auto get_most_used(containers::array<double, bounded::number_of<MoveName>> const & moves, double const percent_threshold) -> containers::vector<LocalTopMoves> {
	auto const total_sum = containers::sum(moves);
	if (total_sum == 0.0) {
		return {};
	}
	auto const usage_threshold = percent_threshold * total_sum;
	auto top_moves = containers::vector<LocalTopMoves>(containers::transform(
		containers::enum_range<MoveName>(),
		[&](MoveName const move) { return LocalTopMoves{move, moves[bounded::integer(move)]}; }
	));
	auto current_sum = 0.0;
	containers::ska_sort(top_moves, [](LocalTopMoves const x) { return -x.value; });
	auto it = containers::begin(top_moves);
	while (true) {
		BOUNDED_ASSERT(it != containers::end(top_moves));
		current_sum += it->value;
		++it;
		if (current_sum >= usage_threshold) {
			break;
		}
	}
	containers::erase_after(top_moves, it);
	return top_moves;
}

} // namespace

Correlations::Correlations(UsageStats const & usage_stats) {
	constexpr auto threshold = 0.95;
	for (auto const species : containers::enum_range<Species>()) {
		m_data[bounded::integer(species)].top_moves = TopMoves(
			containers::assume_unique,
			containers::transform(get_most_used(usage_stats.moves(species), threshold), [](LocalTopMoves const moves) {
				return containers::range_value_t<TopMoves>{moves.move, std::make_unique<LockedAccess<MoveData>>()};
			})
		);
	}
}

namespace {

auto populate_correlation(auto & correlations, auto const key, double const weight) {
	auto & mapped = containers::keyed_insert(correlations, key, 0.0).iterator->mapped;
	mapped += weight;
}

auto populate_teammate_correlations(Correlations::Teammates & teammates, auto const & team, auto const & pokemon1, double const weight) -> void {
	auto is_different = [&](auto const & pokemon2) { return std::addressof(pokemon1) != std::addressof(pokemon2); };
	for (auto const & pokemon2 : containers::filter(team.all_pokemon(), is_different)) {
		auto & mapped = teammates[bounded::integer(pokemon2.species())];
		mapped.usage += weight;
		for (auto const move2 : pokemon2.regular_moves()) {
			populate_correlation(mapped.other_moves, move2.name(), weight);
		}
	}
}

auto populate_move_correlations(auto & correlations, RegularMoves const moves, MoveName const move1, double const weight) -> void {
	auto is_different = [=](Move const move2) { return move2.name() != move1; };
	for (auto const move2 : containers::filter(moves, is_different)) {
		correlations[bounded::integer(move2.name())] += weight;
	}
}

auto populate_correlations(Correlations::MoveData & data, auto const & team, auto const & pokemon, MoveName const move_name, double const weight) -> void {
	populate_teammate_correlations(data.teammates, team, pokemon, weight);
	populate_move_correlations(data.moves, pokemon.regular_moves(), move_name, weight);
	data.items[bounded::integer(pokemon.item(false, false))] += weight;
	data.abilities[bounded::integer(pokemon.initial_ability())] += weight;
	auto const calculated_speed = calculate_speed(Team({pokemon}), Ability::Honey_Gather, Weather());
	containers::at(data.speed, calculated_speed) += weight;
}

} // namespace

auto Correlations::add(GenerationGeneric<Team> const & t, double const weight) & -> void {
	bounded::visit(t, [&](auto const & team) {
		for (auto const & pokemon : team.all_pokemon()) {
			auto & per_species = m_data[bounded::integer(pokemon.species())];
			populate_teammate_correlations(per_species.teammates->locked().data, team, pokemon, weight);
			for (auto const move : pokemon.regular_moves()) {
				auto const move_name = move.name();
				auto const maybe_correlations = containers::lookup(per_species.top_moves, move_name);
				if (!maybe_correlations) {
					continue;
				}
				auto & correlations = *maybe_correlations;
				populate_correlations(correlations->locked().data, team, pokemon, move_name, weight);
			}
		}
	});
}

} // namespace technicalmachine::ps_usage_stats
