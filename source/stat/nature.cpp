// Nature functions
// Copyright (C) 2014 David Stone
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

#include "nature.hpp"

#include "stat_names.hpp"

namespace technicalmachine {

auto make_nature(StatNames const boost, StatNames const drop) -> Nature {
	switch (boost) {
	case StatNames::ATK:
		switch (drop) {
			case StatNames::ATK:
				return Nature::Bashful;
			case StatNames::DEF:
				return Nature::Lonely;
			case StatNames::SPA:
				return Nature::Adamant;
			case StatNames::SPD:
				return Nature::Naughty;
			default:	// case StatNames::SPE:
				return Nature::Brave;
		}
	case StatNames::DEF:
		switch (drop) {
			case StatNames::ATK:
				return Nature::Bold;
			case StatNames::DEF:
				return Nature::Docile;
			case StatNames::SPA:
				return Nature::Impish;
			case StatNames::SPD:
				return Nature::Lax;
			default:	// case StatNames::SPE:
				return Nature::Relaxed;
		}
	case StatNames::SPA:
		switch (drop) {
			case StatNames::ATK:
				return Nature::Modest;
			case StatNames::DEF:
				return Nature::Mild;
			case StatNames::SPA:
				return Nature::Hardy;
			case StatNames::SPD:
				return Nature::Rash;
			default:	// case StatNames::SPE:
				return Nature::Quiet;
		}
	case StatNames::SPD:
		switch (drop) {
			case StatNames::ATK:
				return Nature::Calm;
			case StatNames::DEF:
				return Nature::Gentle;
			case StatNames::SPA:
				return Nature::Careful;
			case StatNames::SPD:
				return Nature::Quirky;
			default:	// case StatNames::SPE:
				return Nature::Sassy;
		}
	default:	// case StatNames::SPE:
		switch (drop) {
			case StatNames::ATK:
				return Nature::Timid;
			case StatNames::DEF:
				return Nature::Hasty;
			case StatNames::SPA:
				return Nature::Jolly;
			case StatNames::SPD:
				return Nature::Naive;
			default:	// case StatNames::SPE:
				return Nature::Serious;
		}
	}
}

auto is_set(Nature const nature) -> bool {
	return nature != Nature::END;
}

template<>
auto boosts_stat<StatNames::ATK>(Nature const nature) -> bool {
	switch (nature) {
		case Nature::Adamant:
		case Nature::Brave:
		case Nature::Lonely:
		case Nature::Naughty:
			return true;
		default:
			return false;
	}
}
template<>
auto boosts_stat<StatNames::SPA>(Nature const nature) -> bool {
	switch (nature) {
		case Nature::Mild:
		case Nature::Modest:
		case Nature::Quiet:
		case Nature::Rash:
			return true;
		default:
			return false;
	}
}

template<>
auto boosts_stat<StatNames::DEF>(Nature const nature) -> bool {
	switch (nature) {
		case Nature::Bold:
		case Nature::Impish:
		case Nature::Lax:
		case Nature::Relaxed:
			return true;
		default:
			return false;
	}
}
template<>
auto boosts_stat<StatNames::SPD>(Nature const nature) -> bool {
	switch (nature) {
		case Nature::Calm:
		case Nature::Careful:
		case Nature::Gentle:
		case Nature::Sassy:
			return true;
		default:
			return false;
	}
}
auto boosts_defending_stat(Nature const nature) -> bool {
	return boosts_stat<StatNames::DEF>(nature) or boosts_stat<StatNames::SPD>(nature);
}

template<>
auto boosts_stat<StatNames::SPE>(Nature const nature) -> bool {
	switch (nature) {
		case Nature::Hasty:
		case Nature::Jolly:
		case Nature::Naive:
		case Nature::Timid:
			return true;
		default:
			return false;
	}
}

template<>
auto lowers_stat<StatNames::ATK>(Nature const nature) -> bool {
	switch (nature) {
		case Nature::Bold:
		case Nature::Calm:
		case Nature::Modest:
		case Nature::Timid:
			return true;
		default:
			return false;
	}
}
template<>
auto lowers_stat<StatNames::SPA>(Nature const nature) -> bool {
	switch (nature) {
		case Nature::Adamant:
		case Nature::Careful:
		case Nature::Impish:
		case Nature::Jolly:
			return true;
		default:
			return false;
	}
}
auto lowers_attacking_stat(Nature const nature) -> bool {
	return lowers_stat<StatNames::ATK>(nature) or lowers_stat<StatNames::SPA>(nature);
}

template<>
auto lowers_stat<StatNames::DEF>(Nature const nature) -> bool {
	switch (nature) {
		case Nature::Gentle:
		case Nature::Hasty:
		case Nature::Lonely:
		case Nature::Mild:
			return true;
		default:
			return false;
	}
}
template<>
auto lowers_stat<StatNames::SPD>(Nature const nature) -> bool {
	switch (nature) {
		case Nature::Lax:
		case Nature::Naive:
		case Nature::Naughty:
		case Nature::Rash:
			return true;
		default:
			return false;
	}
}
auto lowers_defending_stat(Nature const nature) -> bool {
	return lowers_stat<StatNames::DEF>(nature) or lowers_stat<StatNames::SPD>(nature);
}

template<>
auto lowers_stat<StatNames::SPE>(Nature const nature) -> bool {
	switch (nature) {
		case Nature::Brave:
		case Nature::Quiet:
		case Nature::Relaxed:
		case Nature::Sassy:
			return true;
		default:
			return false;
	}
}

}	// namespace technicalmachine
