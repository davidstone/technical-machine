// Optimize defensive EVs and nature to remove waste
// Copyright (C) 2014 David Stone
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

#include <algorithm>
#include <cassert>
#include <set>
#include <string>
#include <vector>

namespace technicalmachine {
namespace {

using namespace bounded::literal;

typedef std::vector<SingleClassificationEVs> Single;
typedef std::vector<DataPoint> Estimates;
typedef std::map<Nature, Estimates> AllPossible;
AllPossible combine_results(Single const & physical, Single const & special, EV::total_type max_evs);

DefensiveEVs::BestPerNature best_possible_per_nature(AllPossible all, Pokemon const & pokemon);
std::set<Nature> used_natures(DefensiveEVs::BestPerNature const & container);

void filter_to_minimum_evs(AllPossible & all);
void minimum_evs_per_nature(Estimates & original);

DefensiveEVs::BestPerNature most_effective_equal_evs(AllPossible const & all, Pokemon const & pokemon);
DefensiveEVs::BestPerNature::mapped_type most_effective_equal_evs_per_nature(Estimates const & original, Pokemon const & pokemon);

EV::total_type defensive_evs_available(Pokemon const & pokemon);

typedef std::vector<std::vector<Nature>> Divided;
Divided divide_natures(DefensiveEVs::BestPerNature const & container);

bool has_same_effect_on_defenses(Nature nature, Nature reference_nature);

}	// namespace

DefensiveEVs::DefensiveEVs(Pokemon const & pokemon) {
	Single const physical = equal_defensiveness<true>(pokemon);
	Single const special = equal_defensiveness<false>(pokemon);
	auto const max_evs = defensive_evs_available(pokemon);
	container = best_possible_per_nature(combine_results(physical, special, max_evs), pokemon);
	for (auto const & value : divide_natures(container)) {
		remove_inefficient_natures(value);
	}
	add_other_potential_natures();
	assert(!container.empty());
}

void DefensiveEVs::remove_inefficient_natures(std::vector<Nature> const & divided_natures) {
	typedef std::vector<BestPerNature::const_iterator> Boosters;
	Boosters boosters;
	for (BestPerNature::const_iterator it = container.begin(); it != container.end(); ++it) {
		if (std::find(std::begin(divided_natures), std::end(divided_natures), it->first) != std::end(divided_natures)) {
			boosters.emplace_back(it);
		}
	}
	auto const iter_sum = [](Boosters::value_type const & lhs, Boosters::value_type const & rhs) {
		return lhs->second.sum() < rhs->second.sum();
	};
	auto const best = std::min_element(boosters.begin(), boosters.end(), iter_sum);
	for (auto const & value : divided_natures) {
		if (value != (*best)->first) {
			container.erase(value);
		}
	}
}

void DefensiveEVs::add_other_potential_natures() {
	auto const used = used_natures(container);
	for (auto const reference_nature : used) {
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
	return (boosts_stat<StatNames::DEF>(nature) and boosts_stat<StatNames::DEF>(reference_nature))
			or (boosts_stat<StatNames::SPD>(nature) and boosts_stat<StatNames::SPD>(reference_nature))
			or (!boosts_defending_stat(nature) and !boosts_defending_stat(reference_nature));
}

bool penalizes_same(Nature const nature, Nature const reference_nature) {
	return (lowers_stat<StatNames::DEF>(nature) and lowers_stat<StatNames::DEF>(reference_nature))
			or (lowers_stat<StatNames::SPD>(nature) and lowers_stat<StatNames::SPD>(reference_nature))
			or (!lowers_defending_stat(nature) and !lowers_defending_stat(reference_nature));
}

AllPossible combine_results(Single const & physical, Single const & special, EV::total_type const max_evs) {
	AllPossible all;
	for (auto const & p : physical) {
		for (auto const & s : special) {
			if (are_compatible(p, s, max_evs)) {
				all[DataPoint::get_nature(p, s)].emplace_back(p, s);
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
		minimum_evs_per_nature(per_nature.second);
	}
}

void minimum_evs_per_nature(Estimates & original) {
	static constexpr auto least_sum = [](DataPoint const & value, DataPoint const & least) {
		return value.sum() < least.sum();
	};
	auto const it = std::min_element(std::begin(original), std::end(original), least_sum);
	auto const invalid = [it](DataPoint const & value) {
		return value.sum() != it->sum();
	};
	original.erase(std::remove_if(std::begin(original), std::end(original), invalid), std::end(original));
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

EV::total_type defensive_evs_available(Pokemon const & pokemon) {
	EV::total_type used_evs = 0_bi;
	for (auto const stat : { StatNames::ATK, StatNames::SPA, StatNames::SPE }) {
		used_evs += get_stat(pokemon, stat).ev.value();
	}
	return bounded::make<EV::max_total>() - used_evs;
}

Divided divide_natures(DefensiveEVs::BestPerNature const & container) {
	enum Divisions { Boost, Penalty, Both, Neutral };
	Divided divided(4);
	for (auto const & value : container) {
		Nature const nature(value.first);
		if (boosts_defending_stat(nature) and !lowers_defending_stat(nature)) {
			divided[Boost].emplace_back(value.first);
		}
		else if (lowers_defending_stat(nature) and !boosts_defending_stat(nature)) {
			divided[Penalty].emplace_back(value.first);
		}
		else if (lowers_defending_stat(nature) and boosts_defending_stat(nature)) {
			divided[Both].emplace_back(value.first);
		}
		else {
			divided[Neutral].emplace_back(value.first);
		}
	}
	return divided;
}


}	// namespace
}	// namespace technicalmachine
