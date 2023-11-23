// Copyright David Stone 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.clients.visible_damage_to_actual_damage;

import tm.clients.move_result;

import tm.move.actual_damage;

import tm.pokemon.substitute;

import tm.stat.current_hp;
import tm.stat.hp;
import tm.stat.to_real_hp;

import tm.visible_hp;

import bounded;
import std_module;
import tv;

namespace technicalmachine {
using namespace bounded::literal;

constexpr auto no_damage_function = [](NoDamage) -> ActualDamage {
	return ActualDamage::Known(0_bi);
};

struct MoveHitSubstitute {
	explicit constexpr MoveHitSubstitute(Substitute const other_substitute):
		m_other_substitute(other_substitute)
	{
	}
	constexpr auto operator()(SubstituteDamaged) const -> ActualDamage {
		if (!m_other_substitute) {
			throw std::runtime_error("Tried to damage a Substitute when the target does not have a Substitute");
		}
		return ActualDamage::Capped(bounded::increase_min<0>(m_other_substitute.hp() - 1_bi));
	}
	constexpr auto operator()(SubstituteBroke) const -> ActualDamage {
		return ActualDamage::Known(m_other_substitute.hp());
	}

private:
	Substitute m_other_substitute;
};

export constexpr auto visible_damage_to_actual_damage(
	Damage const damage,
	bool const damaged_has_exact_hp,
	HP const old_hp,
	Substitute const other_substitute
) -> ActualDamage {
	return tv::visit(damage, tv::overload(
		no_damage_function,
		[&](VisibleHP const hp) -> ActualDamage {
			auto const new_hp = damaged_has_exact_hp ?
				hp.current.value() :
				to_real_hp(old_hp.max(), hp).value;
			auto const value = bounded::check_in_range<CurrentHP>(old_hp.current() - new_hp);
			return ActualDamage::Known(value);
		},
		MoveHitSubstitute(other_substitute)
	));
}

} // namespace technicalmachine
