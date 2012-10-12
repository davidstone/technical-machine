// Optimize EVs and nature to remove waste
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

#include "ev_optimizer.hpp"
#include <array>
#include <cassert>
#include <string>
#include "../pokemon/pokemon.hpp"
#include "../stat/nature.hpp"
#include "../stat/stat.hpp"

#include <iostream>

namespace technicalmachine {
namespace {
enum NatureBoost { Penalty, Neutral, Boost };

template<Stat::Stats stat>
Nature::Natures nature_effect(NatureBoost nature);

class DefensiveEVs {
	public:
		DefensiveEVs(unsigned hp_ev, unsigned defensive_ev, NatureBoost nature);
		template<Stat::Stats stat>
		std::string to_string(std::string const & stat_name) const {
			Nature const nature = nature_effect<stat>(nature_boost);
			return nature.to_string() + " " + std::to_string(hp) + " HP / " + std::to_string(defensive) + " " + stat_name;
		}
		static NatureBoost convert(Nature nature);
		unsigned hp;
		unsigned defensive;
		NatureBoost nature_boost;
};

void remove_unused_offensive_evs(Pokemon & pokemon, bool & lower_attack, bool & lower_special_attack);
bool has_physical_move(Pokemon const & pokemon);
bool has_special_move(Pokemon const & pokemon);
void remove_defensive_waste(Pokemon & pokemon);
template<Stat::Stats stat>
void set_ev(Pokemon & pokemon, unsigned const defensive_ev);
}	// unnamed namespace

void optimize_evs(Pokemon & pokemon) {
	bool lower_attack = false;
	bool lower_special_attack = false;
	remove_unused_offensive_evs(pokemon, lower_attack, lower_special_attack);
	remove_defensive_waste(pokemon);
}

namespace {

void remove_unused_offensive_evs(Pokemon & pokemon, bool & lower_attack, bool & lower_special_attack) {
	if (!has_physical_move(pokemon)) {
		pokemon.atk.ev.set_value(0);
		lower_attack = true;
	}
	if (!has_special_move(pokemon)) {
		pokemon.spa.ev.set_value(0);
		lower_special_attack = true;
	}
}

bool has_physical_move(Pokemon const & pokemon) {
	return pokemon.move.regular_move_exists([](Move const & move) {
		return move.is_physical();
	});
}

bool has_special_move(Pokemon const & pokemon) {
	return pokemon.move.regular_move_exists([](Move const & move) {
		return move.is_special();
	});
}

template<Stat::Stats stat>
std::vector<DefensiveEVs> defensiveness(Pokemon pokemon) {
	std::cerr << pokemon.to_string() << '\n';
	std::cerr << "HP: " << pokemon.hp.max << '\n';
	std::cerr << "Defense: " << initial_stat<stat>(pokemon) << '\n';
	unsigned const initial_product = pokemon.hp.max * initial_stat<stat>(pokemon);
	std::cerr << "initial_product: " << initial_product << '\n';
	std::vector<DefensiveEVs> result;
	static std::array<Nature, 3> const natures = {{
		nature_effect<stat>(Boost),
		nature_effect<stat>(Neutral),
		nature_effect<stat>(Penalty)
	}};
	for (Nature const nature : natures) {
		for (unsigned hp_ev = 0; hp_ev <= 252; hp_ev += 4) {
			pokemon.hp.ev.set_value(hp_ev);
			unsigned const hp = initial_stat<Stat::HP>(pokemon);
			unsigned defensive_ev = 0;
			set_ev<stat>(pokemon, defensive_ev);
			while (initial_stat<stat>(pokemon) * hp < initial_product and defensive_ev <= 252) {
				defensive_ev += 4;
				set_ev<stat>(pokemon, defensive_ev);
			}
			if (initial_stat<stat>(pokemon) * hp >= initial_product) {
				auto const nature_boost = DefensiveEVs::convert(nature);
				if (!result.empty()) {
					auto const & old = result.back();
					if (nature_boost == old.nature_boost and (hp_ev - old.hp <= old.defensive - defensive_ev))
						result.pop_back();
				}
				result.emplace_back(hp_ev, defensive_ev, nature_boost);
			}
		}
	}
	return result;
}

void remove_defensive_waste(Pokemon & pokemon) {
	std::vector<DefensiveEVs> physical = defensiveness<Stat::DEF>(pokemon);
	for (auto const & stat : physical)
		std::cerr << stat.to_string<Stat::DEF>("Def") + '\n';
	
}

template<>
void set_ev<Stat::DEF>(Pokemon & pokemon, unsigned const defensive_ev) {
	pokemon.def.ev.set_value(defensive_ev);
}
template<>
void set_ev<Stat::SPD>(Pokemon & pokemon, unsigned const defensive_ev) {
	pokemon.spd.ev.set_value(defensive_ev);
}

DefensiveEVs::DefensiveEVs(unsigned hp_ev, unsigned defensive_ev, NatureBoost nature):
	hp(hp_ev),
	defensive(defensive_ev),
	nature_boost(nature) {
}

NatureBoost DefensiveEVs::convert(Nature nature) {
	switch (nature.name) {
		case Nature::IMPISH:
		case Nature::CALM:
			return Boost;
		case Nature::HASTY:
		case Nature::NAIVE:
			return Penalty;
		default:
			assert(nature.name == Nature::HARDY);
			return Neutral;
	}
}

template<>
Nature::Natures nature_effect<Stat::DEF>(NatureBoost nature) {
	switch (nature) {
		case Boost:
			return Nature::IMPISH;
		case Penalty:
			return Nature::HASTY;
		default:
			return Nature::HARDY;
	}
}
template<>
Nature::Natures nature_effect<Stat::SPD>(NatureBoost nature) {
	switch (nature) {
		case Boost:
			return Nature::CALM;
		case Penalty:
			return Nature::NAIVE;
		default:
			return Nature::HARDY;
	}
}

}	// unnamed namespace
}	// namespace technicalmachine
