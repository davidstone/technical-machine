// Stat stages
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

#include "stage.hpp"

#include "../rational.hpp"

namespace technicalmachine {
using namespace bounded::literal;
namespace {
constexpr auto max_stage = 6;
}

Stage::Stage() {
	std::fill(m_stages.begin(), m_stages.end(), 0_bi);
}

auto Stage::operator[](StatNames const index) const -> value_type const & {
	return m_stages[container_type::index_type(index, bounded::non_check)];
}
auto Stage::operator[](StatNames const index) -> value_type & {
	return m_stages[container_type::index_type(index, bounded::non_check)];
}

auto Stage::begin() const -> container_type::const_iterator {
	return m_stages.begin();
}
auto Stage::begin() -> container_type::iterator {
	return m_stages.begin();
}
auto Stage::end() const -> container_type::const_iterator {
	return m_stages.end();
}
auto Stage::end() -> container_type::iterator {
	return m_stages.end();
}


auto boost(Stage & stage, StatNames const stat, Stage::boost_type const number_of_stages) -> void {
	stage[stat] += number_of_stages;
}

auto boost_regular(Stage & stage, Stage::boost_type const number_of_stages) -> void {
	for (auto const stat : { StatNames::ATK, StatNames::DEF, StatNames::SPA, StatNames::SPD, StatNames::SPE }) {
		boost(stage, stat, number_of_stages);
	}
}

auto boost_physical(Stage & stage, Stage::boost_type const number_of_stages) -> void {
	for (auto const stat : { StatNames::ATK, StatNames::DEF }) {
		boost(stage, stat, number_of_stages);
	}
}
auto boost_special(Stage & stage, Stage::boost_type const number_of_stages) -> void {
	for (auto const stat : { StatNames::SPA, StatNames::SPD }) {
		boost(stage, stat, number_of_stages);
	}
}
auto boost_defensive(Stage & stage, Stage::boost_type const number_of_stages) -> void {
	for (auto const stat : { StatNames::DEF, StatNames::SPD }) {
		boost(stage, stat, number_of_stages);
	}
}
auto boost_offensive(Stage & stage, Stage::boost_type const number_of_stages) -> void {
	for (auto const stat : { StatNames::ATK, StatNames::SPA }) {
		boost(stage, stat, number_of_stages);
	}
}

namespace {

void swap_specified(Stage & lhs, Stage & rhs, std::initializer_list<StatNames> const & stats) {
	using std::swap;
	for (auto const stat : stats) {
		swap(lhs[stat], rhs[stat]);
	}
}

}	// namespace

auto swap_defensive(Stage & lhs, Stage & rhs) -> void {
	swap_specified(lhs, rhs, { StatNames::DEF, StatNames::SPD });
}
auto swap_offensive(Stage & lhs, Stage & rhs) -> void {
	swap_specified(lhs, rhs, { StatNames::ATK, StatNames::SPA });
}

namespace {
constexpr unsigned stage_radix = max_stage + max_stage + 1;
}	// namespace

// Construct a number in base stage_radix, with each stage representing a digit.
auto Stage::hash() const -> hash_type {
	hash_type h = 0;
	for (auto stage : m_stages) {
		h *= stage_radix;
		// The + max_stage will always make the result non-negative
		h += static_cast<hash_type>(stage + max_stage);
	}
	return h;
}

auto Stage::max_hash() -> hash_type {
	return stage_radix * static_cast<hash_type>(StatNames::END) - 1;
}

auto operator==(Stage const & lhs, Stage const & rhs) -> bool {
	return std::equal(lhs.begin(), lhs.end(), rhs.begin());
}
auto operator!=(Stage const & lhs, Stage const & rhs) -> bool {
	return !(lhs == rhs);
}

} // namespace technicalmachine
