// Pokemon Showdown incoming messages
// Copyright (C) 2020 David Stone
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

#include <tm/buffer_view.hpp>
#include <string_view>

namespace technicalmachine {
namespace {

constexpr auto pop_zero_from_empty() {
	auto view = BufferView(std::string_view(""));

	BOUNDED_ASSERT(view.remainder() == "");

	auto const result = view.pop(0);
	BOUNDED_ASSERT(result == "");
	BOUNDED_ASSERT(view.remainder() == "");
	return true;
}

static_assert(pop_zero_from_empty());


constexpr auto pop_zero_from_non_empty() {
	auto view = BufferView(std::string_view("technical"));

	BOUNDED_ASSERT(view.remainder() == "technical");

	auto const result = view.pop(0);
	BOUNDED_ASSERT(result == "");
	BOUNDED_ASSERT(view.remainder() == "technical");
	return true;
}

static_assert(pop_zero_from_non_empty());

constexpr auto pop_some() {
	auto view = BufferView(std::string_view("machine"));

	BOUNDED_ASSERT(view.remainder() == "machine");

	auto const result = view.pop(3);
	BOUNDED_ASSERT(result == "mac");
	BOUNDED_ASSERT(view.remainder() == "hine");
	return true;
}

static_assert(pop_some());

constexpr auto pop_char_delimiter_from_empty() {
	auto view = DelimitedBufferView(std::string_view(""), 'c');

	BOUNDED_ASSERT(view.remainder() == "");

	auto const result = view.pop();
	BOUNDED_ASSERT(result == "");
	BOUNDED_ASSERT(view.remainder() == "");
	return true;
}

static_assert(pop_char_delimiter_from_empty());

constexpr auto pop_to_found_char_delimiter() {
	auto view = BufferView(std::string_view("pokemon"));

	BOUNDED_ASSERT(view.remainder() == "pokemon");

	auto const result = pop_to_delimiter(view, 'e');
	BOUNDED_ASSERT(result == "pok");
	BOUNDED_ASSERT(view.remainder() == "mon");

	return true;
}

static_assert(pop_to_found_char_delimiter());

constexpr auto pop_to_not_found_char_delimiter() {
	auto view = BufferView(std::string_view("pokemon"));

	BOUNDED_ASSERT(view.remainder() == "pokemon");

	auto const result = pop_to_delimiter(view, 'q');
	BOUNDED_ASSERT(result == "pokemon");
	BOUNDED_ASSERT(view.remainder() == "");

	return true;
}

static_assert(pop_to_not_found_char_delimiter());

constexpr auto pop_to_found_string_delimiter() {
	auto view = BufferView(std::string_view("pokemon"));

	BOUNDED_ASSERT(view.remainder() == "pokemon");

	auto const result = pop_to_delimiter(view, std::string_view("kem"));
	BOUNDED_ASSERT(result == "po");
	BOUNDED_ASSERT(view.remainder() == "on");

	return true;
}

static_assert(pop_to_found_string_delimiter());

constexpr auto pop_to_not_found_string_delimiter() {
	auto view = BufferView(std::string_view("pokemon"));

	BOUNDED_ASSERT(view.remainder() == "pokemon");

	auto const result = pop_to_delimiter(view, std::string_view("pikachu"));
	BOUNDED_ASSERT(result == "pokemon");
	BOUNDED_ASSERT(view.remainder() == "");

	return true;
}

static_assert(pop_to_not_found_string_delimiter());

} // namespace
} // namespace technicalmachine