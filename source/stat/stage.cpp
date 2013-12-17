// Stat stages
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

#include "stage.hpp"

#include "../rational.hpp"

namespace technicalmachine {
using namespace bounded_integer::literal;
namespace {
constexpr auto max_stage = 6;
}

Stage::Stage() {
	reset();
}

void Stage::reset() {
	std::fill(m_stages.begin(), m_stages.end(), 0_bi);
}

void Stage::boost(StatNames const stat, boost_type const number_of_stages) {
	m_stages[stat] += number_of_stages;
}

void Stage::boost_regular(boost_type const number_of_stages) {
	for (auto const stat : { StatNames::ATK, StatNames::DEF, StatNames::SPA, StatNames::SPD, StatNames::SPE }) {
		boost(stat, number_of_stages);
	}
}

void Stage::boost_physical(boost_type const number_of_stages) {
	for (auto const stat : { StatNames::ATK, StatNames::DEF }) {
		boost(stat, number_of_stages);
	}
}
void Stage::boost_special(boost_type const number_of_stages) {
	for (auto const stat : { StatNames::SPA, StatNames::SPD }) {
		boost(stat, number_of_stages);
	}
}
void Stage::boost_defensive(boost_type const number_of_stages) {
	for (auto const stat : { StatNames::DEF, StatNames::SPD }) {
		boost(stat, number_of_stages);
	}
}
void Stage::boost_offensive(boost_type const number_of_stages) {
	for (auto const stat : { StatNames::ATK, StatNames::SPA }) {
		boost(stat, number_of_stages);
	}
}

void Stage::swap_specified(Stage & lhs, Stage & rhs, std::initializer_list<StatNames> const & stats) {
	using std::swap;
	for (auto const stat : stats) {
		swap(lhs.m_stages[stat], rhs.m_stages[stat]);
	}
}
void Stage::swap_defensive(Stage & lhs, Stage & rhs) {
	swap_specified(lhs, rhs, { StatNames::DEF, StatNames::SPD });
}
void Stage::swap_offensive(Stage & lhs, Stage & rhs) {
	swap_specified(lhs, rhs, { StatNames::ATK, StatNames::SPA });
}

namespace {
// stage is the magnitude of the stat boost.
template<unsigned base>
constexpr Rational positive_stage_boost(unsigned const stage) {
	static_assert(base == 2 or base == 3, "Most stats use a divisor of 2, but accuracy and evasion use 3.");
	return Rational(base + stage, base);
}
template<unsigned base>
constexpr Rational negative_stage_boost(unsigned const stage) {
	static_assert(base == 2 or base == 3, "Most stats use a numerator of 2, but accuracy and evasion use 3.");
	return Rational(base, base + stage);
}

Rational attacking_stage_modifier(Stage::value_type const stage, bool const ch) {
	if (stage >= 0)		// >= is better than > to check for a CH less often
		return positive_stage_boost<2>(static_cast<unsigned>(stage));
	else
		return (!ch) ? negative_stage_boost<2>(static_cast<unsigned>(-stage)) : Rational(1, 1);
}

Rational defending_stage_modifier(Stage::value_type const stage, bool const ch) {
	if (stage > 0)		// > is better than >= to check for a CH less often
		return (!ch) ? positive_stage_boost<2>(static_cast<unsigned>(stage)) : Rational(1, 1);
	else
		return negative_stage_boost<2>(static_cast<unsigned>(-stage));
}

Rational speed_stage_modifier(Stage::value_type const stage) {
	return (stage >= 0) ?
		positive_stage_boost<2>(static_cast<unsigned>(stage)) :
		negative_stage_boost<2>(static_cast<unsigned>(-stage));
}

Rational accuracy_stage_modifier(Stage::value_type const stage) {
	return (stage >= 0) ?
		positive_stage_boost<3>(static_cast<unsigned>(stage)) :
		negative_stage_boost<3>(static_cast<unsigned>(-stage));
}

Rational evasion_stage_modifier(Stage::value_type const stage) {
	return (stage < 0) ?
		positive_stage_boost<3>(static_cast<unsigned>(-stage)) :
		negative_stage_boost<3>(static_cast<unsigned>(stage));
}

}	// unnamed namespace

template<>
Rational Stage::modifier<StatNames::ATK>(bool const ch) const {
	return attacking_stage_modifier(m_stages[StatNames::ATK], ch);
}

template<>
Rational Stage::modifier<StatNames::SPA>(bool const ch) const {
	return attacking_stage_modifier(m_stages[StatNames::SPA], ch);
}

template<>
Rational Stage::modifier<StatNames::DEF>(bool const ch) const {
	return defending_stage_modifier(m_stages[StatNames::DEF], ch);
}

template<>
Rational Stage::modifier<StatNames::SPD>(bool const ch) const {
	return defending_stage_modifier(m_stages[StatNames::SPD], ch);
}

template<>
Rational Stage::modifier<StatNames::SPE>() const {
	return speed_stage_modifier(m_stages[StatNames::SPE]);
}

template<>
Rational Stage::modifier<StatNames::ACC>() const {
	return accuracy_stage_modifier(m_stages[StatNames::ACC]);
}

template<>
Rational Stage::modifier<StatNames::EVA>() const {
	return evasion_stage_modifier(m_stages[StatNames::EVA]);
}

namespace {
constexpr unsigned stage_radix = max_stage + max_stage + 1;
}	// namespace

// Construct a number in base stage_radix, with each stage representing a digit.
uint64_t Stage::hash() const {
	uint64_t h = 0;
	for (auto stage : m_stages) {
		h *= stage_radix;
		// The + max_stage will always make the result non-negative
		h += static_cast<uint64_t>(stage + max_stage);
	}
	return h;
}

uint64_t Stage::max_hash() {
	return stage_radix * static_cast<uint64_t>(StatNames::END) - 1;
}

bool operator==(Stage::array const & lhs, Stage::array const & rhs) {
	return lhs.m_value == rhs.m_value;
}

bool operator==(Stage const & lhs, Stage const & rhs) {
	return lhs.m_stages == rhs.m_stages;
}

bool operator!=(Stage const & lhs, Stage const & rhs) {
	return !(lhs == rhs);
}

auto Stage::array::operator[](StatNames const index) const -> value_type const & {
	return m_value.at(index);
}
auto Stage::array::operator[](StatNames const index) -> value_type & {
	return m_value.at(index);
}

} // namespace technicalmachine
