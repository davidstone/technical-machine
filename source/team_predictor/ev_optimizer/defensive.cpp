// Optimize defensive EVs and nature to remove waste
// Copyright (C) 2016 David Stone
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
#include <set>
#include <string>
#include <unordered_map>

namespace technicalmachine {
namespace {

using namespace bounded::literal;

using Single = containers::vector<SingleClassificationEVs>;
using Estimates = containers::vector<DataPoint>;
using AllPossible = std::unordered_map<Nature, Estimates>;
AllPossible combine_results(Single const & physical, Single const & special);

DefensiveEVs::BestPerNature best_possible_per_nature(AllPossible all, Pokemon const & pokemon);
std::set<Nature> used_natures(DefensiveEVs::BestPerNature const & container);

void filter_to_minimum_evs(AllPossible & all);

DefensiveEVs::BestPerNature most_effective_equal_evs(AllPossible const & all, Pokemon const & pokemon);
DefensiveEVs::BestPerNature::mapped_type most_effective_equal_evs_per_nature(Estimates const & original, Pokemon const & pokemon);

using Divided = containers::array<DefensiveEVs::Natures, 4>;
Divided divide_natures(DefensiveEVs::BestPerNature const & container);

bool has_same_effect_on_defenses(Nature nature, Nature reference_nature);

}	// namespace

DefensiveEVs::DefensiveEVs(Pokemon const & pokemon) {
	auto const physical = equal_defensiveness<true>(pokemon);
	auto const special = equal_defensiveness<false>(pokemon);
	container = best_possible_per_nature(combine_results(physical, special), pokemon);
	for (auto const & value : divide_natures(container)) {
		remove_inefficient_natures(value);
	}
	add_other_potential_natures();
	assert(!container.empty());
}

void DefensiveEVs::remove_inefficient_natures(DefensiveEVs::Natures const & divided_natures) {
	auto const capacity = static_cast<std::intmax_t>(DefensiveEVs::Natures::capacity());
	containers::static_vector<std::reference_wrapper<BestPerNature::value_type const>, capacity> boosters;
	for (auto const & value : container) {
		if (containers::any_equal(begin(divided_natures), end(divided_natures), value.first)) {
			boosters.emplace_back(value);
		}
	}
	auto const iter_sum = [](auto const & lhs, auto const & rhs) {
		return lhs.get().second.sum() < rhs.get().second.sum();
	};
	auto const best = std::min_element(begin(boosters), end(boosters), iter_sum);
	for (auto const & value : divided_natures) {
		if (value != best->get().first) {
			container.erase(value);
		}
	}
}

void DefensiveEVs::add_other_potential_natures() {
	for (auto const reference_nature : used_natures(container)) {
		for (StatNames boosted = static_cast<StatNames>(0); boosted != StatNames::NORMAL_END; boosted = static_cast<StatNames>(static_cast<int>(boosted) + 1)) {
			for (StatNames penalized = static_cast<StatNames>(0); penalized != StatNames::NORMAL_END; penalized = static_cast<StatNames>(static_cast<int>(penalized) + 1)) {
				auto const nature = make_nature(boosted, penalized);
				if (!has_same_effect_on_defenses(nature, reference_nature)) {
					continue;
				}
				if (nature == reference_nature) {
					continue;
				}
				DataPoint const new_data_point(container.at(reference_nature), nature);
				container.emplace(nature, new_data_point);
			}
		}
	}
}

namespace {

std::set<Nature> used_natures(DefensiveEVs::BestPerNature const & container) {
	std::set<Nature> used;
	for (auto const & value : container) {
		used.emplace(value.first);
	}
	return used;
}

bool boosts_same(Nature nature, Nature reference_nature);
bool penalizes_same(Nature nature, Nature reference_nature);

bool has_same_effect_on_defenses(Nature const nature, Nature const reference_nature) {
	return boosts_same(nature, reference_nature) and penalizes_same(nature, reference_nature);
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

AllPossible combine_results(Single const & physical, Single const & special) {
	AllPossible all;
	for (auto const & p : physical) {
		for (auto const & s : special) {
			if (are_compatible(p, s)) {
				all[p.nature()].emplace_back(p, s);
			}
		}
	}
	return all;
}

DefensiveEVs::BestPerNature best_possible_per_nature(AllPossible all, Pokemon const & pokemon) {
	filter_to_minimum_evs(all);
	return most_effective_equal_evs(all, pokemon);
}

void filter_to_minimum_evs(AllPossible & all) {
	for (auto & per_nature : all) {
		auto const least_sum = [](DataPoint const & lhs, DataPoint const & rhs) {
			return lhs.sum() < rhs.sum();
		};
		auto & container = per_nature.second;
		auto const it = std::min_element(std::begin(container), std::end(container), least_sum);
		auto const not_minimum = [it](DataPoint const & value) {
			return value.sum() != it->sum();
		};
		erase_if(container, not_minimum);
	}
}


DefensiveEVs::BestPerNature most_effective_equal_evs(AllPossible const & all, Pokemon const & pokemon) {
	DefensiveEVs::BestPerNature result;
	for (auto & per_nature : all) {
		result.emplace(per_nature.first, most_effective_equal_evs_per_nature(per_nature.second, pokemon));
	}
	return result;
}

DefensiveEVs::BestPerNature::mapped_type most_effective_equal_evs_per_nature(Estimates const & original, Pokemon const & pokemon) {
	auto const greatest_product = [& pokemon](DataPoint const & largest, DataPoint const & value) {
		return lesser_product(largest, value, pokemon);
	};
	return *std::max_element(std::begin(original), std::end(original), greatest_product);
}

Divided divide_natures(DefensiveEVs::BestPerNature const & container) {
	constexpr auto boost = 0_bi;
	constexpr auto penalty = 1_bi;
	constexpr auto both = 2_bi;
	constexpr auto neutral = 3_bi;
	Divided divided;
	for (auto const & value : container) {
		Nature const nature(value.first);
		auto & stat =
			(boosts_defending_stat(nature) and !lowers_defending_stat(nature)) ? divided[boost] :
			(lowers_defending_stat(nature) and !boosts_defending_stat(nature)) ? divided[penalty] :
			(lowers_defending_stat(nature) and boosts_defending_stat(nature)) ? divided[both] :
			divided[neutral];
		stat.emplace_back(value.first);
	}
	return divided;
}

}	// namespace
}	// namespace technicalmachine
