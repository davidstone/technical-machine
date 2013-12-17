// Optimize defensive EVs and nature to remove waste
// Copyright (C) 2013 David Stone
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

#include "../../stat/nature.hpp"
#include "../../stat/stat.hpp"

#include "../../string_conversions/nature.hpp"

namespace technicalmachine {
namespace {
Nature::Natures nature_boost_convert(SingleClassificationEVs::NatureBoost nature, bool physical);
SingleClassificationEVs::NatureBoost nature_boost_convert(Nature nature);

constexpr Stat::Stats from_physical(bool physical) {
	return physical ? Stat::DEF : Stat::SPD;
}

template<bool physical>
Stat & defensive_stat(Pokemon & pokemon);

template<>
Stat & defensive_stat<true>(Pokemon & pokemon) {
	return get_stat(pokemon, Stat::DEF);
}
template<>
Stat & defensive_stat<false>(Pokemon & pokemon) {
	return get_stat(pokemon, Stat::SPD);
}

std::string stat_name(bool const physical) {
	return physical ? "Def" : "SpD";
}

}	// namespace

SingleClassificationEVs::SingleClassificationEVs(EV hp_ev, EV defensive_ev, Nature nature, bool is_physical):
	hp(hp_ev),
	defensive(defensive_ev),
	nature_boost(nature_boost_convert(nature)),
	physical(is_physical) {
}

std::string SingleClassificationEVs::to_string() const {
	Nature const nature = nature_boost_convert(nature_boost, physical);
	return ::technicalmachine::to_string(nature.name) + " " + bounded_integer::to_string(hp.value()) + " HP / " + bounded_integer::to_string(defensive.value()) + " " + stat_name(physical);
}

bool are_compatible(SingleClassificationEVs const & physical, SingleClassificationEVs const & special, EV::total_type const max_evs) {
	assert(physical.physical);
	assert(!special.physical);
	if (physical.hp.value() != special.hp.value()) {
		return false;
	}
	bool const evs_over_cap = (physical.hp.value() + physical.defensive.value() + special.defensive.value() > max_evs);
	if (evs_over_cap) {
		return false;
	}
	bool const legal_nature_combination = (physical.nature_boost != special.nature_boost or physical.nature_boost == SingleClassificationEVs::Neutral);
	return legal_nature_combination;
}

namespace {

SingleClassificationEVs::NatureBoost nature_boost_convert(Nature nature) {
	switch (nature.name) {
	case Nature::IMPISH:
	case Nature::CALM:
		return SingleClassificationEVs::Boost;
	case Nature::HASTY:
	case Nature::NAIVE:
		return SingleClassificationEVs::Penalty;
	default:
		assert(nature.name == Nature::HARDY);
		return SingleClassificationEVs::Neutral;
	}
}

Nature::Natures nature_boost_convert(SingleClassificationEVs::NatureBoost nature, bool physical) {
	switch (nature) {
	case SingleClassificationEVs::Boost:
		return physical ? Nature::IMPISH : Nature::CALM;
	case SingleClassificationEVs::Penalty:
		return physical ? Nature::HASTY : Nature::NAIVE;
	default:
		return Nature::HARDY;
	}
}
}	// unnamed namespace


template<bool physical>
std::vector<SingleClassificationEVs> equal_defensiveness(Pokemon pokemon) {
	auto const initial_product = get_stat(pokemon, Stat::HP).max * initial_stat<from_physical(physical)>(pokemon);
	std::vector<SingleClassificationEVs> result;
	static std::initializer_list<Nature> const natures = {
		nature_boost_convert(SingleClassificationEVs::Boost, physical),
		nature_boost_convert(SingleClassificationEVs::Neutral, physical),
		nature_boost_convert(SingleClassificationEVs::Penalty, physical)
	};
	for (Nature const nature : natures) {
		get_nature(pokemon) = nature;
		for (EV::value_type hp_ev = 0_bi; ; hp_ev += 4_bi) {
			get_stat(pokemon, Stat::HP).ev = EV(EV::value_type(hp_ev));
			auto const hp = initial_stat<Stat::HP>(pokemon);
			EV::value_type defensive_ev = 0_bi;
			defensive_stat<physical>(pokemon).ev = EV(defensive_ev);
			while (initial_stat<from_physical(physical)>(pokemon) * hp < initial_product and defensive_ev <= EV::max - 4) {
				defensive_ev += 4_bi;
				defensive_stat<physical>(pokemon).ev = EV(defensive_ev);
			}
			if (initial_stat<from_physical(physical)>(pokemon) * hp >= initial_product) {
				result.emplace_back(EV(hp_ev), EV(defensive_ev), nature, physical);
			}
			if (hp_ev == EV::max) {
				break;
			}
		}
	}
	return result;
}
template std::vector<SingleClassificationEVs> equal_defensiveness<true>(Pokemon pokemon);
template std::vector<SingleClassificationEVs> equal_defensiveness<false>(Pokemon pokemon);

}	// namespace technicalmachine
