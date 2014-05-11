// Effectiveness of a type
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

#include "effectiveness.hpp"
#include <algorithm>
#include "collection.hpp"
#include "type.hpp"
#include "../pokemon/pokemon.hpp"
#include "../rational.hpp"

namespace technicalmachine {
namespace {

constexpr auto ne = make_rational(0_bi, 1_bi);
constexpr auto nve = make_rational(1_bi, 2_bi);
constexpr auto reg = make_rational(1_bi, 1_bi);
constexpr auto se = make_rational(2_bi, 1_bi);

auto lookup_effectiveness(Type const attacking, Type const defending) {
	using lookup_type = std::common_type<decltype(ne), decltype(nve), decltype(reg), decltype(se)>::type;
	static constexpr bounded::array<bounded::array<lookup_type, 18>, 18> effectiveness {{
		{ reg, se, reg, reg, nve, nve, nve, nve, se, reg, reg, reg, nve, se, reg, nve, reg, reg },	// Bug
		{ reg, nve, reg, reg, nve, reg, reg, se, reg, reg, reg, reg, reg, se, reg, nve, reg, reg },	// Dark
		{ reg, reg, se, reg, reg, reg, reg, reg, reg, reg, reg, reg, reg, reg, reg, nve, reg, reg },	// Dragon
		{ reg, reg, nve, nve, reg, reg, se, reg, nve, ne, reg, reg, reg, reg, reg, reg, se, reg },	// Electric
		{ nve, se, reg, reg, reg, reg, nve, ne, reg, reg, se, se, nve, nve, se, se, reg, reg },	// Fighting
		{ se, reg, nve, reg, reg, nve, reg, reg, se, reg, se, reg, reg, reg, nve, se, nve, reg },	// Fire
		{ se, reg, reg, nve, se, reg, reg, reg, se, reg, reg, reg, reg, reg, nve, nve, reg, reg },	// Flying
		{ reg, nve, reg, reg, reg, reg, reg, se, reg, reg, reg, ne, reg, se, reg, nve, reg, reg },	// Ghost
		{ nve, reg, nve, reg, reg, nve, nve, reg, nve, se, reg, reg, nve, reg, se, nve, se, reg },	// Grass
		{ nve, reg, reg, se, reg, se, ne, reg, nve, reg, reg, reg, se, reg, se, se, reg, reg },	// Ground
		{ reg, reg, se, reg, reg, nve, se, reg, se, se, nve, reg, reg, reg, reg, nve, nve, reg },	// Ice
		{ reg, reg, reg, reg, reg, reg, reg, ne, reg, reg, reg, reg, reg, reg, nve, nve, reg, reg },	// Normal
		{ reg, reg, reg, reg, reg, reg, reg, nve, se, nve, reg, reg, nve, reg, nve, ne, reg, reg },	// Poison
		{ reg, ne, reg, reg, se, reg, reg, reg, reg, reg, reg, reg, se, nve, reg, nve, reg, reg },	// Psychic
		{ se, reg, reg, reg, nve, se, se, reg, reg, nve, se, reg, reg, reg, reg, nve, reg, reg },	// Rock
		{ reg, reg, reg, nve, reg, nve, reg, reg, reg, reg, se, reg, reg, reg, se, nve, nve, reg },	// Steel
		{ reg, reg, nve, reg, reg, se, reg, reg, nve, se, reg, reg, reg, reg, se, reg, nve, reg },	// Water
		{ reg, reg, reg, reg, reg, reg, reg, reg, reg, reg, reg, reg, reg, reg, reg, reg, reg, reg }	// Typeless
		//	Bug	Drk	Drg	Elc	Ftg	Fir	Fly	Gho	Grs	Grd	Ice	Nrm	Psn	Psy	Rck	Stl	Wtr	Typ		
	}};
	return effectiveness.at(attacking).at(defending);
}

template<typename Container, typename Product>
auto check_effectiveness(Container const & effectiveness, std::initializer_list<Product> const & results) {
	auto const value = effectiveness[0_bi] * effectiveness[1_bi];
	return std::find(std::begin(results), std::end(results), value) != std::end(results);
}



}	// namespace

Effectiveness::Effectiveness(Type const attacking, Type const defending1, Type const defending2):
	m_effectiveness({
		lookup_effectiveness(attacking, defending1),
		lookup_effectiveness(attacking, defending2)
	}) {
	static_assert(
		std::is_same<decltype(lookup_effectiveness(attacking, defending1)), SingleType>::value,
		"Incorrect effectiveness type."
	);
}

Effectiveness::Effectiveness(Type const type, Pokemon const & defender):
	Effectiveness(type, *get_type(defender).types.begin(), *bounded::next(get_type(defender).types.begin())) {
}

Effectiveness::Effectiveness(Type const attacking, Type const defending):
	Effectiveness(attacking, defending, Type::Typeless) {
}

auto Effectiveness::is_super_effective() const -> bool {
	return check_effectiveness(m_effectiveness, { Product(se), Product(se * se) });
}

auto Effectiveness::is_not_very_effective() const -> bool {
	return check_effectiveness(m_effectiveness, { Product(nve), Product(nve * nve) });
}

auto Effectiveness::has_no_effect() const -> bool {
	return check_effectiveness(m_effectiveness, { Product(ne) });
}

}	// namespace technicalmachine
