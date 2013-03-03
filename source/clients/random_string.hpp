// Generate a random string of arbitrary length
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

#ifndef CLIENT__RANDOM_STRING_HPP_
#define CLIENT__RANDOM_STRING_HPP_

#include <algorithm>
#include <random>
#include <string>

namespace technicalmachine {

template<typename RandomEngine>
char random_character(RandomEngine & random_engine) {
	static constexpr char legal_characters[] = "abcdefghijklmnopqrstuvwxyz0123456789";
	static constexpr auto number_of_characters = sizeof(legal_characters) - 1;
	std::uniform_int_distribution<size_t> distribution(0, number_of_characters - 1);
	return legal_characters[distribution(random_engine)];
}

template<typename RandomEngine>
std::string random_string(RandomEngine & random_engine, size_t const size) {
	std::string str;
	str.resize(size);
	std::generate(std::begin(str), std::end(str), [&]() { return random_character(random_engine); });
	return str;
}

}	// namespace technicalmachine
#endif	// CLIENT__RANDOM_STRING_HPP_
