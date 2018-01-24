// Keep track of changes in HP to make sure TM has the same view as the server
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

#include <map>
#include <bounded/integer.hpp>
#include "../damage.hpp"
#include "../pokemon/species_forward.hpp"
#include "../stat/hp.hpp"

namespace technicalmachine {
struct Team;

using VisibleFoeHP = bounded::integer<48, 100>;

struct UpdatedHP {
	using VisibleHP = std::common_type<
		bounded::checked_integer<0, HP::max_value>,
		VisibleFoeHP
	>::type;
	explicit UpdatedHP(Team const & team);
	void reset_between_turns();
	void add(bool is_me, Species species, VisibleHP max_precision);
	void update(bool is_me, Species species, VisibleHP value);
	void direct_damage(bool is_me, Species species, damage_type damage);
	void faint(bool is_me, Species species);
	VisibleHP get(bool is_me, Species species) const;
	damage_type damage(bool is_me, Species species) const;
	bool is_fainted(bool is_me, Species species) const;
private:
	typedef std::pair<bool, Species> key_type;
	struct mapped_type {
		mapped_type(VisibleHP set_new_hp);
		VisibleHP new_hp() const;
		damage_type damage() const;
		bool is_fainted() const;
		void change_hp(VisibleHP set_new_hp);
		void direct_damage(damage_type set_damage);
		void faint();
		void reset();
	private:
		damage_type m_damage;
		VisibleHP m_new_hp;
		bool m_fainted;
	};
	using container_type = std::map<key_type, mapped_type>;
	container_type container;
};

}	// namespace technicalmachine
