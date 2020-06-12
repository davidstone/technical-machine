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

#include <tm/stat/stat_to_ev.hpp>

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
constexpr auto nature_hp_iv = "\n\tIVs: "sv;
constexpr auto hp_iv_atk_iv = " HP / "sv;
constexpr auto atk_iv_def_iv = " Atk / "sv;
constexpr auto def_iv_spa_iv = " Def / "sv;
constexpr auto spa_iv_spd_iv = " SpA / "sv;
constexpr auto spd_iv_spe_iv = " SpD / "sv;
constexpr auto spe_iv_hp_ev= " Spe\n\tEVs: "sv;
constexpr auto hp_ev_atk_ev = " HP / "sv;
constexpr auto atk_ev_def_ev = " Atk / "sv;
constexpr auto def_ev_spa_ev = " Def / "sv;
constexpr auto spa_ev_spd_ev = " SpA / "sv;
constexpr auto spd_ev_spe_ev = " SpD / "sv;
constexpr auto spe_ev_moves = " Spe"sv;
constexpr auto moves_separator = "\n\t- "sv;

} // namespace

// TODO: Print level
// TODO: Print gender
// TODO: Make this compatible with Pokemon Showdown

auto to_string(Generation const generation, Pokemon const pokemon) -> containers::string {
	// Boost.Format fails to compile with C++20, so we have to do this instead
	auto hp_to_string = [&] {
		auto const buffer = std::to_string(100.0 * static_cast<double>(hp_ratio(pokemon)));
		auto const it = containers::find(buffer, '.');
		auto const last = (end(buffer) - it <= 2) ? end(buffer) : it + 2;
		return containers::string(containers::range_view(begin(buffer), last));
	};

	auto status_to_string = [&] {
		auto const output_status = !is_clear(pokemon.status());
		return output_status ?
			containers::concatenate<containers::string>(ability_status, to_string(pokemon.status().name())) :
			containers::string("");
	};

	auto stats = calculate_ivs_and_evs(generation, pokemon);
	
	auto stat_to_iv_string = [&](PermanentStat const stat_name) {
		return bounded::to_string(stats[stat_name].iv.value());
	};
	
	auto stat_to_ev_string = [&](PermanentStat const stat_name) {
		return bounded::to_string(stats[stat_name].ev.value());
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
	containers::uninitialized_copy(containers::begin(std::move(str)), containers::end(std::move(str)), begin(out));
	containers::concatenate<containers::string>(std::string());

	return containers::concatenate<containers::string>(
		to_string(pokemon.species()),
		species_hp, hp_to_string(),
		hp_item, to_string(pokemon.unmodified_item()),
		item_ability, to_string(pokemon.initial_ability()),
		status_to_string(),
		status_nature, to_string(pokemon.nature()),
		nature_hp_iv, stat_to_iv_string(PermanentStat::hp),
		hp_iv_atk_iv, stat_to_iv_string(PermanentStat::atk),
		atk_iv_def_iv, stat_to_iv_string(PermanentStat::def),
		def_iv_spa_iv, stat_to_iv_string(PermanentStat::spa),
		spa_iv_spd_iv, stat_to_iv_string(PermanentStat::spd),
		spd_iv_spe_iv, stat_to_iv_string(PermanentStat::spe),
		spe_iv_hp_ev, stat_to_ev_string(PermanentStat::hp),
		hp_ev_atk_ev, stat_to_ev_string(PermanentStat::atk),
		atk_ev_def_ev, stat_to_ev_string(PermanentStat::def),
		def_ev_spa_ev, stat_to_ev_string(PermanentStat::spa),
		spa_ev_spd_ev, stat_to_ev_string(PermanentStat::spd),
		spd_ev_spe_ev, stat_to_ev_string(PermanentStat::spe),
		spe_ev_moves, moves_to_string()
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

template<typename T>
constexpr auto pop_value_type(BufferView<std::string_view> & buffer, std::string_view const delimiter) {
	return T(typed_pop<typename T::value_type>(buffer, delimiter));
}

} // namespace

auto pokemon_from_string(Generation const generation, std::string_view const str, TeamSize const team_size) -> Pokemon {
	auto buffer = BufferView(str);
	auto const species = typed_pop<Species>(buffer, species_hp);
	auto const hp_percent = typed_pop<double>(buffer, hp_item);
	auto const item = typed_pop<Item>(buffer, item_ability);
	auto const [ability, status] = pop_ability_and_status(buffer);
	auto const nature = typed_pop<Nature>(buffer, nature_hp_iv);

	auto pokemon = Pokemon(generation, team_size, species, Level(100_bi), Gender::genderless, item, ability, nature);

	pokemon.set_status(status);

	auto const ivs = IVs{
		pop_value_type<IV>(buffer, hp_iv_atk_iv),
		pop_value_type<IV>(buffer, atk_iv_def_iv),
		pop_value_type<IV>(buffer, def_iv_spa_iv),
		pop_value_type<IV>(buffer, spa_iv_spd_iv),
		pop_value_type<IV>(buffer, spd_iv_spe_iv),
		pop_value_type<IV>(buffer, spe_iv_hp_ev),
	};
	auto const evs = GenericStats<EV>{
		pop_value_type<EV>(buffer, hp_ev_atk_ev),
		pop_value_type<EV>(buffer, atk_ev_def_ev),
		pop_value_type<EV>(buffer, def_ev_spa_ev),
		pop_value_type<EV>(buffer, spa_ev_spd_ev),
		pop_value_type<EV>(buffer, spd_ev_spe_ev),
		pop_value_type<EV>(buffer, spe_ev_moves),
	};
	for (auto const stat_name : containers::enum_range<PermanentStat>()) {
		pokemon.set_ev(generation, stat_name, ivs[stat_name], evs[stat_name]);
	}
	pokemon.set_hp(HP::current_type(static_cast<int>(static_cast<double>(pokemon.hp().max()) * hp_percent / 100.0)));

	auto const should_be_empty = pop_to_delimiter(buffer, moves_separator);
	if (!should_be_empty.empty()) {
		throw std::runtime_error("Expected empty string while parsing Pokemon string, got " + std::string(should_be_empty));
	}

	auto & moves = pokemon.all_moves();
	while (!buffer.remainder().empty()) {
		add_seen_move(moves, generation, typed_pop<Moves>(buffer, moves_separator));
	}

	return pokemon;
}

}	// namespace technicalmachine
