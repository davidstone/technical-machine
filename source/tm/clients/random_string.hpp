// Generate a random string of arbitrary length
// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <algorithm>
#include <random>
#include <string>

namespace technicalmachine {

char random_character(auto & random_engine) {
	static constexpr char legal_characters[] = "abcdefghijklmnopqrstuvwxyz0123456789";
	static constexpr auto number_of_characters = sizeof(legal_characters) - 1;
	std::uniform_int_distribution<size_t> distribution(0, number_of_characters - 1);
	return legal_characters[distribution(random_engine)];
}

std::string random_string(auto & random_engine, size_t const size) {
	std::string str;
	str.resize(size);
	std::generate(std::begin(str), std::end(str), [&]() { return random_character(random_engine); });
	return str;
}

}	// namespace technicalmachine
