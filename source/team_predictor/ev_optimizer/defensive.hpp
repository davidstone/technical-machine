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

#pragma once

#include "defensive_data_point.hpp"

#include "../../stat/nature.hpp"

#include <containers/algorithms/find.hpp>
#include <containers/static_vector/static_vector.hpp>

namespace technicalmachine {
struct Pokemon;

struct DefensiveEVs {
	explicit DefensiveEVs(Pokemon const & pokemon);
	friend auto begin(DefensiveEVs const & defensive) {
		return begin(defensive.container);
	}
	friend auto end(DefensiveEVs const & defensive) {
		return end(defensive.container);
	}
	auto find(Nature const nature) const {
		return containers::find_if(container, [=](auto const value) { return value.nature == nature; });
	}
	static constexpr auto number_of_natures = size(containers::enum_range<Nature>());
	using BestPerNature = containers::static_vector<DataPoint, number_of_natures.value()>;
	using Natures = containers::static_vector<Nature, number_of_natures.value()>;
private:
	void remove_inefficient_natures(Natures const & divided_natures);
	void add_other_potential_natures();
	BestPerNature container;
};

}	// namespace technicalmachine
