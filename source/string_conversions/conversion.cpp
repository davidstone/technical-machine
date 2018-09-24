// Copyright (C) 2018 David Stone
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

#include "conversion.hpp"

namespace technicalmachine {
namespace {

constexpr auto compare = lowercase_alphanumeric{};
constexpr auto a = std::string_view("adaptability");
constexpr auto b = std::string_view("Adaptability");
static_assert(!compare(a, b));
static_assert(!compare(b, a));
constexpr auto c = std::string_view("Aftermath");
static_assert(compare(a, c));
static_assert(compare(b, c));
static_assert(!compare(c, a));
static_assert(!compare(c, b));
constexpr auto d = std::string_view("----aftermath----");
static_assert(compare(a, d));
static_assert(!compare(d, a));
static_assert(!compare(d, c));
static_assert(!compare(c, d));

} // namespace
} // namespace technical machine
