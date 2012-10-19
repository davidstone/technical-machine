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

#include <cassert>
#include <initializer_list>
#include <string>
#include <vector>

#include "../../pokemon/pokemon.hpp"

#include "../../stat/nature.hpp"
#include "../../stat/stat.hpp"

namespace technicalmachine {
namespace {
Nature::Natures nature_boost_convert(SingleClassificationEVs::NatureBoost nature, bool physical);
SingleClassificationEVs::NatureBoost nature_boost_convert(Nature nature);

constexpr Stat::Stats from_physical(bool physical) {
	return physical ? Stat::DEF : Stat::SPD;
}

template<bool physical>
void set_ev(Pokemon & pokemon, unsigned const defensive_ev) {
	Stat & stat = physical ? pokemon.def() : pokemon.spd();
	stat.ev.set_value(defensive_ev);
}

}	// unnamed namespace

SingleClassificationEVs::SingleClassificationEVs(unsigned hp_ev, unsigned defensive_ev, Nature nature, bool is_physical):
	hp(hp_ev),
	defensive(defensive_ev),
	nature_boost(nature_boost_convert(nature)),
	physical(is_physical) {
}

std::string SingleClassificationEVs::to_string() const {
	Nature const nature = nature_boost_convert(nature_boost, physical);
	return nature.to_string() + " " + std::to_string(hp) + " HP / " + std::to_string(defensive) + " " + stat_name();
}

std::string SingleClassificationEVs::stat_name() const {
	return physical ? "Def" : "SpD";
}

bool are_compatible(SingleClassificationEVs const & physical, SingleClassificationEVs const & special, unsigned max_evs) {
	assert(physical.physical);
	assert(!special.physical);
	if (physical.hp != special.hp)
		return false;
	bool const evs_over_cap = (physical.hp + physical.defensive + special.defensive > max_evs);
	if (evs_over_cap)
		return false;
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
	unsigned const initial_product = pokemon.hp().max * initial_stat<from_physical(physical)>(pokemon);
	std::vector<SingleClassificationEVs> result;
	static std::initializer_list<Nature> const natures = {
		nature_boost_convert(SingleClassificationEVs::Boost, physical),
		nature_boost_convert(SingleClassificationEVs::Neutral, physical),
		nature_boost_convert(SingleClassificationEVs::Penalty, physical)
	};
	for (Nature const nature : natures) {
		pokemon.nature() = nature;
		for (unsigned hp_ev = 0; hp_ev <= 252; hp_ev += 4) {
			pokemon.hp().ev.set_value(hp_ev);
			unsigned const hp = initial_stat<Stat::HP>(pokemon);
			unsigned defensive_ev = 0;
			set_ev<physical>(pokemon, defensive_ev);
			while (initial_stat<from_physical(physical)>(pokemon) * hp < initial_product and defensive_ev <= 252 - 4) {
				defensive_ev += 4;
				set_ev<physical>(pokemon, defensive_ev);
			}
			if (initial_stat<from_physical(physical)>(pokemon) * hp >= initial_product) {
				result.emplace_back(hp_ev, defensive_ev, nature, physical);
			}
		}
	}
	return result;
}
template std::vector<SingleClassificationEVs> equal_defensiveness<true>(Pokemon pokemon);
template std::vector<SingleClassificationEVs> equal_defensiveness<false>(Pokemon pokemon);

}	// namespace technicalmachine
