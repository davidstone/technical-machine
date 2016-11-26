// Optimize defensive EVs and nature to remove waste
// Copyright (C) 2016 David Stone
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

#include "../../stat/ev.hpp"

#include <containers/vector/vector.hpp>

#include <string>

namespace technicalmachine {
enum class Nature : uint8_t;
struct Pokemon;

struct SingleClassificationEVs {
	SingleClassificationEVs(EV hp, EV defensive, Nature nature);
	
	auto hp() const {
		return m_hp;
	}
	auto defensive() const {
		return m_defensive;
	}
	auto nature() const {
		return m_nature;
	}
	
private:
	EV m_hp;
	EV m_defensive;
	Nature m_nature;
};

auto are_compatible(SingleClassificationEVs const & physical, SingleClassificationEVs const & special) -> bool;

containers::vector<SingleClassificationEVs> equal_defensiveness(Pokemon const & pokemon, bool physical);

}	// namespace technicalmachine
