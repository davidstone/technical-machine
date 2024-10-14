// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.move.other_action;

import tm.move.category;
import tm.move.future_selection;
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
	constexpr OtherAction(FutureSelection const selection):
		m_action(selection)
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
						switch (move_type(generation, move.name, tv::none)) {
							case Type::Normal:
							case Type::Fighting:
								return true;
							default:
								return false;
						}
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

	constexpr auto future_selection_is_damaging() const {
		return tv::visit(m_action, tv::overload(
			[](FutureSelection const selection) { return selection.is_damaging; },
			[](auto) { return false; }
		));
	}

private:
	tv::variant<KnownMove, FutureSelection, IrrelevantAction> m_action;
};

} // namespace technicalmachine
