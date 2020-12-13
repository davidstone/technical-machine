// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <tm/type/pokemon_types.hpp>

#include <tm/move/known_move.hpp>

#include <tm/generation.hpp>
#include <tm/operators.hpp>
#include <tm/rational.hpp>
#include <tm/weather.hpp>

#include <bounded/integer.hpp>

namespace technicalmachine {

template<Generation>
struct ActivePokemon;
template<Generation>
struct MutableActivePokemon;

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

template<Generation generation>
auto affects_target(KnownMove const move, ActivePokemon<generation> const target, Weather const weather) -> bool {
	auto const effectiveness = Effectiveness(generation, move.type, target.types());
	auto generation_1_fixed_damage_move = [=] {
		switch (move.name) {
			case Moves::Night_Shade:
			case Moves::Seismic_Toss:
			case Moves::Sonic_Boom:
			case Moves::Super_Fang:
				return true;
			default:
				return false;
		}
	};
	if (generation == Generation::one and generation_1_fixed_damage_move()) {
		return true;
	}
	return !effectiveness.has_no_effect() and (move.type != Type::Ground or grounded(target, weather));
}

}	// namespace technicalmachine
