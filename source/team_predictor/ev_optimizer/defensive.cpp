// Optimize defensive EVs and nature to remove waste
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

#include "defensive.hpp"

#include "offensive.hpp"
#include "single_classification_evs.hpp"

#include "../../pokemon/pokemon.hpp"

#include "../../stat/nature.hpp"
#include "../../stat/stat_names.hpp"

#include <containers/algorithms/all_any_none.hpp>
#include <containers/array/array.hpp>
#include <containers/vector/vector.hpp>

#include <algorithm>
#include <cassert>
#include <string>
#include <unordered_map>

namespace technicalmachine {
namespace {

using namespace bounded::literal;

using Estimates = containers::vector<DataPoint>;
using AllPossible = std::unordered_map<Nature, Estimates>;

auto combine_results(EqualDefensiveness const & physical, EqualDefensiveness const & special) {
	AllPossible all;
	for (auto const & p : physical) {
		for (auto const & s : special) {
			if (are_compatible(p, s)) {
				all[p.nature].emplace_back(p, s);
			}
		}
	}
	return all;
}

void filter_to_minimum_evs(AllPossible & all) {
	for (auto & per_nature : all) {
		auto const least_sum = [](DataPoint const & lhs, DataPoint const & rhs) {
			return ev_sum(lhs) < ev_sum(rhs);
		};
		auto & container = per_nature.second;
		auto const it = std::min_element(begin(container), end(container), least_sum);
		auto const not_minimum = [it](DataPoint const & value) {
			return ev_sum(value) != ev_sum(*it);
		};
		erase_if(container, not_minimum);
	}
}

DataPoint most_effective_equal_evs_per_nature(Estimates const & original, Pokemon const & pokemon) {
	auto const greatest_product = [& pokemon](DataPoint const & largest, DataPoint const & value) {
		return lesser_product(largest, value, pokemon);
	};
	return *std::max_element(begin(original), end(original), greatest_product);
}

auto most_effective_equal_evs(AllPossible const & all, Pokemon const & pokemon) {
	DefensiveEVs::BestPerNature result;
	for (auto & per_nature : all) {
		result.emplace_back(most_effective_equal_evs_per_nature(per_nature.second, pokemon));
	}
	return result;
}

auto best_possible_per_nature(AllPossible all, Pokemon const & pokemon) {
	filter_to_minimum_evs(all);
	return most_effective_equal_evs(all, pokemon);
}

auto divide_natures(DefensiveEVs::BestPerNature const & container) {
	constexpr auto boost = 0_bi;
	constexpr auto penalty = 1_bi;
	constexpr auto both = 2_bi;
	constexpr auto neutral = 3_bi;
	auto divided = containers::array<DefensiveEVs::Natures, 4>{};
	for (auto const & value : container) {
		auto const nature = value.nature;
		auto & stat =
			(boosts_defending_stat(nature) and !lowers_defending_stat(nature)) ? divided[boost] :
			(lowers_defending_stat(nature) and !boosts_defending_stat(nature)) ? divided[penalty] :
			(lowers_defending_stat(nature) and boosts_defending_stat(nature)) ? divided[both] :
			divided[neutral];
		stat.emplace_back(nature);
	}
	return divided;
}

bool boosts_same(Nature const nature, Nature const reference_nature) {
	return (boosts_stat(nature, StatNames::DEF) and boosts_stat(reference_nature, StatNames::DEF))
			or (boosts_stat(nature, StatNames::SPD) and boosts_stat(reference_nature, StatNames::SPD))
			or (!boosts_defending_stat(nature) and !boosts_defending_stat(reference_nature));
}

bool penalizes_same(Nature const nature, Nature const reference_nature) {
	return (lowers_stat(nature, StatNames::DEF) and lowers_stat(reference_nature, StatNames::DEF))
			or (lowers_stat(nature, StatNames::SPD) and lowers_stat(reference_nature, StatNames::SPD))
			or (!lowers_defending_stat(nature) and !lowers_defending_stat(reference_nature));
}

bool has_same_effect_on_defenses(Nature const nature, Nature const reference_nature) {
	return boosts_same(nature, reference_nature) and penalizes_same(nature, reference_nature);
}

constexpr auto matches_nature(Nature const nature) {
	return [=](auto const value) { return value.nature == nature; };
}

auto used_natures(DefensiveEVs::BestPerNature const & container) {
	auto used = DefensiveEVs::Natures();
	for (auto const & value : container) {
		assert(!containers::any_equal(used, value.nature));
		used.emplace_back(value.nature);
	}
	return used;
}

}	// namespace

DefensiveEVs::DefensiveEVs(Pokemon const & pokemon) {
	auto const physical = equal_defensiveness(pokemon, true);
	auto const special = equal_defensiveness(pokemon, false);
	container = best_possible_per_nature(combine_results(physical, special), pokemon);
	for (auto const & value : divide_natures(container)) {
		remove_inefficient_natures(value);
	}
	add_other_potential_natures();
	assert(!empty(container));
}

void DefensiveEVs::remove_inefficient_natures(DefensiveEVs::Natures const & divided_natures) {
	auto const capacity = static_cast<std::intmax_t>(DefensiveEVs::Natures::capacity());
	containers::static_vector<DataPoint, capacity> boosters;
	for (auto const & value : container) {
		if (containers::any_equal(divided_natures, value.nature)) {
			boosters.emplace_back(value);
		}
	}
	auto const iter_sum = [](auto const & lhs, auto const & rhs) {
		return ev_sum(lhs) < ev_sum(rhs);
	};
	auto const best = std::min_element(begin(boosters), end(boosters), iter_sum);
	for (auto const & nature : divided_natures) {
		if (nature != best->nature) {
			containers::erase(container, containers::find_if(container, matches_nature(nature)));
		}
	}
}

void DefensiveEVs::add_other_potential_natures() {
	for (auto const reference_nature : used_natures(container)) {
		for (auto const boosted : containers::enum_range(StatNames::NORMAL_END)) {
			for (auto const penalized : containers::enum_range(StatNames::NORMAL_END)) {
				auto const nature = make_nature(boosted, penalized);
				if (!has_same_effect_on_defenses(nature, reference_nature)) {
					continue;
				}
				if (nature == reference_nature) {
					continue;
				}
				auto const old_data_point = containers::find_if(container, matches_nature(reference_nature));
				assert(old_data_point != end(container));
				container.emplace_back(*old_data_point, nature);
			}
		}
	}
}

}	// namespace technicalmachine
