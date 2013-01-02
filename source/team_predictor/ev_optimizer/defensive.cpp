// Optimize defensive EVs and nature to remove waste
// Copyright (C) 2012 David Stone
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

#include <algorithm>
#include <iostream>
#include <set>
#include <string>
#include <vector>

#include "offensive.hpp"
#include "single_classification_evs.hpp"

#include "../../pokemon/pokemon.hpp"

#include "../../stat/nature.hpp"
#include "../../stat/stat.hpp"

namespace technicalmachine {
namespace {

typedef std::vector<SingleClassificationEVs> Single;
typedef std::vector<DataPoint> Estimates;
typedef std::map<Nature::Natures, Estimates> AllPossible;
AllPossible combine_results(Single const & physical, Single const & special, unsigned max_evs, Pokemon const & pokemon);

DefensiveEVs::BestPerNature best_possible_per_nature(AllPossible all, Pokemon const & pokemon);
std::set<Nature::Natures> used_natures(DefensiveEVs::BestPerNature const & container);

void filter_to_minimum_evs(AllPossible & all);
void minimum_evs_per_nature(Estimates & original);

DefensiveEVs::BestPerNature most_effective_equal_evs(AllPossible const & all, Pokemon const & pokemon);
DefensiveEVs::BestPerNature::mapped_type most_effective_equal_evs_per_nature(Estimates const & original, Pokemon const & pokemon);

unsigned defensive_evs_available(Pokemon const & pokemon);

typedef std::vector<std::vector<Nature::Natures>> Divided;
Divided divide_natures(DefensiveEVs::BestPerNature const & container);

bool has_same_effect_on_defenses(Nature const & nature, Nature const & reference_nature);

}	// unnamed namespace

DefensiveEVs::DefensiveEVs(Pokemon pokemon) {
	Single const physical = equal_defensiveness<true>(pokemon);
	Single const special = equal_defensiveness<false>(pokemon);
	unsigned const max_evs = defensive_evs_available(pokemon);
	container = best_possible_per_nature(combine_results(physical, special, max_evs, pokemon), pokemon);
	for (auto const & value : divide_natures(container)) {
		remove_inefficient_natures(value);
	}
	add_other_potential_natures();
}

void DefensiveEVs::remove_inefficient_natures(std::vector<Nature::Natures> const & divided_natures) {
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
		for (Stat::Stats boosted = static_cast<Stat::Stats>(0); boosted != Stat::NORMAL_END; boosted = static_cast<Stat::Stats>(boosted + 1)) {
			for (Stat::Stats penalized = static_cast<Stat::Stats>(0); penalized != Stat::NORMAL_END; penalized = static_cast<Stat::Stats>(penalized + 1)) {
				Nature const nature(boosted, penalized);
				if (!has_same_effect_on_defenses(nature, reference_nature)) {
					continue;
				}
				if (nature.name == reference_nature) {
					continue;
				}
				DataPoint const new_data_point(container.at(reference_nature), nature);
				container.insert(std::make_pair(nature.name, new_data_point));
			}
		}
	}
}

namespace {

std::set<Nature::Natures> used_natures(DefensiveEVs::BestPerNature const & container) {
	std::set<Nature::Natures> used;
	for (auto const & value : container) {
		used.insert(value.first);
	}
	return used;
}

bool boosts_same(Nature const & nature, Nature const & reference_nature);
bool penalizes_same(Nature const & nature, Nature const & reference_nature);

bool has_same_effect_on_defenses(Nature const & nature, Nature const & reference_nature) {
	bool const result = boosts_same(nature, reference_nature) and penalizes_same(nature, reference_nature);
	return result;
}

bool boosts_same(Nature const & nature, Nature const & reference_nature) {
	return (nature.boosts_stat<Stat::DEF>() and reference_nature.boosts_stat<Stat::DEF>())
			or (nature.boosts_stat<Stat::SPD>() and reference_nature.boosts_stat<Stat::SPD>())
			or (!boosts_defending_stat(nature) and !boosts_defending_stat(reference_nature));
}

bool penalizes_same(Nature const & nature, Nature const & reference_nature) {
	return (nature.lowers_stat<Stat::DEF>() and reference_nature.lowers_stat<Stat::DEF>())
			or (nature.lowers_stat<Stat::SPD>() and reference_nature.lowers_stat<Stat::SPD>())
			or (!lowers_defending_stat(nature) and !lowers_defending_stat(reference_nature));
}

AllPossible combine_results(Single const & physical, Single const & special, unsigned max_evs, Pokemon const & pokemon) {
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
		result.insert(std::make_pair(per_nature.first, most_effective_equal_evs_per_nature(per_nature.second, pokemon)));
	}
	return result;
}

DefensiveEVs::BestPerNature::mapped_type most_effective_equal_evs_per_nature(Estimates const & original, Pokemon const & pokemon) {
	auto const greatest_product = [& pokemon](DataPoint const & largest, DataPoint const & value) {
		return lesser_product(largest, value, pokemon);
	};
	return *std::max_element(std::begin(original), std::end(original), greatest_product);
}

unsigned defensive_evs_available(Pokemon const & pokemon) {
	constexpr unsigned max_total_evs = 510;
	auto used_evs = 0u;
	for (auto const stat : { Stat::ATK, Stat::SPA, Stat::SPE }) {
		used_evs += pokemon.stat(stat).ev.value();
	}
	return max_total_evs - used_evs;
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


}	// unnamed namespace
}	// namespace technicalmachine
