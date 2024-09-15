// Copyright David Stone 2024.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.action_prediction.predict_max_damage_selection;

import tm.action_prediction.predicted;

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

export template<Generation generation>
constexpr auto predict_max_damage_selection(Team<generation> const & user, Team<generation> const & other, Environment const environment) -> AllPredicted {
	auto const legal_selections = get_legal_selections(user, other, environment);

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

	return AllPredicted({Predicted(best->selection, 1.0)});
}

} // namespace technicalmachine