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

#pragma once

#include <tm/stat/stage.hpp>
#include <tm/stat/stat_names.hpp>

#include <tm/type/effectiveness.hpp>

#include <tm/exists_if.hpp>
#include <tm/team.hpp>
#include <tm/weather.hpp>

#include <bounded/integer.hpp>
#include <bounded/optional.hpp>
#include <bounded/detail/tuple.hpp>

#include <containers/algorithms/accumulate.hpp>
#include <containers/algorithms/all_any_none.hpp>
#include <containers/algorithms/filter_iterator.hpp>
#include <containers/algorithms/transform.hpp>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

namespace technicalmachine {
using namespace bounded::literal;

template<Generation generation>
struct Evaluate {
	Evaluate() {
		auto file = boost::property_tree::ptree();
		read_xml("settings/evaluate.xml", file);
		auto const pt = file.get_child("score");

		m_hp = pt.get<value_type>("hp", 0_bi);
		if constexpr (exists<decltype(m_hidden)>) {
			m_hidden = pt.get<value_type>("hidden", 0_bi);
		}
		if constexpr (exists<decltype(m_spikes)>) {
			m_spikes = pt.get<value_type>("spikes", 0_bi);
		}
		if constexpr (exists<decltype(m_stealth_rock)>) {
			m_stealth_rock = pt.get<value_type>("stealth_rock", 0_bi);
		}
		if constexpr (exists<decltype(m_toxic_spikes)>) {
			m_toxic_spikes = pt.get<value_type>("toxic_spikes", 0_bi);
		}
	}

	auto operator()(Team<generation> const & ai, Team<generation> const & foe) const {
		return score_team(ai) - score_team(foe);
	}

private:
	// Arbitrary values
	using value_type = bounded::integer<-4096, 4096>;

	auto hp(Pokemon<generation> const & pokemon) const {
		return value_type(hp_ratio(pokemon) * m_hp);
	}

	auto hidden(Pokemon<generation> const & pokemon) const {
		if constexpr (exists<decltype(m_hidden)>) {
			return !pokemon.has_been_seen() ? m_hidden : 0_bi;
		} else {
			return 0_bi;
		}
	}

	auto spikes(EntryHazards<generation> const entry_hazards, bool const grounded) const {
		if constexpr (exists<decltype(m_spikes)>) {
			return grounded ? entry_hazards.spikes() * m_spikes : 0_bi;
		} else {
			return 0_bi;
		}
	}

	auto stealth_rock(EntryHazards<generation> const entry_hazards, PokemonTypes const types) const {
		if constexpr (exists<decltype(m_stealth_rock)>) {
			return entry_hazards.stealth_rock() ? Effectiveness(generation, Type::Rock, types) * m_stealth_rock : 0_bi;
		} else {
			return 0_bi;
		}
	}

	auto toxic_spikes(EntryHazards<generation> const entry_hazards, bool const grounded) const {
		if constexpr (exists<decltype(m_toxic_spikes)>) {
			return grounded ? entry_hazards.toxic_spikes() * m_toxic_spikes : 0_bi;
		} else {
			return 0_bi;
		}
	}

	auto score_pokemon(Pokemon<generation> const & pokemon, EntryHazards<generation> const entry_hazards) const {
		auto const types = PokemonTypes(generation, pokemon.species());
		auto const grounded =
			containers::any_equal(types, Type::Flying) or
			is_immune_to_ground(pokemon.initial_ability());
		return
			hp(pokemon) +
			hidden(pokemon) +
			spikes(entry_hazards, grounded) +
			stealth_rock(entry_hazards, types) +
			toxic_spikes(entry_hazards, grounded);
	}

	auto score_team(Team<generation> const & team) const {
		auto has_hp = [](Pokemon<generation> const & pokemon) { return pokemon.hp() != 0_bi; };
		auto get_score = [&](Pokemon<generation> const & pokemon) {
			return score_pokemon(pokemon, team.entry_hazards());
		};
		return containers::sum(containers::transform(containers::filter(team.all_pokemon(), has_hp), get_score));
	}

	value_type m_hp;
	[[no_unique_address]] ExistsIf<value_type, generation <= Generation::four, __LINE__> m_hidden;
	[[no_unique_address]] ExistsIf<value_type, generation >= Generation::two, __LINE__> m_spikes;
	[[no_unique_address]] ExistsIf<value_type, generation >= Generation::four, __LINE__> m_stealth_rock;
	[[no_unique_address]] ExistsIf<value_type, generation >= Generation::four, __LINE__> m_toxic_spikes;
};

struct AllEvaluate {
	template<Generation generation>
	auto get() const {
		return m_data[bounded::detail::types<Evaluate<generation>>()];
	}
private:
	bounded::tuple<
		Evaluate<Generation::one>,
		Evaluate<Generation::two>,
		Evaluate<Generation::three>,
		Evaluate<Generation::four>,
		Evaluate<Generation::five>,
		Evaluate<Generation::six>,
		Evaluate<Generation::seven>,
		Evaluate<Generation::eight>
	> m_data;
};

// 100% chance to win
template<Generation generation>
inline constexpr auto victory = bounded::max_value<decltype(std::declval<Evaluate<generation>>()(std::declval<Team<generation>>(), std::declval<Team<generation>>()))> + 1_bi;

// Returns victory if the battle is won. Returns -victory if the battle is
// lost. Returns 0 otherwise.
template<Generation generation>
constexpr auto win(Team<generation> const & team1, Team<generation> const & team2) -> bounded::optional<double> {
	auto single_team_win = [](Team<generation> const & team) {
		BOUNDED_ASSERT(team.size() != 0_bi);
		return BOUNDED_CONDITIONAL(
			team.size() == 1_bi and team.pokemon().hp() == 0_bi,
			BOUNDED_CONDITIONAL(team.is_me(), -victory<generation>, victory<generation>),
			0_bi
		);
	};
	auto const win1 = single_team_win(team1);
	auto const win2 = single_team_win(team2);
	if (win1 != 0_bi or win2 != 0_bi) {
		return static_cast<double>(win1 + win2);
	}
	return bounded::none;
}

} // namespace technicalmachine
