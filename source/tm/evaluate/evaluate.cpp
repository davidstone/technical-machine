// Copyright David Stone 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.evaluate.evaluate;

import tm.evaluate.evaluate_settings;

import tm.pokemon.hp_ratio;
import tm.pokemon.pokemon;

import tm.type.effectiveness;
import tm.type.pokemon_types;
import tm.type.type;

import tm.entry_hazards;
import tm.exists_if;
import tm.generation;
import tm.team;

import bounded;
import containers;

namespace technicalmachine {
using namespace bounded::literal;

export template<Generation generation>
struct Evaluate {
	constexpr explicit Evaluate(EvaluateSettings const settings):
		m_hp(settings.hp),
		m_hidden(settings.hidden),
		m_spikes(settings.spikes),
		m_stealth_rock(settings.stealth_rock),
		m_toxic_spikes(settings.toxic_spikes)
	{
	}

	constexpr auto operator()(Team<generation> const & ai, Team<generation> const & foe) const {
		return score_team(ai) - score_team(foe);
	}

private:
	using value_type = EvaluateSettings::value_type;

	constexpr auto hp(Pokemon<generation> const & pokemon) const {
		return bounded::assume_in_range<value_type>(hp_ratio(pokemon) * m_hp);
	}

	constexpr auto hidden(Pokemon<generation> const & pokemon) const {
		if constexpr (exists<decltype(m_hidden)>) {
			return !pokemon.has_been_seen() ? m_hidden : 0_bi;
		} else {
			return 0_bi;
		}
	}

	constexpr auto spikes(EntryHazards<generation> const entry_hazards, bool const grounded) const {
		if constexpr (exists<decltype(m_spikes)>) {
			return grounded ? entry_hazards.spikes() * m_spikes : 0_bi;
		} else {
			return 0_bi;
		}
	}

	constexpr auto stealth_rock(EntryHazards<generation> const entry_hazards, PokemonTypes const types) const {
		if constexpr (exists<decltype(m_stealth_rock)>) {
			return entry_hazards.stealth_rock() ? Effectiveness(generation, Type::Rock, types) * m_stealth_rock : 0_bi;
		} else {
			return 0_bi;
		}
	}

	constexpr auto toxic_spikes(EntryHazards<generation> const entry_hazards, bool const grounded) const {
		if constexpr (exists<decltype(m_toxic_spikes)>) {
			return grounded ? entry_hazards.toxic_spikes() * m_toxic_spikes : 0_bi;
		} else {
			return 0_bi;
		}
	}

	constexpr auto score_pokemon(Pokemon<generation> const & pokemon, EntryHazards<generation> const entry_hazards) const {
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

	constexpr auto score_team(Team<generation> const & team) const {
		auto has_hp = [](Pokemon<generation> const & pokemon) { return pokemon.hp().current() != 0_bi; };
		auto get_score = [&](Pokemon<generation> const & pokemon) {
			return score_pokemon(pokemon, team.entry_hazards());
		};
		return containers::sum(containers::transform(containers::filter(team.all_pokemon(), has_hp), get_score));
	}

	value_type m_hp;
	// TODO: https://github.com/llvm/llvm-project/issues/61360
	[[no_unique_address]] ExistsIf<value_type, generation <= Generation::four, struct evaluate_hidden> m_hidden;
	[[no_unique_address]] ExistsIf<value_type, generation >= Generation::two, struct evaluate_spikes> m_spikes;
	[[no_unique_address]] ExistsIf<value_type, generation >= Generation::four, struct evaluate_stealth_rock> m_stealth_rock;
	[[no_unique_address]] ExistsIf<value_type, generation >= Generation::four, struct evaluate_toxic_spikes> m_toxic_spikes;
};

} // namespace technicalmachine
