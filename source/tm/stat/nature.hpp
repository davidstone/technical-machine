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

#include <tm/stat/stat_names.hpp>

#include <tm/enum.hpp>
#include <tm/rational.hpp>

#include <bounded/integer.hpp>

#include <cstdint>

namespace technicalmachine {
using namespace bounded::literal;

enum class Nature : std::uint8_t {
	Adamant, Bashful, Bold, Brave, Calm,
	Careful, Docile, Gentle, Hardy, Hasty,
	Impish, Jolly, Lax, Lonely, Mild, Modest,
	Naive, Naughty, Quiet, Quirky, Rash,
	Relaxed, Sassy, Serious, Timid
};

constexpr auto boosts_stat(Nature const nature, StatNames const stat) -> bool {
	switch (nature) {
		case Nature::Adamant:
		case Nature::Brave:
		case Nature::Lonely:
		case Nature::Naughty:
			return stat == StatNames::ATK;
		case Nature::Bold:
		case Nature::Impish:
		case Nature::Lax:
		case Nature::Relaxed:
			return stat == StatNames::DEF;
		case Nature::Mild:
		case Nature::Modest:
		case Nature::Quiet:
		case Nature::Rash:
			return stat == StatNames::SPA;
		case Nature::Calm:
		case Nature::Careful:
		case Nature::Gentle:
		case Nature::Sassy:
			return stat == StatNames::SPD;
		case Nature::Hasty:
		case Nature::Jolly:
		case Nature::Naive:
		case Nature::Timid:
			return stat == StatNames::SPE;
		case Nature::Bashful:
		case Nature::Docile:
		case Nature::Hardy:
		case Nature::Quirky:
		case Nature::Serious:
			return false;
	}
}

constexpr auto lowers_stat(Nature const nature, StatNames const stat) -> bool {
	switch (nature) {
		case Nature::Bold:
		case Nature::Calm:
		case Nature::Modest:
		case Nature::Timid:
			return stat == StatNames::ATK;
		case Nature::Gentle:
		case Nature::Hasty:
		case Nature::Lonely:
		case Nature::Mild:
			return stat == StatNames::DEF;
		case Nature::Adamant:
		case Nature::Careful:
		case Nature::Impish:
		case Nature::Jolly:
			return stat == StatNames::SPA;
		case Nature::Lax:
		case Nature::Naive:
		case Nature::Naughty:
		case Nature::Rash:
			return stat == StatNames::SPD;
		case Nature::Brave:
		case Nature::Quiet:
		case Nature::Relaxed:
		case Nature::Sassy:
			return stat == StatNames::SPE;
		case Nature::Bashful:
		case Nature::Docile:
		case Nature::Hardy:
		case Nature::Quirky:
		case Nature::Serious:
			return false;
	}
}

constexpr auto boosts_attacking_stat(Nature const nature) -> bool {
	return boosts_stat(nature, StatNames::ATK) or boosts_stat(nature, StatNames::SPA);
}

constexpr auto boosts_defending_stat(Nature const nature) -> bool {
	return boosts_stat(nature, StatNames::DEF) or boosts_stat(nature, StatNames::SPD);
}

constexpr auto lowers_attacking_stat(Nature const nature) -> bool {
	return lowers_stat(nature, StatNames::ATK) or lowers_stat(nature, StatNames::SPA);
}

constexpr auto lowers_defending_stat(Nature const nature) -> bool {
	return lowers_stat(nature, StatNames::DEF) or lowers_stat(nature, StatNames::SPD);
}

constexpr auto boost(Nature const nature, StatNames const stat) {
	auto const numerator =
		BOUNDED_CONDITIONAL(boosts_stat(nature, stat), 11_bi,
		BOUNDED_CONDITIONAL(lowers_stat(nature, stat), 9_bi,
		10_bi
	));
	return rational(numerator, 10_bi);
}

}	// namespace technicalmachine

namespace std {

template<>
struct hash<technicalmachine::Nature> : public technicalmachine::std_hash<technicalmachine::Nature> {};

template<>
struct numeric_limits<technicalmachine::Nature> : technicalmachine::enum_numeric_limits<technicalmachine::Nature::Timid> {};

}	// namespace std

