// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <operators/forward.hpp>

#include <stdexcept>
#include <string>

namespace technicalmachine {

struct IncorrectCalculation : std::logic_error {
	IncorrectCalculation(auto const result, auto const expected):
		std::logic_error("Result of " + to_string(result) + " instead of the expected " + to_string(expected) + '\n')
		{
	}
private:
	static std::string to_string(auto && value) {
		using std::to_string;
		return to_string(OPERATORS_FORWARD(value));
	}

};

void check_equal(auto const lhs, auto const rhs) {
	if (lhs != rhs) {
		throw IncorrectCalculation(lhs, rhs);
	}
}

}	// namespace technicalmachine

