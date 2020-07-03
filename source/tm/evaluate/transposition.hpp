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

#pragma once

#include <tm/move/moves.hpp>

#include <tm/compress.hpp>
#include <tm/weather.hpp>

#include <bounded/optional.hpp>

#include <containers/array/array.hpp>

namespace technicalmachine {

struct Team;

struct BestMove {
	Moves name;
	double score;
};

struct TranspositionTable {
	auto add_score(Team const & ai, Team const & foe, Weather, unsigned depth, BestMove) -> void;
	auto get_score(Team const & ai, Team const & foe, Weather, unsigned depth) const -> bounded::optional<BestMove>;
private:
	using CompressedBattle = bounded::tuple<
		bounded::integer<0, bounded::detail::normalize<(17476366957422410805543616810844159999_bi).value()>>,
		bounded::integer<0, bounded::detail::normalize<(18297444554530391011428210604769279999_bi).value()>>,
		bounded::integer<0, bounded::detail::normalize<(1179924466646960606084156227583999999_bi).value()>>,
		bounded::integer<0, bounded::detail::normalize<(95341770858580775465420294432143441919_bi).value()>>,
		bounded::integer<0, bounded::detail::normalize<(17476366957422410805543616810844159999_bi).value()>>,
		bounded::integer<0, bounded::detail::normalize<(18297444554530391011428210604769279999_bi).value()>>,
		bounded::integer<0, bounded::detail::normalize<(109252265430274130192977428479999_bi).value()>>,
		bounded::integer<0, bounded::detail::normalize<(95341770858580775465420294432143441919_bi).value()>>
	>;

	struct Value {
		CompressedBattle compressed_battle = {0_bi, 0_bi, 0_bi, 0_bi, 0_bi, 0_bi, 0_bi, 0_bi};
		unsigned depth = 0;
		Moves move = {};
		double score = 0.0;
	};
	static constexpr auto size = bounded::constant<1 << 13>;
	using Data = containers::array<Value, size.value()>;
	using Index = containers::index_type<Data>;

	static auto index(CompressedBattle const & compressed_battle) -> Index;

	Data m_data;
};

}	// namespace technicalmachine
