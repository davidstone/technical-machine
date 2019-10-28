// Random effects of moves
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

#include <tm/move/moves.hpp>
#include <tm/pokemon/active_pokemon.hpp>
#include <tm/pokemon/collection.hpp>
#include <tm/pokemon/level.hpp>
#include <tm/pokemon/species_forward.hpp>
#include <tm/status.hpp>
#include <tm/weather.hpp>

#include <bounded/assert.hpp>
#include <bounded/integer.hpp>
#include <bounded/unreachable.hpp>

#include <containers/static_vector/static_vector.hpp>

namespace technicalmachine {
struct ActivePokemon;
struct Team;

struct Variable {
	using value_type = bounded::integer<0, 150>;

	constexpr auto effect_activates() const {
		return m_value != 0_bi;
	}
	
	constexpr auto acupressure_stat_boost() const {
		return static_cast<StatNames>(m_value);
	}
	
	constexpr auto confuse() {
		m_value = 1_bi;
	}

	auto fang_side_effects(Pokemon & user, MutableActivePokemon target, Weather const weather, Statuses const status) const {
		// Uproar is irrelevant here
		constexpr auto uproar = false;
		switch (m_value.value()) {
			case 0:
				break;
			case 1:
				apply(status, user, target, weather, uproar);
				break;
			case 2:
				target.flinch();
				break;
			case 3:	
				apply(status, user, target, weather, uproar);
				target.flinch();
				break;
			default:
				bounded::assert_or_assume_unreachable();
		}
	}

	// TODO: Handle Fangs
	constexpr auto set_flinch(bool const set = true) {
		m_value = BOUNDED_CONDITIONAL(set, 1_bi, 0_bi);
	}

	constexpr auto magnitude_power() const {
		return m_value;
	}
	using Magnitude = bounded::checked_integer<4, 10>;
	auto set_magnitude(Magnitude magnitude) -> void;

	constexpr auto present_heals() const {
		return m_value != 0_bi;
	}
	constexpr auto present_power() const {
		BOUNDED_ASSERT(!present_heals());
		return m_value;
	}

	constexpr auto phaze_index() const {
		return static_cast<TeamIndex>(m_value);
	}
	// Team is the Team that was phazed, not the team that used the phazing move
	auto set_phaze_index(Team const & team, Species species) -> void;

	constexpr auto psywave_damage(Level const level) const {
		return bounded::max(1_bi, level() * static_cast<bounded::integer<50, 150>>(m_value) / 100_bi);
	}
	
	constexpr void apply_status(Moves const move, Statuses const status) {
		m_value = [=]() -> value_type {
			switch (move) {
				case Moves::Tri_Attack:
					switch (status) {
						case Statuses::burn: return 1_bi;
						case Statuses::freeze: return 2_bi;
						case Statuses::paralysis: return 3_bi;
						default: throw std::runtime_error("Invalid Tri Attack status");
					}
				case Moves::Fire_Fang:
				case Moves::Ice_Fang:
				case Moves::Thunder_Fang:
					// TODO: Handle flinch
					return 1_bi;
				default:
					return 1_bi;
			}
		}();
	}

	constexpr auto tri_attack_status() const {
		switch (m_value.value()) {
			case 0: return Statuses::clear;
			case 1: return Statuses::burn;
			case 2: return Statuses::freeze;
			case 3: return Statuses::paralysis;
			default: bounded::assert_or_assume_unreachable();
		}
	}

	// Treat this as private. Must be public to qualify as an aggregate	
	value_type m_value;
};

struct VariableProbability {
	Variable variable;
	double probability;
};

using Probabilities = containers::static_vector<VariableProbability, 101>;
auto all_probabilities(Moves move, TeamSize foe_size) -> Probabilities;

}	// namespace technicalmachine
