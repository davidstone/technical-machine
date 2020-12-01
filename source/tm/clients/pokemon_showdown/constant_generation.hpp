// Handles challenges / current battles
// Copyright (C) 2019 David Stone
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

#include <tm/generation.hpp>

#include <type_traits>

namespace technicalmachine {
namespace ps {

template<Generation generation>
inline constexpr auto constant_gen = std::integral_constant<Generation, generation>();

constexpr auto constant_generation(Generation const generation, auto && make) {
	switch (generation) {
		case Generation::one: return make(constant_gen<Generation::one>);
		case Generation::two: return make(constant_gen<Generation::two>);
		case Generation::three: return make(constant_gen<Generation::three>);
		case Generation::four: return make(constant_gen<Generation::four>);
		case Generation::five: return make(constant_gen<Generation::five>);
		case Generation::six: return make(constant_gen<Generation::six>);
		case Generation::seven: return make(constant_gen<Generation::seven>);
		case Generation::eight: return make(constant_gen<Generation::eight>);
	}
}

} // namespace ps
} // namespace technicalmachine
