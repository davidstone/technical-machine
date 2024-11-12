// Copyright David Stone 2024.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.strategy.max_damage;

import tm.move.calculate_damage;
import tm.move.damage_type;
import tm.move.executed_move;
import tm.move.irrelevant_action;
import tm.move.known_move;
import tm.move.legal_selections;
import tm.move.move_name;
import tm.move.no_effect_function;
import tm.move.other_action;
import tm.move.pass;
import tm.move.pp;
import tm.move.selection;
import tm.move.switch_;

import tm.pokemon.get_hidden_power_type;

import tm.strategy.strategy;
import tm.strategy.weighted_selection;

import tm.type.move_type;

import tm.contact_ability_effect;
import tm.environment;
import tm.generation;
import tm.team;

import bounded;
import containers;
import std_module;
import tv;

namespace technicalmachine {

using namespace bounded::literal;

struct SelectionAndDamage {
	Selection selection;
	damage_type damage;
};

template<Generation generation>
constexpr auto max_damage(
	Team<generation> const & user,
	LegalSelections const legal_selections,
	Team<generation> const & other,
	Environment const environment
) -> WeightedSelections {
	auto const scores = containers::make_static_vector(containers::transform(
		legal_selections,
		[&](Selection const selection) -> SelectionAndDamage {
			return tv::visit(selection, tv::overload(
				[&](MoveName const name) -> SelectionAndDamage {
					auto const known_move = KnownMove(
						name,
						move_type(
							generation,
							name,
							get_hidden_power_type(user.pokemon())
						)
					);
					// TODO: annoying that I would have to scan to find this
					auto const pp = PP(generation, name, 0_bi);
					constexpr auto critical_hit = false;
					constexpr auto contact_ability_effect = ContactAbilityEffect::nothing;
					constexpr auto move_weakened_from_item = false;
					// This means it will never pick Sucker Punch and Counter and
					// usually pick Focus Punch.
					constexpr auto other_action = OtherAction(IrrelevantAction());
					return SelectionAndDamage(
						selection,
						calculate_damage(
							user,
							ExecutedMove<Team<generation>>(
								known_move,
								pp,
								no_effect_function,
								critical_hit,
								contact_ability_effect
							),
							move_weakened_from_item,
							other,
							other_action,
							environment
						)
					);
				},
				[](Switch const switch_) -> SelectionAndDamage {
					return SelectionAndDamage(switch_, 0_bi);
				},
				[](Pass) -> SelectionAndDamage {
					return SelectionAndDamage(pass, 0_bi);
				}
			));
		}
	));
	auto const best = containers::max_element(
		scores,
		[&](SelectionAndDamage const lhs, SelectionAndDamage const rhs) {
			return lhs.damage > rhs.damage;
		}
	);
	return WeightedSelections(containers::transform(
		containers::filter(
			scores,
			[&](SelectionAndDamage const element) {
				return element.damage == best->damage;
			}
		),
		[](SelectionAndDamage const element) {
			return WeightedSelection(element.selection, 1.0);
		}
	));
}

export auto make_max_damage() -> Strategy {
	return Strategy([]<Generation generation>(
		Team<generation> const & ai,
		LegalSelections const ai_selections,
		Team<generation> const & foe,
		[[maybe_unused]] LegalSelections const foe_selections,
		Environment const environment
	) -> WeightedSelections {
		return max_damage(ai, ai_selections, foe, environment);
	});
}

} // namespace technicalmachine
