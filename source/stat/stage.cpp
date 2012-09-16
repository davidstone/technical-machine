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

#include "stage.hpp"

#include <algorithm>

#include "../rational.hpp"

namespace technicalmachine {
namespace {
constexpr int max_stage = 6;
}

Stage::Stage () :
	stages({{}})
	{
}

void Stage::reset() {
	std::fill(stages.begin(), stages.end(), 0);
}

void Stage::boost (int8_t & stage, int const n) {
	stage = std::min(max_stage, std::max(-max_stage, stage + n));
}

void Stage::boost (Stat::Stats const stat, int const n) {
	boost(stages[stat], n);
}

void Stage::boost(std::initializer_list<Stat::Stats> const & stats, int n) {
	for (auto const stat : stats)
		boost(stat, n);
}

unsigned Stage::accumulate(std::function<unsigned(int)> const & f) const {
	return std::accumulate(stages.begin(), stages.end(), 0u,[& f](unsigned const initial, int const stage) {
		return initial + f(stage);
	});
}

int Stage::dot_product(Stage const & stage, std::array<int, Stat::END> const & multiplier) {
	int result = 0;
	for (unsigned n = 0; n != multiplier.size(); ++n) {
		result += stage[n] * multiplier[n];
	}
	return result;
}
int Stage::dot_product(std::array<int, Stat::END> const & multiplier, Stage const & stage) {
	return dot_product(stage, multiplier);
}

void Stage::boost_regular(int const n) {
	boost({ Stat::ATK, Stat::DEF, Stat::SPA, Stat::SPD, Stat::SPE }, n);
}

void Stage::boost_physical(int n) {
	boost({ Stat::ATK, Stat::DEF }, n);
}
void Stage::boost_special(int n) {
	boost({ Stat::SPA, Stat::SPD }, n);
}
void Stage::boost_defensive(int n) {
	boost({ Stat::DEF, Stat::SPD }, n);
}
void Stage::boost_offensive(int n) {
	boost({ Stat::ATK, Stat::SPA }, n);
}

void Stage::swap_specified(Stage & lhs, Stage & rhs, std::initializer_list<Stat::Stats> const & stats) {
	using std::swap;
	for (auto const stat : stats)
		swap(lhs[stat], rhs[stat]);
}
void Stage::swap_defensive(Stage & lhs, Stage & rhs) {
	swap_specified(lhs, rhs, { Stat::DEF, Stat::SPD });
}
void Stage::swap_offensive(Stage & lhs, Stage & rhs) {
	swap_specified(lhs, rhs, { Stat::ATK, Stat::SPA });
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

Rational attacking_stage_modifier(int const stage, bool const ch) {
	if (stage >= 0)		// >= is better than > to check for a CH less often
		return positive_stage_boost<2>(static_cast<unsigned>(stage));
	else
		return (!ch) ? negative_stage_boost<2>(static_cast<unsigned>(-stage)) : Rational(1, 1);
}

Rational defending_stage_modifier(int const stage, bool const ch) {
	if (stage > 0)		// > is better than >= to check for a CH less often
		return (!ch) ? positive_stage_boost<2>(static_cast<unsigned>(stage)) : Rational(1, 1);
	else
		return negative_stage_boost<2>(static_cast<unsigned>(-stage));
}

Rational speed_stage_modifier(int const stage) {
	return (stage >= 0) ?
		positive_stage_boost<2>(static_cast<unsigned>(stage)) :
		negative_stage_boost<2>(static_cast<unsigned>(-stage));
}

Rational accuracy_stage_modifier(int const stage) {
	return (stage >= 0) ?
		positive_stage_boost<3>(static_cast<unsigned>(stage)) :
		negative_stage_boost<3>(static_cast<unsigned>(-stage));
}

Rational evasion_stage_modifier(int const stage) {
	return (stage < 0) ?
		positive_stage_boost<3>(static_cast<unsigned>(-stage)) :
		negative_stage_boost<3>(static_cast<unsigned>(stage));
}

}	// unnamed namespace

template<>
Rational Stage::modifier<Stat::ATK>(bool const ch) const {
	return attacking_stage_modifier(stages[Stat::ATK], ch);
}

template<>
Rational Stage::modifier<Stat::SPA>(bool const ch) const {
	return attacking_stage_modifier(stages[Stat::SPA], ch);
}

template<>
Rational Stage::modifier<Stat::DEF>(bool const ch) const {
	return defending_stage_modifier(stages[Stat::DEF], ch);
}

template<>
Rational Stage::modifier<Stat::SPD>(bool const ch) const {
	return defending_stage_modifier(stages[Stat::SPD], ch);
}

template<>
Rational Stage::modifier<Stat::SPE>() const {
	return speed_stage_modifier(stages[Stat::SPE]);
}

template<>
Rational Stage::modifier<Stat::ACC>() const {
	return accuracy_stage_modifier(stages[Stat::ACC]);
}

template<>
Rational Stage::modifier<Stat::EVA>() const {
	return evasion_stage_modifier(stages[Stat::EVA]);
}

namespace {
constexpr unsigned stage_radix = max_stage + max_stage + 1;
}	// unnamed namespace

// Construct a number in base stage_radix, with each stage representing a digit.
uint64_t Stage::hash() const {
	uint64_t h = 0;
	for (auto stage : stages) {
		h *= stage_radix;
		// The + max_stage will always make the result non-negative
		h += static_cast<uint64_t>(stage + max_stage);
	}
	return h;
}

uint64_t Stage::max_hash() {
	return stage_radix * Stat::END - 1;
}

bool operator==(Stage const & lhs, Stage const & rhs) {
	return lhs.stages == rhs.stages;
}

bool operator!=(Stage const & lhs, Stage const & rhs) {
	return !(lhs == rhs);
}

int8_t const & Stage::operator[](size_t const index) const {
	return stages[index];
}
int8_t & Stage::operator[](size_t const index) {
	return stages[index];
}

} // namespace technicalmachine
