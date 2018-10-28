// Optimize EVs and nature to remove waste
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

#pragma once

#include <tm/pokemon/level.hpp>
#include <tm/pokemon/species_forward.hpp>

#include <tm/stat/ev.hpp>
#include <tm/stat/nature.hpp>
#include <tm/stat/stat_names.hpp>

#include <tm/operators.hpp>

#include <containers/array/array.hpp>

#include <random>

namespace technicalmachine {
struct Pokemon;

struct Combined {
	Nature nature;
	EV hp;
	EV attack;
	EV defense;
	EV special_attack;
	EV special_defense;
	EV speed;
};
inline auto operator==(Combined const lhs, Combined const rhs) {
	return
		lhs.nature == rhs.nature and
		lhs.hp == rhs.hp and
		lhs.attack == rhs.attack and
		lhs.defense == rhs.defense and
		lhs.special_attack == rhs.special_attack and
		lhs.special_defense == rhs.special_defense and
		lhs.speed == rhs.speed;
}

constexpr auto regular_stats() {
	return containers::array{StatNames::ATK, StatNames::DEF, StatNames::SPA, StatNames::SPD, StatNames::SPE};
}

auto pull_out_stats(Pokemon const & pokemon) -> Combined;

void optimize_evs(Pokemon & pokemon, std::mt19937 & random_engine);
auto minimize_evs(Combined stats, Species, Level, bool include_attack, bool include_special_attack) -> Combined;
auto pad_random_evs(Combined combined, bool include_attack, bool include_special_attack, std::mt19937 & random_engine) -> Combined;

}	// namespace technicalmachine
