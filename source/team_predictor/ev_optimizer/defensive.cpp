// Optimize defensive EVs and nature to remove waste
// Copyright (C) 2018 David Stone
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

#include "../../pokemon/pokemon.hpp"

#include "../../stat/calculate.hpp"
#include "../../stat/nature.hpp"
#include "../../stat/stat_names.hpp"

#include <containers/algorithms/find.hpp>
#include <containers/integer_range.hpp>

#include <cassert>

namespace technicalmachine {
namespace {

using namespace bounded::literal;

constexpr auto ev_sum(DataPoint const data) {
	return data.hp.value() + data.defense.value() + data.special_defense.value();
}

}	// namespace

DefensiveEVs::DefensiveEVs(Pokemon const & pokemon) {
	auto const species = static_cast<Species>(pokemon);
	auto const level = get_level(pokemon);
	auto const original_nature = get_nature(pokemon);
	auto const original_hp = get_hp(pokemon);
	auto const defense = get_stat(pokemon, StatNames::DEF);
	auto const special_defense = get_stat(pokemon, StatNames::SPD);

	auto product = [=](Nature const nature, HP const hp, StatNames const name, Stat const stat) {
		return hp.max() * initial_stat(name, stat, level, nature);
	};
	auto initial_product = [=](StatNames const name, Stat const stat) {
		return product(original_nature, original_hp, name, stat);
	};
	auto const defense_product = initial_product(StatNames::DEF, defense);
	auto const special_defense_product = initial_product(StatNames::SPD, special_defense);

	auto defensive_product = [=](DataPoint const value) {
		auto const hp = HP(species, level, value.hp);
		auto single_product = [=](StatNames const stat, EV const ev) {
			return product(value.nature, hp, stat, Stat(species, stat, ev));
		};

		return single_product(StatNames::DEF, value.defense) * single_product(StatNames::SPD, value.special_defense);
	};

	for (auto const nature : containers::enum_range<Nature>()) {
		constexpr auto ev_range = containers::transform(
			containers::integer_range(0_bi, EV::max + 4_bi, 4_bi),
			[](auto const ev) { return EV(ev); }
		);
		auto best_per_nature = bounded::optional<DataPoint>{};
		for (auto const hp_ev : ev_range) {
			auto const hp = HP(species, level, hp_ev, original_hp.iv());
			auto find_minimum_matching = [=](StatNames const stat_name, Stat const stat, auto const original_product) {
				return containers::maybe_find_if(ev_range, [=](EV const ev) {
					return product(nature, hp, stat_name, Stat(stat, ev)) >= original_product;
				});
			};

			auto const defense_ev = find_minimum_matching(StatNames::DEF, defense, defense_product);
			if (!defense_ev) {
				continue;
			}
			auto const special_defense_ev = find_minimum_matching(StatNames::SPD, special_defense, special_defense_product);
			if (!special_defense_ev) {
				continue;
			}
			auto is_better = [=](DataPoint const candidate, DataPoint const current) {
				auto const cmp = compare(ev_sum(candidate), ev_sum(current));
				if (cmp < 0) {
					return true;
				} else if (cmp > 0) {
					return false;
				}
				auto const candidate_product = defensive_product(candidate);
				auto const current_product = defensive_product(current);
				return candidate_product > current_product;
			};
			auto const candidate = DataPoint{nature, hp_ev, *defense_ev, *special_defense_ev};
			if (!best_per_nature or is_better(candidate, *best_per_nature)) {
				best_per_nature.emplace(candidate);
			}
		}
		if (best_per_nature) {
			push_back(m_container, *best_per_nature);
		}
	}
	assert(!empty(m_container));
}

}	// namespace technicalmachine
