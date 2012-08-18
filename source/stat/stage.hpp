// Stat stages
// Copyright (C) 2012 David Stone
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

#ifndef STAT__STAGE_HPP_
#define STAT__STAGE_HPP_

#include <array>
#include <cstddef>
#include <cstdint>
#include <functional>
#include "stat.hpp"

namespace technicalmachine {

class Stage {
	public:
		Stage();
		void reset();
		void boost(Stat::Stats stat, int n);
		unsigned accumulate(std::function<unsigned(int)> const & f) const;
		static int dot_product(Stage const & stage, std::array<int, Stat::END> const & multiplier);
		static int dot_product(std::array<int, Stat::END> const & multiplier, Stage const & stage);
		void boost_regular(int n);
		void boost_physical(int n);
		void boost_special(int n);
		void boost_defensive(int n);
		void boost_offensive(int n);
		void maximize_attack();
		static void swap_defensive(Stage & lhs, Stage & rhs);
		static void swap_offensive(Stage & lhs, Stage & rhs);
		uint64_t hash() const;
		static uint64_t max_hash();
		friend bool operator==(Stage const & lhs, Stage const & rhs);
		friend bool operator!=(Stage const & lhs, Stage const & rhs);
	private:
		friend class Stat;
		friend class ChanceToHit;
		static void boost(int8_t & stage, int n);
		void boost(std::initializer_list<Stat::Stats> const & stats, int n);
		static void swap_specified(Stage & lhs, Stage & rhs, std::initializer_list<Stat::Stats> const & stats);
		int8_t const & operator[](size_t index) const;
		int8_t & operator[](size_t index);
		std::array<int8_t, Stat::END> stages;
};

}	// namespace technicalmachine
#endif	// STAT__STAGE_HPP_
