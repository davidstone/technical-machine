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

#include <tm/generation.hpp>
#include <tm/move/moves.hpp>
#include <tm/pokemon/level.hpp>
#include <tm/pokemon/max_pokemon_per_team.hpp>
#include <tm/pokemon/species_forward.hpp>
#include <tm/stat/stat_names.hpp>
#include <tm/status.hpp>

#include <bounded/integer.hpp>
#include <bounded/unreachable.hpp>

#include <containers/static_vector/static_vector.hpp>

namespace technicalmachine {
struct Team;

struct Variable {
	using value_type = bounded::integer<0, 150>;

	constexpr auto effect_activates() const {
		return m_value != 0_bi;
	}
	
	constexpr auto acupressure_stat_boost() const {
		return BoostableStat(m_value);
	}
	
	constexpr auto confuse() {
		m_value = 1_bi;
	}

	auto fang_side_effects() const {
		struct Result {
			bool status;
			bool flinch;
		};
		switch (m_value.value()) {
			case 0: return Result{false, false};
			case 1: return Result{true, false};
			case 2: return Result{false, true};
			case 3: return Result{true, true};
			default: bounded::assert_or_assume_unreachable();
		}
	}

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

	constexpr auto psywave_damage(Generation const generation, Level const level) const {
		switch (generation) {
			case Generation::one:
			case Generation::two:
				// Close enough
				return bounded::max(1_bi, level() * m_value / 100_bi);
			case Generation::three:
			case Generation::four:
				return bounded::max(1_bi, level() * (bounded::integer<0, 10>(m_value) * 10_bi + 50_bi) / 100_bi);
			case Generation::five:
			case Generation::six:
			case Generation::seven:
			case Generation::eight:
				return bounded::max(1_bi, level() * (bounded::integer<0, 100>(m_value) + 50_bi) / 100_bi);
		}
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

using Probabilities = containers::static_vector<VariableProbability, 151>;
auto all_probabilities(Generation, Moves, TeamSize foe_size) -> Probabilities;

}	// namespace technicalmachine
