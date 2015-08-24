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
Nature nature_boost_convert(SingleClassificationEVs::NatureBoost nature, bool physical);
SingleClassificationEVs::NatureBoost nature_boost_convert(Nature nature);

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

SingleClassificationEVs::NatureBoost nature_boost_convert(Nature const nature) {
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

Nature nature_boost_convert(SingleClassificationEVs::NatureBoost nature, bool physical) {
	switch (nature) {
	case SingleClassificationEVs::Boost:
		return physical ? Nature::Impish : Nature::Calm;
	case SingleClassificationEVs::Penalty:
		return physical ? Nature::Hasty : Nature::Naive;
	default:
		return Nature::Hardy;
	}
}
}	// namespace


template<bool physical>
std::vector<SingleClassificationEVs> equal_defensiveness(Pokemon const & pokemon) {
	static constexpr StatNames stat_name = from_physical(physical);
	Stat stat = get_stat(pokemon, stat_name);
	Level const level = get_level(pokemon);
	Nature current_nature = get_nature(pokemon);
	HP const & initial_hp = get_hp(pokemon);
	auto const initial_product = initial_hp.max() * initial_stat<stat_name>(stat, level, current_nature);
	std::vector<SingleClassificationEVs> result;
	static std::initializer_list<Nature> const natures = {
		nature_boost_convert(SingleClassificationEVs::Boost, physical),
		nature_boost_convert(SingleClassificationEVs::Neutral, physical),
		nature_boost_convert(SingleClassificationEVs::Penalty, physical)
	};
	for (Nature const nature : natures) {
		current_nature = nature;
		for (EV::value_type hp_ev = 0_bi; ; hp_ev += 4_bi) {
			HP const hp(pokemon, level, EV(EV::value_type(hp_ev)));
			EV::value_type defensive_ev = 0_bi;
			stat.ev = EV(defensive_ev);
			while (initial_stat<stat_name>(stat, level, current_nature) * hp.max() < initial_product) {
				defensive_ev += 4_bi;
				stat.ev = EV(defensive_ev);
				if (defensive_ev == bounded::constant<EV::max>) {
					break;
				}
			}
			if (initial_stat<stat_name>(stat, level, current_nature) * hp.max() >= initial_product) {
				result.emplace_back(EV(hp_ev), EV(defensive_ev), nature);
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
