// Class to help get the next most likely Pokemon
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

#include "estimate.hpp"

#include "multiplier.hpp"

#include "../team.hpp"

#include <algorithm>
#include <cstddef>
#include <random>
#include <stdexcept>

namespace technicalmachine {
namespace {
class InvalidRandomSpecies : public std::logic_error {
public:
	InvalidRandomSpecies():
		std::logic_error("Random species selection is invalid.")
		{
	}
};
}	// unnamed namespace

Estimate::Estimate(Overall const & overall, Lead const & lead, unsigned const total) {
	for (auto const n : bounded_integer::range(bounded_integer::make_bounded<number_of_species>())) {
		estimate[n] = lead[n] * overall[n] / total;
	}
}

void Estimate::update(Multiplier const & multiplier, Team const & team) {
	for (auto const & pokemon : team.all_pokemon()) {
		update(multiplier, pokemon);
	}
}

void Estimate::update(Multiplier const & multiplier, Species const seen) {
	for (auto const predicted : bounded_integer::range(bounded_integer::make_bounded<number_of_species>())) {
		estimate[predicted] *= multiplier(seen, static_cast<Species>(predicted));
	}
}

Species Estimate::most_likely() const {
	auto const it = std::max_element(std::begin(estimate), std::end(estimate));
	// Estimate is indexed by Species ID. The Species ID is therefore the
	// distance between the found element and the beginning.
	return static_cast<Species>(it - std::begin(estimate));
}

Species Estimate::random(std::mt19937 & random_engine) const {
	auto const total = std::accumulate(std::begin(estimate), std::end(estimate), 0.0F);
	std::uniform_real_distribution<float> distribution(0.0F, total);
	auto usage_threshold = distribution(random_engine);

	for (Container::const_iterator it = std::begin(estimate); it != std::end(estimate); ++it) {
		if (usage_threshold >= *it)
			usage_threshold -= *it;
		else
			return static_cast<Species>(it - std::begin(estimate));
	}
	throw InvalidRandomSpecies();
}

}	// namespace technicalmachine
