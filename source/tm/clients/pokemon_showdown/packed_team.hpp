// Copyright (C) 2019 David Stone
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

#include <tm/stat/stat_to_ev.hpp>

#include <tm/string_conversions/ability.hpp>
#include <tm/string_conversions/gender.hpp>
#include <tm/string_conversions/item.hpp>
#include <tm/string_conversions/move.hpp>
#include <tm/string_conversions/nature.hpp>
#include <tm/string_conversions/species.hpp>

#include <tm/buffer_view.hpp>
#include <tm/team.hpp>

#include <bounded/to_integer.hpp>

#include <containers/integer_range.hpp>
#include <containers/string.hpp>

#include <string_view>

namespace technicalmachine {

enum class Generation : std::uint8_t;

namespace ps {

template<Generation generation>
auto to_packed_format(Team<generation> const & team) -> containers::string {
	auto result = containers::string();
	for (auto const & pokemon : team.all_pokemon()) {
		if (!empty(result)) {
			result += ']';
		}
		result += to_string(pokemon.species());
		result += "||";
		auto const item = pokemon.item(false, false);
		result += item == Item::None ? "" : to_string(item);
		result += '|';
		result += to_string(pokemon.initial_ability());
		result += '|';
		auto const moves = pokemon.regular_moves();
		for (auto it = begin(moves); it != end(moves); ++it) {
			if (it != begin(moves)) {
				result += ',';
			}
			result += to_string(it->name());
		}
		result += '|';
		result += to_string(pokemon.nature());

		result += '|';
		auto const stats = calculate_ivs_and_evs(pokemon);

		for (auto const stat_name : containers::enum_range<PermanentStat>()) {
			if (stat_name != PermanentStat::hp) {
				result += ',';
			}
			result += std::string_view(to_string(stats[stat_name].ev.value()));
		}

		result += '|';
		result += to_string(pokemon.gender());

		result += '|';
		for (auto const stat_name : containers::enum_range<PermanentStat>()) {
			if (stat_name != PermanentStat::hp) {
				result += ',';
			}
			result += std::string_view(to_string(stats[stat_name].iv.value()));
		}

		result += '|';
		// Assume non-shiny
		result += '|';
		result += std::string_view(to_string(pokemon.level()()));
		result += '|';
		// Assume max happiness
		// Assume regular Poke Ball
		// Generations with Hyper Training are not yet supported
	}
	return result;
}

inline constexpr auto pokemon_delimiter = ']';

inline auto parse_species(std::string_view const species_str, std::string_view const nickname) {
	return from_string<Species>(species_str.empty() ? nickname : species_str);
}

constexpr auto parse_ability(std::string_view const ability_str, Species const species [[maybe_unused]]) {
	// TODO: Get the correct values for this
	return
		ability_str == "" ? Ability::Honey_Gather :
		ability_str == "0" ? Ability::Honey_Gather :
		ability_str == "1" ? Ability::Honey_Gather :
		ability_str == "H" ? Ability::Honey_Gather :
		from_string<Ability>(ability_str);
}

constexpr auto parse_moves(std::string_view const str, Generation const generation) {
	auto buffer = DelimitedBufferView(str, ',');
	auto moves = RegularMoves();
	while (!buffer.remainder().empty()) {
		moves.push_back(Move(generation, from_string<Moves>(buffer.pop())));
	}
	return moves;
}

constexpr auto parse_nature(std::string_view const str) {
	return str.empty() ? Nature::Serious : from_string<Nature>(str);
}

template<typename T>
constexpr auto parse_stat_components(std::string_view const str, T default_value) {
	auto buffer = DelimitedBufferView(str, ',');
	auto next = [&] {
		auto maybe = buffer.pop();
		return maybe.empty() ? default_value : T(bounded::to_integer<typename T::value_type>(maybe));
	};
	// Order of evaluation is defined with {} init
	return GenericStats<T>{
		next(),
		next(),
		next(),
		next(),
		next(),
		next()
	};
}

inline auto parse_gender(std::string_view const str) {
	return str.empty() ? Gender::genderless : from_string<Gender>(str);
}

constexpr auto parse_shiny(std::string_view const str) {
	return
		str == "S" ? true :
		str == "" ? false :
		throw std::runtime_error("Invalid shiny string: " + std::string(str));
}

template<typename T>
auto parse_integer_wrapper(std::string_view const str) {
	using integer = typename T::value_type;
	return T(str.empty() ? bounded::max_value<integer> : bounded::to_integer<integer>(str));
}

template<Generation generation>
auto parse_pokemon(std::string_view const str) {
	auto buffer = DelimitedBufferView(str, '|');
	auto const nickname = buffer.pop();
	auto const species = parse_species(buffer.pop(), nickname);
	auto const item = from_string<Item>(buffer.pop());
	auto const ability = parse_ability(buffer.pop(), species);
	auto const moves = parse_moves(buffer.pop(), generation);
	auto const nature = parse_nature(buffer.pop());
	auto const evs = parse_stat_components(buffer.pop(), EV(0_bi));
	auto const gender = parse_gender(buffer.pop());
	auto const ivs = parse_stat_components(buffer.pop(), default_iv(generation));
	auto const shiny [[maybe_unused]] = parse_shiny(buffer.pop());
	auto const level = parse_integer_wrapper<Level>(buffer.pop());
	auto const happiness = parse_integer_wrapper<Happiness>(buffer.pop(','));
	auto const pokeball [[maybe_unused]] = buffer.pop(',');
	// TODO: Support Hyper Training
	auto const hidden_power_type [[maybe_unused]] = buffer.remainder();
	auto pokemon = Pokemon<generation>(
		species,
		level,
		gender,
		item,
		ability,
		combine(nature, ivs, evs),
		moves,
		happiness
	);
	return pokemon;
}

template<Generation generation>
auto packed_format_to_team(std::string_view const str, TeamSize const team_size) -> Team<generation> {
	auto buffer = DelimitedBufferView(str, pokemon_delimiter);

	constexpr auto is_me = true;
	auto team = Team<generation>(team_size, is_me);

	while (!buffer.remainder().empty()) {
		team.add_pokemon(parse_pokemon<generation>(buffer.pop()));
	}
	return team;
}

template<Generation generation>
auto packed_format_to_team(std::string_view const str) -> Team<generation> {
	auto const team_size = bounded::check_in_range<TeamSize>(bounded::integer(std::count(str.begin(), str.end(), pokemon_delimiter) + 1));
	return packed_format_to_team<generation>(str, team_size);
}

} // namespace ps
} // namespace technicalmachine
