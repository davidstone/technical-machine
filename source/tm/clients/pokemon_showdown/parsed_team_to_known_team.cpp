// Copyright David Stone 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.clients.ps.parsed_team_to_known_team;

import tm.clients.ps.parsed_team;

import tm.move.move;
import tm.move.move_name;
import tm.move.regular_moves;

import tm.pokemon.hidden_power;
import tm.pokemon.known_pokemon;

import tm.stat.calculate_ivs_and_evs;
import tm.stat.hp;
import tm.stat.initial_stat;
import tm.stat.stat_style;
import tm.stat.stats;

import tm.generation;
import tm.team;

import bounded;
import containers;
import std_module;
import tv;

namespace technicalmachine::ps {
using namespace bounded::literal;

export template<Generation generation>
constexpr auto parsed_team_to_known_team(ParsedSide const & team) -> KnownTeam<generation> {
	return KnownTeam<generation>(containers::transform(
		team.all_pokemon,
		[](auto const & pokemon) {
			auto stats = [&] {
				if constexpr (generation == Generation::one) {
					if (pokemon.stats.spa != pokemon.stats.spd) {
						throw std::runtime_error("Special Attack and Special Defense do not match.");
					}
				}
				auto get = [](auto const stat) {
					using Stat = InitialStat<stat_style_for(generation)>;
					return bounded::check_in_range<Stat>(stat);
				};
				return Stats<stat_style_for(generation)>(
					HP(pokemon.stats.hp.max.value()),
					get(pokemon.stats.atk),
					get(pokemon.stats.def),
					get(pokemon.stats.spa),
					get(pokemon.stats.spd),
					get(pokemon.stats.spe)
				);
			};
			auto hidden_power = [&] -> tv::optional<HiddenPower<generation>> {
				if (!pokemon.hidden_power_type) {
					return tv::none;
				}
				constexpr auto const power = [] {
					if constexpr (generation <= Generation::one) {
						return 0_bi;
					} else if constexpr (generation <= Generation::five) {
						return 70_bi;
					} else {
						return 60_bi;
					}
				}();
				return HiddenPower<generation>(power, *pokemon.hidden_power_type);
			};
			return KnownPokemon<generation>(
				pokemon.species,
				pokemon.nickname,
				pokemon.level,
				pokemon.gender,
				pokemon.item,
				pokemon.ability,
				calculate_ivs_and_evs(
					pokemon.species,
					pokemon.level,
					stats(),
					hidden_power()
				),
				RegularMoves(containers::transform(
					pokemon.moves,
					[](MoveName const name) {
						return Move(generation, name);
					}
				))
			);
		}
	));
}

} // namespace technicalmachine::ps
