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

#include "offensive.hpp"

#include "../../pokemon/pokemon.hpp"

#include "../../stat/nature.hpp"
#include "../../stat/stat.hpp"

#include <iostream>

namespace technicalmachine {
namespace {

bool boosts_defensive_stat(Nature nature);
bool weakens_defensive_stat(Nature nature);

unsigned defensive_evs_available(Pokemon const & pokemon);

bool affects_defensive_stat(Nature const nature, bool const boost);

}	// unnamed namespace

DefensiveEVs::DefensiveEVs(Pokemon pokemon) {
	Single const physical = equal_defensiveness<true>(pokemon);
	Single const special = equal_defensiveness<false>(pokemon);
	unsigned const max_evs = defensive_evs_available(pokemon);
	combine_results(physical, special, max_evs, pokemon);
	for (auto const & per_nature : container) {
		for (auto const & stat : per_nature.second)
			std::cerr << stat.to_string() + '\n';
	}
	std::cerr << "Reduced to:\n";
	for (bool const boost_nature : { true, false })
		remove_inefficient_natures(boost_nature);
	for (auto const & per_nature : container) {
		for (auto const & stat : per_nature.second)
			std::cerr << stat.to_string() + '\n';
	}
}

void DefensiveEVs::combine_results(Single const & physical, Single const & special, unsigned max_evs, Pokemon const & pokemon) {
	for (auto const & p : physical) {
		for (auto const & s : special) {
			if (are_compatible(p, s, max_evs)) {
				container[DataPoint::get_nature(p, s)].emplace_back(p, s);
			}
		}
	}
	filter_to_minimum_evs();
	most_effective_equal_evs(pokemon);
}

void DefensiveEVs::filter_to_minimum_evs() {
	for (auto & per_nature : container) {
		minimum_evs_per_nature(per_nature.second);
	}
}

void DefensiveEVs::minimum_evs_per_nature(Estimates & original) {
	static auto const least_sum = [](DataPoint const & value, DataPoint const & least) {
		return value.sum() < least.sum();
	};
	auto const it = std::min_element(std::begin(original), std::end(original), least_sum);
	auto const invalid = [it](DataPoint const & value) {
		return value.sum() != it->sum();
	};
	original.erase(std::remove_if(std::begin(original), std::end(original), invalid), std::end(original));
}

void DefensiveEVs::most_effective_equal_evs(Pokemon const & pokemon) {
	for (auto & per_nature : container) {
		most_effective_equal_evs_per_nature(per_nature.second, pokemon);
	}
}

void DefensiveEVs::most_effective_equal_evs_per_nature(Estimates & original, Pokemon const & pokemon) {
	auto const greatest_product = [& pokemon](DataPoint const & largest, DataPoint const & value) {
		return lesser_product(largest, value, pokemon);
	};
	auto const it = std::max_element(std::begin(original), std::end(original), greatest_product);
	original.front() = *it;
	original.erase(std::begin(original) + 1, std::end(original));
}

void DefensiveEVs::remove_inefficient_natures(bool boost_nature) {
	typedef std::vector<Container::const_iterator> Boosters;
	Boosters boosters;
	for (Container::const_iterator it = container.begin(); it != container.end(); ++it) {
		if (affects_defensive_stat(it->first, boost_nature))
			boosters.emplace_back(it);
	}
	auto const iter_sum = [](Boosters::value_type const & lhs, Boosters::value_type const & rhs) {
		return lhs->second.front().sum() < rhs->second.front().sum();
	};
	auto const best = std::min_element(boosters.begin(), boosters.end(), iter_sum);
	for (Container::iterator it = container.begin(); it != container.end();) {
		if (affects_defensive_stat(it->first, boost_nature) and it != *best)
			it = container.erase(it);
		else
			++it;
	}
}

namespace {

unsigned defensive_evs_available(Pokemon const & pokemon) {
	constexpr unsigned max_total_evs = 510;
	return max_total_evs - pokemon.atk().ev.value() + pokemon.spa().ev.value() + pokemon.spe().ev.value();
}

bool affects_defensive_stat(Nature const nature, bool const boost) {
	if (boost) {
		switch (nature.name) {
		case Nature::CALM:
		case Nature::IMPISH:
		case Nature::GENTLE:
		case Nature::LAX:
			return true;
		default:
			return false;
		}
	}
	else {
		switch (nature.name) {
		case Nature::HASTY:
		case Nature::NAIVE:
			return true;
		default:
			return false;
		}
	}
}
}	// unnamed namespace
}	// namespace technicalmachine
