// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

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

#include <containers/algorithms/all_any_none.hpp>
#include <containers/algorithms/concatenate.hpp>
#include <containers/algorithms/find.hpp>
#include <containers/array/array.hpp>
#include <containers/begin_end.hpp>
#include <containers/push_back.hpp>
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

template<Generation generation>
auto to_string(Pokemon<generation> const pokemon) -> containers::string {
	// Boost.Format fails to compile with C++20, so we have to do this instead
	auto const hp_str_full = std::to_string(100.0 * static_cast<double>(hp_ratio(pokemon)));
	auto hp_to_string = [&] {
		auto const pos = hp_str_full.find('.');
		constexpr auto after_decimal = 2;
		auto const size = (pos != std::string::npos and hp_str_full.size() - pos > after_decimal) ? pos + after_decimal : hp_str_full.size();
		return std::string_view(hp_str_full.data(), size);
	};

	auto status_to_string = [&] {
		auto const output_status = !is_clear(pokemon.status());
		return output_status ?
			containers::concatenate<containers::string>(ability_status, to_string(pokemon.status().name())) :
			containers::string("");
	};

	auto stats = calculate_ivs_and_evs(pokemon);
	
	auto stat_to_iv_string = [&](PermanentStat const stat_name) {
		return bounded::to_string(stats[stat_name].iv.value());
	};
	
	auto stat_to_ev_string = [&](PermanentStat const stat_name) {
		return bounded::to_string(stats[stat_name].ev.value());
	};
	
	auto moves_to_string = [&]{
		containers::string output;
		for (auto const & move : pokemon.regular_moves()) {
			output = containers::concatenate<containers::string>(std::move(output), moves_separator, to_string(move.name()));
		}
		return output;
	};

	return containers::concatenate<containers::string>(
		to_string(pokemon.species()),
		species_hp,
		hp_to_string(),
		hp_item,
		to_string(pokemon.item(false, false)),
		item_ability,
		to_string(pokemon.initial_ability()),
		status_to_string(),
		status_nature,
		to_string(pokemon.nature()),
		nature_hp_iv,
		stat_to_iv_string(PermanentStat::hp),
		hp_iv_atk_iv,
		stat_to_iv_string(PermanentStat::atk),
		atk_iv_def_iv,
		stat_to_iv_string(PermanentStat::def),
		def_iv_spa_iv,
		stat_to_iv_string(PermanentStat::spa),
		spa_iv_spd_iv,
		stat_to_iv_string(PermanentStat::spd),
		spd_iv_spe_iv,
		stat_to_iv_string(PermanentStat::spe),
		spe_iv_hp_ev,
		stat_to_ev_string(PermanentStat::hp),
		hp_ev_atk_ev,
		stat_to_ev_string(PermanentStat::atk),
		atk_ev_def_ev,
		stat_to_ev_string(PermanentStat::def),
		def_ev_spa_ev,
		stat_to_ev_string(PermanentStat::spa),
		spa_ev_spd_ev,
		stat_to_ev_string(PermanentStat::spd),
		spd_ev_spe_ev,
		stat_to_ev_string(PermanentStat::spe),
		spe_ev_moves,
		moves_to_string()
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

auto pop_ability_and_status(BufferView<std::string_view> & buffer) {
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

template<Generation generation>
auto pokemon_from_string(std::string_view const str) -> Pokemon<generation> {
	auto buffer = BufferView(str);
	auto const species = typed_pop<Species>(buffer, species_hp);
	auto const hp_percent = typed_pop<double>(buffer, hp_item);
	auto const item = typed_pop<Item>(buffer, item_ability);
	auto const [ability, status] = pop_ability_and_status(buffer);
	auto const nature = typed_pop<Nature>(buffer, nature_hp_iv);

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

	auto const should_be_empty = pop_to_delimiter(buffer, moves_separator);
	if (!should_be_empty.empty()) {
		throw std::runtime_error(containers::concatenate<std::string>("Expected empty string while parsing Pokemon string, got "sv, should_be_empty));
	}

	auto moves = RegularMoves();
	while (!buffer.remainder().empty()) {
		auto const move_name = typed_pop<Moves>(buffer, moves_separator);
		containers::push_back(moves, Move(generation, move_name));
	}

	auto pokemon = Pokemon<generation>(
		species,
		Level(100_bi),
		Gender::genderless,
		item,
		ability,
		combine(nature, ivs, evs),
		moves
	);
	pokemon.set_hp(HP::current_type(static_cast<int>(static_cast<double>(pokemon.hp().max()) * hp_percent / 100.0)));
	pokemon.set_status(status);

	return pokemon;
}

#define TECHNICALMACHINE_EXPLICIT_INSTANTIATION(generation) \
	template auto to_string<generation>(Pokemon<generation> const pokemon) -> containers::string; \
	template auto pokemon_from_string<generation>(std::string_view const str) -> Pokemon<generation>

TECHNICALMACHINE_EXPLICIT_INSTANTIATION(Generation::one);
TECHNICALMACHINE_EXPLICIT_INSTANTIATION(Generation::two);
TECHNICALMACHINE_EXPLICIT_INSTANTIATION(Generation::three);
TECHNICALMACHINE_EXPLICIT_INSTANTIATION(Generation::four);
TECHNICALMACHINE_EXPLICIT_INSTANTIATION(Generation::five);
TECHNICALMACHINE_EXPLICIT_INSTANTIATION(Generation::six);
TECHNICALMACHINE_EXPLICIT_INSTANTIATION(Generation::seven);
TECHNICALMACHINE_EXPLICIT_INSTANTIATION(Generation::eight);

}	// namespace technicalmachine
