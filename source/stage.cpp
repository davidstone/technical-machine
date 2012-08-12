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

void Stage::maximize_attack() {
	stages[Stat::ATK] = max_stage;
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
