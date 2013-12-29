// Incorrect Calculation class
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

#ifndef TEST__INCORRECT_CALCULATION_HPP_
#define TEST__INCORRECT_CALCULATION_HPP_

#include <stdexcept>
#include <string>

namespace technicalmachine {

class IncorrectCalculation : public std::logic_error {
public:
	template<typename LHS, typename RHS>
	IncorrectCalculation(LHS const result, RHS const expected):
		std::logic_error("Result of " + to_string(result) + " instead of the expected " + to_string(expected) + '\n')
		{
	}
private:
	template<typename T>
	static std::string to_string(T && t) {
		using std::to_string;
		return to_string(std::forward<T>(t));
	}

};

template<typename LHS, typename RHS>
void check_equal(LHS const lhs, RHS const rhs) {
	if (lhs != rhs) {
		throw IncorrectCalculation(lhs, rhs);
	}
}

}	// namespace technicalmachine

#endif	// TEST__INCORRECT_CALCULATION_HPP_
