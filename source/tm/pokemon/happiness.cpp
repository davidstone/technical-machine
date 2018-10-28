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

#include <tm/pokemon/happiness.hpp>

namespace technicalmachine {

Happiness::Happiness(value_type const value):
	m_value(value) {
}

auto Happiness::operator()() const -> value_type {
	return m_value;
}

bounded::integer<0, 102> return_power(Happiness const happiness) {
	return happiness.m_value * 2_bi / 5_bi;
}
bounded::integer<0, 102> frustration_power(Happiness const happiness) {
	return 102_bi - return_power(happiness);
}


}	// namespace technicalmachine
