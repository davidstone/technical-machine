// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <operators/forward.hpp>

#include <containers/algorithms/concatenate.hpp>
#include <containers/single_element_range.hpp>

#include <stdexcept>
#include <string>
#include <string_view>

namespace technicalmachine {

using namespace std::string_view_literals;

struct IncorrectCalculation : std::logic_error {
	IncorrectCalculation(auto const result, auto const expected):
		std::logic_error(containers::concatenate<std::string>("Result of "sv, to_string(result), " instead of the expected "sv, to_string(expected), containers::single_element_range('\n')))
	{
	}
private:
	static auto to_string_impl(auto && value) {
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

