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
#include <string>
#include <vector>

#include "offensive.hpp"
#include "single_classification_evs.hpp"

#include "../../pokemon/pokemon.hpp"

#include "../../stat/nature.hpp"
#include "../../stat/stat.hpp"

#include <iostream>

namespace technicalmachine {
namespace {

typedef std::vector<SingleClassificationEVs> Single;
typedef std::vector<DataPoint> Estimates;
typedef std::map<Nature::Natures, Estimates> AllPossible;
AllPossible combine_results(Single const & physical, Single const & special, unsigned max_evs, Pokemon const & pokemon);

DefensiveEVs::BestPerNature best_possible_per_nature(AllPossible all, Pokemon const & pokemon);

void filter_to_minimum_evs(AllPossible & all);
void minimum_evs_per_nature(Estimates & original);

DefensiveEVs::BestPerNature most_effective_equal_evs(AllPossible const & all, Pokemon const & pokemon);
DefensiveEVs::BestPerNature::mapped_type most_effective_equal_evs_per_nature(Estimates const & original, Pokemon const & pokemon);

unsigned defensive_evs_available(Pokemon const & pokemon);

}	// unnamed namespace

DefensiveEVs::DefensiveEVs(Pokemon pokemon) {
	Single const physical = equal_defensiveness<true>(pokemon);
	Single const special = equal_defensiveness<false>(pokemon);
	unsigned const max_evs = defensive_evs_available(pokemon);
	container = best_possible_per_nature(combine_results(physical, special, max_evs, pokemon), pokemon);
	for (bool const boost_nature : { true, false })
		remove_inefficient_natures(boost_nature);
	for (auto const & per_nature : container) {
		auto const & stat = per_nature.second;
		std::cerr << stat.to_string() + '\n';
	}
}

void DefensiveEVs::remove_inefficient_natures(bool boost_nature) {
	typedef std::vector<BestPerNature::const_iterator> Boosters;
	Boosters boosters;
	for (BestPerNature::const_iterator it = container.begin(); it != container.end(); ++it) {
		if (it->second.affects_defensive_stat(boost_nature))
			boosters.emplace_back(it);
	}
	auto const iter_sum = [](Boosters::value_type const & lhs, Boosters::value_type const & rhs) {
		return lhs->second.sum() < rhs->second.sum();
	};
	auto const best = std::min_element(boosters.begin(), boosters.end(), iter_sum);
	for (BestPerNature::iterator it = container.begin(); it != container.end();) {
		if (it->second.affects_defensive_stat(boost_nature) and it != *best)
			it = container.erase(it);
		else
			++it;
	}
}

namespace {

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
	static auto const least_sum = [](DataPoint const & value, DataPoint const & least) {
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
	return max_total_evs - pokemon.atk().ev.value() + pokemon.spa().ev.value() + pokemon.spe().ev.value();
}

}	// unnamed namespace
}	// namespace technicalmachine
