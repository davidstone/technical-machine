// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <tm/type/pokemon_types.hpp>

#include <tm/move/known_move.hpp>
#include <tm/move/moves.hpp>

#include <tm/pokemon/any_pokemon.hpp>

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

constexpr auto always_affects_target(Generation const generation, Moves const move) {
	switch (generation) {
		case Generation::one:
			switch (move) {
				case Moves::Night_Shade:
				case Moves::Seismic_Toss:
				case Moves::Sonic_Boom:
				case Moves::Super_Fang:
					return true;
				default:
					return false;
			}
		case Generation::two:
		case Generation::three:
		case Generation::four:
		case Generation::five:
			switch (move) {
				case Moves::Block:
				case Moves::Mean_Look:
				case Moves::Spider_Web:
					return true;
				default:
					return false;
			}
		case Generation::six:
		case Generation::seven:
		case Generation::eight:
			return false;
	}
}

template<any_active_pokemon TargetPokemon>
auto affects_target(KnownMove const move, TargetPokemon const target, Weather const weather) -> bool {
	constexpr auto generation = generation_from<TargetPokemon>;
	auto const effectiveness = Effectiveness(generation, move.type, target.types());
	if (always_affects_target(generation, move.name)) {
		return true;
	}
	return !effectiveness.has_no_effect() and (move.type != Type::Ground or grounded(target, weather));
}

}	// namespace technicalmachine
