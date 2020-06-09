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

#include <tm/string_conversions/pokemon.hpp>

#include <tm/string_conversions/ability.hpp>
#include <tm/string_conversions/item.hpp>
#include <tm/string_conversions/move.hpp>
#include <tm/string_conversions/nature.hpp>
#include <tm/string_conversions/species.hpp>
#include <tm/string_conversions/status.hpp>

#include <tm/buffer_view.hpp>

#include <bounded/to_integer.hpp>

#include <containers/algorithms/concatenate.hpp>
#include <containers/algorithms/find.hpp>
#include <containers/array/array.hpp>
#include <containers/range_view.hpp>

namespace technicalmachine {
namespace {

using namespace std::string_view_literals;

constexpr auto species_hp = " ("sv;
constexpr auto hp_item = "% HP) @ "sv;
constexpr auto item_ability = "\n\tAbility: "sv;
constexpr auto ability_status = "\n\tStatus: "sv;
constexpr auto status_nature = "\n\tNature: "sv;
constexpr auto nature_hp = "\n\t"sv;
constexpr auto hp_atk = " HP / "sv;
constexpr auto atk_def = " Atk / "sv;
constexpr auto def_spa = " Def / "sv;
constexpr auto spa_spd = " SpA / "sv;
constexpr auto spd_spe = " SpD / "sv;
constexpr auto spe_moves = " Spe"sv;
constexpr auto moves_separator = "\n\t- "sv;

} // namespace

// TODO: Print level
// TODO: Print gender
// TODO: Print IVs if they are not the default
// TODO: Make this compatible with Pokemon Showdown

auto to_string(Pokemon const pokemon) -> containers::string {
	// Boost.Format fails to compile with C++20, so we have to do this instead
	auto hp_to_string = [&] {
		auto const buffer = std::to_string(100.0 * static_cast<double>(hp_ratio(pokemon)));
		auto const it = containers::find(buffer, '.');
		auto const last = (end(buffer) - it <= 2) ? end(buffer) : it + 2;
		return containers::string(containers::range_view(begin(buffer), last));
	};

	auto status_to_string = [&] {
		auto const output_status = !is_clear(get_status(pokemon));
		return output_status ?
			containers::concatenate<containers::string>(ability_status, to_string(get_status(pokemon).name())) :
			containers::string("");
	};
	
	auto stat_to_string = [&](RegularStat const stat) {
		return bounded::to_string(get_stat(pokemon, stat).ev().value());
	};
	
	auto moves_to_string = [&]{
		containers::string output;
		for (auto const & move : regular_moves(pokemon)) {
			output = containers::concatenate<containers::string>(std::move(output), moves_separator, to_string(move.name()));
		}
		return output;
	};

	auto str = std::string();
	auto out = containers::string();
	auto it = containers::begin(std::move(str));
	++it;
	containers::uninitialized_copy(containers::begin(std::move(str)), containers::end(std::move(str)), begin(out));
	containers::concatenate<containers::string>(std::string());

	return containers::concatenate<containers::string>(
		to_string(get_species(pokemon)),
		species_hp, hp_to_string(),
		hp_item, to_string(pokemon.unmodified_item()),
		item_ability, to_string(pokemon.initial_ability()),
		status_to_string(),
		status_nature, to_string(get_nature(pokemon)),
		nature_hp, std::string_view(bounded::to_string(get_hp(pokemon).ev().value())),
		hp_atk, stat_to_string(RegularStat::atk),
		atk_def, stat_to_string(RegularStat::def),
		def_spa, stat_to_string(RegularStat::spa),
		spa_spd, stat_to_string(RegularStat::spd),
		spd_spe, stat_to_string(RegularStat::spe),
		spe_moves, moves_to_string()
	);
}

namespace {

template<typename T>
constexpr auto typed_pop(BufferView<std::string_view> & buffer, std::string_view const delimiter) {
	auto const str = pop_to_delimiter(buffer, delimiter);
	if constexpr (std::is_same_v<T, double>) {
		// TODO: Use std::from_chars when that is implemented
		return std::stod(std::string(str));
	} else if constexpr (bounded::bounded_integer<T>) {
		return bounded::to_integer<T>(str);
	} else {
		return from_string<T>(str);
	}
}

struct AbilityAndStatus {
	Ability ability;
	Statuses status;
};

constexpr auto pop_ability_and_status(BufferView<std::string_view> & buffer) {
	auto const ability_and_status_str = pop_to_delimiter(buffer, status_nature);
	auto const [ability_str, status_str] = split_view(ability_and_status_str, ability_status);
	return AbilityAndStatus{
		from_string<Ability>(ability_str),
		status_str.empty() ? Statuses::clear : from_string<Statuses>(status_str)
	};
}

constexpr auto pop_ev(BufferView<std::string_view> & buffer, std::string_view const delimiter) {
	return EV(typed_pop<EV::value_type>(buffer, delimiter));
}

} // namespace

auto pokemon_from_string(std::string_view const str, Generation const generation, TeamSize const team_size) -> Pokemon {
	auto buffer = BufferView(str);
	auto const species = typed_pop<Species>(buffer, species_hp);
	auto const hp_percent = typed_pop<double>(buffer, hp_item);
	auto const item = typed_pop<Item>(buffer, item_ability);
	auto const [ability, status] = pop_ability_and_status(buffer);
	auto const nature = typed_pop<Nature>(buffer, nature_hp);

	auto const iv = IV(default_iv(generation));

	auto pokemon = Pokemon(generation, team_size, species, Level(100_bi), Gender::genderless, item, ability, nature);

	pokemon.set_status(status);

	set_hp_ev(generation, pokemon, iv, pop_ev(buffer, hp_atk));
	pokemon.set_hp(HP::current_type(static_cast<int>(static_cast<double>(get_hp(pokemon).max()) * hp_percent / 100.0)));

	auto set_stat = [&](RegularStat const stat, EV const ev) {
		set_stat_ev(pokemon, stat, iv, ev);
	};
	set_stat(RegularStat::atk, pop_ev(buffer, atk_def));
	set_stat(RegularStat::def, pop_ev(buffer, def_spa));
	set_stat(RegularStat::spa, pop_ev(buffer, spa_spd));
	set_stat(RegularStat::spd, pop_ev(buffer, spd_spe));
	set_stat(RegularStat::spe, pop_ev(buffer, spe_moves));

	auto const should_be_empty = pop_to_delimiter(buffer, moves_separator);
	if (!should_be_empty.empty()) {
		throw std::runtime_error("Expected empty string while parsing Pokemon string, got " + std::string(should_be_empty));
	}

	auto & moves = all_moves(pokemon);
	while (!buffer.remainder().empty()) {
		add_seen_move(moves, generation, typed_pop<Moves>(buffer, moves_separator));
	}

	return pokemon;
}

}	// namespace technicalmachine
