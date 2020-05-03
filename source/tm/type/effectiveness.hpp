// Copyright (C) 2015 David Stone
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

#include <tm/type/pokemon_types.hpp>

#include <tm/move/known_move.hpp>
#include <tm/pokemon/active_pokemon.hpp>

#include <tm/generation.hpp>
#include <tm/operators.hpp>
#include <tm/rational.hpp>
#include <tm/weather.hpp>

#include <bounded/integer.hpp>

namespace technicalmachine {

struct Effectiveness {
	Effectiveness(Generation generation, Type attacking, PokemonTypes defending);
	auto is_super_effective() const -> bool;
	auto is_not_very_effective() const -> bool;
	auto has_no_effect() const -> bool;

private:
	auto product() const {
		return m_first * m_second;
	}

	friend auto operator*(Effectiveness const lhs, auto const rhs) {
		return rhs * lhs.product();
	}

	using SingleType = rational<bounded::integer<0, 2>, bounded::integer<1, 2>>;
	SingleType m_first;
	SingleType m_second;
};

auto operator*(auto const lhs, Effectiveness const rhs) {
	return rhs * lhs;
}

inline auto affects_target(Generation const generation, KnownMove const move, ActivePokemon const target, Weather const weather) -> bool {
	auto const effectiveness = Effectiveness(generation, move.type, target.types());
	auto generation_1_fixed_damage_move = [=] {
		switch (move.name) {
			case Moves::Night_Shade:
			case Moves::Seismic_Toss:
				return true;
			default:
				return false;
		}
	};
	if (generation == Generation::one and generation_1_fixed_damage_move()) {
		return true;
	}
	return !effectiveness.has_no_effect() and (move.type != Type::Ground or grounded(generation, target, weather));
}

}	// namespace technicalmachine
