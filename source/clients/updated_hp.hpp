// Keep track of changes in HP to make sure TM has the same view as the server
// Copyright (C) 2013 David Stone
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

#ifndef CLIENTS__UPDATED_HP_HPP_
#define CLIENTS__UPDATED_HP_HPP_

#include <map>
#include "../pokemon/species_forward.hpp"

namespace technicalmachine {
class Team;

class UpdatedHP {
public:
	explicit UpdatedHP(Team const & team);
	void reset_between_turns();
	void add(bool is_me, Species species, unsigned max_precision);
	void update(bool is_me, Species species, unsigned value);
	void direct_damage(bool is_me, Species species, unsigned damage);
	void faint(bool is_me, Species species);
	unsigned get(bool is_me, Species species) const;
	unsigned damage(bool is_me, Species species) const;
	bool is_fainted(bool is_me, Species species) const;
private:
	typedef std::pair<bool, Species> key_type;
	class mapped_type {
	public:
		mapped_type(unsigned set_new_hp, unsigned set_damage);
		unsigned new_hp() const;
		unsigned damage() const;
		bool is_fainted() const;
		void change_hp(unsigned set_new_hp);
		void direct_damage(unsigned set_damage);
		void faint();
		void reset();
	private:
		unsigned m_new_hp;
		unsigned m_damage;
		bool m_fainted;
	};
	using container_type = std::map<key_type, mapped_type>;
	container_type container;
};

}	// namespace technicalmachine
#endif	// CLIENTS__UPDATED_HP_HPP_
