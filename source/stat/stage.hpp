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

#pragma once

#include "stat_names.hpp"
#include "../hash.hpp"
#include "../rational.hpp"

#include <bounded_integer/array.hpp>
#include <bounded_integer/bounded_integer.hpp>

#include <algorithm>
#include <cstdint>

namespace technicalmachine {

using namespace bounded::literal;

struct Stage {
	using value_type = bounded::clamped_integer<-6, 6>;
	using boost_type = bounded::checked_integer<-3, 12>;
	static constexpr auto number_of_stats = bounded::make<static_cast<intmax_t>(StatNames::END)>();
	using container_type = bounded::array<value_type, number_of_stats.value()>;

	Stage();

	auto operator[](StatNames index) const -> value_type const &;
	auto operator[](StatNames index) -> value_type &;
	
	auto begin() const -> container_type::const_iterator;
	auto begin() -> container_type::iterator;
	auto end() const -> container_type::const_iterator;
	auto end() -> container_type::iterator;

private:
	container_type m_stages;
};

inline auto hash(Stage const stage) noexcept {
	return hash_range<Stage::container_type::size_type>(stage.begin(), stage.end());
}

namespace detail {

template<StatNames stat>
struct Base {
	static constexpr auto value = 2_bi;
};
template<>
struct Base<StatNames::ACC> {
	static constexpr auto value = 3_bi;
};
template<>
struct Base<StatNames::EVA> {
	static constexpr auto value = 3_bi;
};

}	// namespace detail

template<StatNames stat, enable_if_t<stat == StatNames::ATK or stat == StatNames::SPA> = clang_dummy>
auto modifier(Stage const & stage, bool const ch) {
	constexpr auto base = detail::Base<stat>::value;
	return BOUNDED_CONDITIONAL((stage[stat] >= 0_bi),
		make_rational(base + bounded::abs(stage[stat]), base),
		make_rational(base, BOUNDED_CONDITIONAL(!ch, base + bounded::abs(stage[stat]), base))
	);
}

template<StatNames stat, enable_if_t<stat == StatNames::DEF or stat == StatNames::SPD> = clang_dummy>
auto modifier(Stage const & stage, bool const ch) {
	constexpr auto base = detail::Base<stat>::value;
	return BOUNDED_CONDITIONAL((stage[stat] <= 0_bi),
		make_rational(base, base + bounded::abs(stage[stat])),
		make_rational(BOUNDED_CONDITIONAL(!ch, base + bounded::abs(stage[stat]), base), base)
	);
}

template<StatNames stat, enable_if_t<stat == StatNames::SPE or stat == StatNames::ACC or stat == StatNames::EVA> = clang_dummy>
auto modifier(Stage const & stage) {
	constexpr auto base = detail::Base<stat>::value;
	return BOUNDED_CONDITIONAL((stage[stat] >= 0_bi),
		make_rational(base + bounded::abs(stage[stat]), base),
		make_rational(base, base + bounded::abs(stage[stat]))
	);
}


auto boost(Stage & stage, StatNames stat, Stage::boost_type number_of_stages) -> void;
auto boost_regular(Stage & stage, Stage::boost_type number_of_stages) -> void;
auto boost_physical(Stage & stage, Stage::boost_type number_of_stages) -> void;
auto boost_special(Stage & stage, Stage::boost_type number_of_stages) -> void;
auto boost_defensive(Stage & stage, Stage::boost_type number_of_stages) -> void;
auto boost_offensive(Stage & stage, Stage::boost_type number_of_stages) -> void;

template<typename Function>
auto accumulate(Stage const & stages, Function && f) {
	using sum_type = decltype(f(std::declval<Stage::value_type>()) * Stage::number_of_stats);
	return std::accumulate(stages.begin(), stages.end(), sum_type(0_bi), [& f](sum_type const initial, Stage::value_type const stage) {
		return initial + f(stage);
	});
}

inline auto positive_boosts(Stage const & stage) {
	auto const positive_values = [](Stage::value_type const check_stage) {
		return bounded::max(check_stage, 0_bi);
	};
	return accumulate(stage, positive_values);
}

auto swap_defensive(Stage & lhs, Stage & rhs) -> void;
auto swap_offensive(Stage & lhs, Stage & rhs) -> void;


auto operator==(Stage const & lhs, Stage const & rhs) -> bool;
auto operator!=(Stage const & lhs, Stage const & rhs) -> bool;

}	// namespace technicalmachine
