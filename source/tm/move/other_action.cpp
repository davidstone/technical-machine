// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.move.other_action;

import tm.move.category;
import tm.move.future_action;
import tm.move.irrelevant_action;
import tm.move.known_move;
import tm.move.move_name;

import tm.type.move_type;
import tm.type.type;

import tm.generation;

import tv;

namespace technicalmachine {

export struct OtherAction {
	constexpr OtherAction(KnownMove const move):
		m_action(move)
	{
	}
	constexpr OtherAction(FutureAction const action):
		m_action(action)
	{
	}
	constexpr OtherAction(IrrelevantAction const action):
		m_action(action)
	{
	}

	constexpr auto is_counterable(Generation const generation) const {
		return tv::visit(m_action, tv::overload(
			[=](KnownMove const move) {
				switch (generation) {
					case Generation::one:
						// TODO: This isn't quite right
						return move_type(generation, move.name, tv::none) == Type::Normal;
					case Generation::two:
					case Generation::three:
						return
							move.name == MoveName::Hidden_Power or
							is_physical(generation, move);
					case Generation::four:
					case Generation::five:
					case Generation::six:
					case Generation::seven:
					case Generation::eight:
						return is_physical(generation, move);
				}
				return generation <= Generation::three and move.name == MoveName::Hidden_Power ? true : is_physical(generation, move);
			},
			[](auto) { return false; }
		));
	}
	constexpr auto is_mirror_coatable(Generation const generation) const {
		return tv::visit(m_action, tv::overload(
			[=](KnownMove const move) {
				return generation <= Generation::three and move.name == MoveName::Hidden_Power ? false : is_special(generation, move);
			},
			[](auto) { return false; }
		));
	}

	constexpr auto future_action_is_damaging() const {
		return tv::visit(m_action, tv::overload(
			[](FutureAction const action) { return action.is_damaging; },
			[](auto) { return false; }
		));
	}

private:
	tv::variant<KnownMove, FutureAction, IrrelevantAction> m_action;
};

} // namespace technicalmachine
