// Optimize defensive EVs and nature to remove waste
// Copyright (C) 2015 David Stone
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

#include "single_classification_evs.hpp"

#include <cassert>
#include <initializer_list>
#include <string>
#include <vector>

#include "../../pokemon/pokemon.hpp"

#include "../../stat/calculate.hpp"
#include "../../stat/nature.hpp"
#include "../../stat/stat.hpp"

#include "../../string_conversions/nature.hpp"

namespace technicalmachine {
namespace {

constexpr auto nature_boost_convert(Nature const nature) {
	switch (nature) {
	case Nature::Impish:
	case Nature::Calm:
		return SingleClassificationEVs::Boost;
	case Nature::Hasty:
	case Nature::Naive:
		return SingleClassificationEVs::Penalty;
	default:
		assert(nature == Nature::Hardy);
		return SingleClassificationEVs::Neutral;
	}
}

constexpr StatNames from_physical(bool physical) {
	return physical ? StatNames::DEF : StatNames::SPD;
}

}	// namespace

SingleClassificationEVs::SingleClassificationEVs(EV hp_ev, EV defensive_ev, Nature nature):
	hp(hp_ev),
	defensive(defensive_ev),
	nature_boost(nature_boost_convert(nature)) {
}

bool are_compatible(SingleClassificationEVs const & physical, SingleClassificationEVs const & special) {
	if (physical.hp.value() != special.hp.value()) {
		return false;
	}
	return physical.nature_boost != special.nature_boost or physical.nature_boost == SingleClassificationEVs::Neutral;
}

namespace {

constexpr auto nature_boost_convert(bool physical) {
	return physical ?
		bounded::make_array(Nature::Impish, Nature::Hardy, Nature::Hasty) :
		bounded::make_array(Nature::Calm, Nature::Hardy, Nature::Naive);
}

}	// namespace


template<bool physical>
std::vector<SingleClassificationEVs> equal_defensiveness(Pokemon const & pokemon) {
	static constexpr StatNames stat_name = from_physical(physical);
	Stat stat = get_stat(pokemon, stat_name);
	Level const level = get_level(pokemon);
	auto const initial_product = get_hp(pokemon).max() * initial_stat<stat_name>(stat, level, get_nature(pokemon));
	std::vector<SingleClassificationEVs> result;
	for (Nature const nature : nature_boost_convert(physical)) {
		for (EV::value_type hp_ev = 0_bi; ; hp_ev += 4_bi) {
			HP const hp(pokemon, level, EV(hp_ev));
			stat.ev = EV(0_bi);
			while (initial_stat<stat_name>(stat, level, nature) * hp.max() < initial_product) {
				stat.ev.add(4_bi);
				if (stat.ev.value() == EV::max) {
					break;
				}
			}
			if (initial_stat<stat_name>(stat, level, nature) * hp.max() >= initial_product) {
				result.emplace_back(hp.ev, stat.ev, nature);
			}
			if (hp_ev == EV::max) {
				break;
			}
		}
	}
	return result;
}
template std::vector<SingleClassificationEVs> equal_defensiveness<true>(Pokemon const & pokemon);
template std::vector<SingleClassificationEVs> equal_defensiveness<false>(Pokemon const & pokemon);

}	// namespace technicalmachine
