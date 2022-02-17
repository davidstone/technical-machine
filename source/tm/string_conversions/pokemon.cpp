// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <tm/string_conversions/pokemon.hpp>

#include <tm/pokemon/known_pokemon.hpp>
#include <tm/pokemon/pokemon.hpp>
#include <tm/pokemon/seen_pokemon.hpp>

#include <tm/string_conversions/ability.hpp>
#include <tm/string_conversions/item.hpp>
#include <tm/string_conversions/move.hpp>
#include <tm/string_conversions/nature.hpp>
#include <tm/string_conversions/species.hpp>
#include <tm/string_conversions/status.hpp>

#include <tm/stat/calculate_ivs_and_evs.hpp>
#include <tm/stat/ev.hpp>
#include <tm/stat/iv.hpp>

#include <tm/buffer_view.hpp>

#include <bounded/to_integer.hpp>

#include <containers/algorithms/all_any_none.hpp>
#include <containers/algorithms/concatenate.hpp>
#include <containers/algorithms/find.hpp>
#include <containers/array.hpp>
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
constexpr auto spe_iv_hp_ev = " Spe\n\tEVs: "sv;
constexpr auto hp_ev_atk_ev = " HP / "sv;
constexpr auto atk_ev_def_ev = " Atk / "sv;
constexpr auto def_ev_spa_ev = " Def / "sv;
constexpr auto spa_ev_spd_ev = " SpA / "sv;
constexpr auto spd_ev_spe_ev = " SpD / "sv;
constexpr auto spe_ev_moves = " Spe"sv;

constexpr auto status_atk_dv = "\n\tDVs: "sv;
constexpr auto atk_dv_def_dv = " Atk / "sv;
constexpr auto def_dv_spe_dv = " Def / "sv;
constexpr auto spe_dv_spc_dv = " Spe / "sv;
constexpr auto spc_dv_old_hp_ev = " Spc\n\tEVs: "sv;
constexpr auto old_hp_ev_atk_ev = " HP / "sv;
constexpr auto old_atk_ev_def_ev = " Atk / "sv;
constexpr auto old_def_ev_spe_ev = " Def / "sv;
constexpr auto old_spe_ev_spc_ev = " Spe / "sv;
constexpr auto old_spc_ev_moves = " Spc"sv;

constexpr auto moves_separator = "\n\t- "sv;

} // namespace

// TODO: Print nickname
// TODO: Print level
// TODO: Print gender
// TODO: Make this compatible with Pokemon Showdown

template<any_pokemon PokemonType>
auto to_string(PokemonType const & pokemon) -> containers::string {
	constexpr auto generation = generation_from<PokemonType>;
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

	auto stat_to_iv_string = [&](auto const stat_name) {
		return bounded::to_string(stats.dvs_or_ivs[stat_name].value());
	};

	auto stat_to_ev_string = [&](auto const stat_name) {
		return bounded::to_string(stats.evs[stat_name].value());
	};

	auto stats_to_string = [&] {
		if constexpr (generation <= Generation::two) {
			return containers::concatenate<containers::string>(
				stat_to_iv_string(SpecialRegularStat::atk),
				atk_dv_def_dv,
				stat_to_iv_string(SpecialRegularStat::def),
				def_dv_spe_dv,
				stat_to_iv_string(SpecialRegularStat::spe),
				spe_dv_spc_dv,
				stat_to_iv_string(SpecialRegularStat::spc),
				spc_dv_old_hp_ev,
				stat_to_ev_string(SpecialPermanentStat::hp),
				old_hp_ev_atk_ev,
				stat_to_ev_string(SpecialPermanentStat::atk),
				old_atk_ev_def_ev,
				stat_to_ev_string(SpecialPermanentStat::def),
				old_def_ev_spe_ev,
				stat_to_ev_string(SpecialPermanentStat::spe),
				old_spe_ev_spc_ev,
				stat_to_ev_string(SpecialPermanentStat::spc),
				old_spc_ev_moves
			);
		} else {
			return containers::concatenate<containers::string>(
				to_string(pokemon.nature()),
				nature_hp_iv,
				stat_to_iv_string(SplitSpecialPermanentStat::hp),
				hp_iv_atk_iv,
				stat_to_iv_string(SplitSpecialPermanentStat::atk),
				atk_iv_def_iv,
				stat_to_iv_string(SplitSpecialPermanentStat::def),
				def_iv_spa_iv,
				stat_to_iv_string(SplitSpecialPermanentStat::spa),
				spa_iv_spd_iv,
				stat_to_iv_string(SplitSpecialPermanentStat::spd),
				spd_iv_spe_iv,
				stat_to_iv_string(SplitSpecialPermanentStat::spe),
				spe_iv_hp_ev,
				stat_to_ev_string(SplitSpecialPermanentStat::hp),
				hp_ev_atk_ev,
				stat_to_ev_string(SplitSpecialPermanentStat::atk),
				atk_ev_def_ev,
				stat_to_ev_string(SplitSpecialPermanentStat::def),
				def_ev_spa_ev,
				stat_to_ev_string(SplitSpecialPermanentStat::spa),
				spa_ev_spd_ev,
				stat_to_ev_string(SplitSpecialPermanentStat::spd),
				spd_ev_spe_ev,
				stat_to_ev_string(SplitSpecialPermanentStat::spe),
				spe_ev_moves
			);
		}
	};

	auto moves_to_string = [&] {
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
		generation <= Generation::two ? status_atk_dv : status_nature,
		stats_to_string(),
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

auto pop_ability_and_status(BufferView<std::string_view> & buffer, Generation const generation) {
	auto const ability_and_status_str = pop_to_delimiter(buffer, generation <= Generation::two ? status_atk_dv : status_nature);
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

template<Generation generation>
constexpr auto pop_dvs_or_ivs(BufferView<std::string_view> & buffer) {
	if constexpr (generation <= Generation::two) {
		auto const atk = pop_value_type<DV>(buffer, atk_dv_def_dv);
		auto const def = pop_value_type<DV>(buffer, def_dv_spe_dv);
		auto const spe = pop_value_type<DV>(buffer, spe_dv_spc_dv);
		auto const spc = pop_value_type<DV>(buffer, spc_dv_old_hp_ev);
		return DVs(atk, def, spe, spc);
	} else {
		auto const hp = pop_value_type<IV>(buffer, hp_iv_atk_iv);
		auto const atk = pop_value_type<IV>(buffer, atk_iv_def_iv);
		auto const def = pop_value_type<IV>(buffer, def_iv_spa_iv);
		auto const spa = pop_value_type<IV>(buffer, spa_iv_spd_iv);
		auto const spd = pop_value_type<IV>(buffer, spd_iv_spe_iv);
		auto const spe = pop_value_type<IV>(buffer, spe_iv_hp_ev);
		return IVs(hp, atk, def, spa, spd, spe);
	}
}

template<Generation generation>
constexpr auto pop_evs(BufferView<std::string_view> & buffer) {
	if constexpr (generation <= Generation::two) {
		auto const hp = pop_value_type<EV>(buffer, old_hp_ev_atk_ev);
		auto const atk = pop_value_type<EV>(buffer, old_atk_ev_def_ev);
		auto const def = pop_value_type<EV>(buffer, old_def_ev_spe_ev);
		auto const spe = pop_value_type<EV>(buffer, old_spe_ev_spc_ev);
		auto const spc = pop_value_type<EV>(buffer, old_spc_ev_moves);
		return OldGenEVs(hp, atk, def, spe, spc);
	} else {
		auto const hp = pop_value_type<EV>(buffer, hp_ev_atk_ev);
		auto const atk = pop_value_type<EV>(buffer, atk_ev_def_ev);
		auto const def = pop_value_type<EV>(buffer, def_ev_spa_ev);
		auto const spa = pop_value_type<EV>(buffer, spa_ev_spd_ev);
		auto const spd = pop_value_type<EV>(buffer, spd_ev_spe_ev);
		auto const spe = pop_value_type<EV>(buffer, spe_ev_moves);
		return EVs(hp, atk, def, spa, spd, spe);
	}
}

} // namespace

template<Generation generation>
auto pokemon_from_string(std::string_view const str) -> Pokemon<generation> {
	auto buffer = BufferView(str);
	auto const species = typed_pop<Species>(buffer, species_hp);
	auto const hp_percent = typed_pop<double>(buffer, hp_item);
	auto const item = typed_pop<Item>(buffer, item_ability);
	auto const [ability, status] = pop_ability_and_status(buffer, generation);
	auto const nature = generation <= Generation::two ? Nature::Hardy : typed_pop<Nature>(buffer, nature_hp_iv);

	auto const dvs_or_ivs = pop_dvs_or_ivs<generation>(buffer);
	auto const evs = pop_evs<generation>(buffer);

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
		CombinedStats<generation>{nature, dvs_or_ivs, evs},
		moves
	);
	pokemon.set_hp(bounded::assume_in_range<HP::current_type>(static_cast<int>(static_cast<double>(pokemon.hp().max()) * hp_percent / 100.0)));
	pokemon.set_status(status);

	return pokemon;
}

#define TECHNICALMACHINE_EXPLICIT_INSTANTIATION_IMPL(PokemonType) \
	template auto to_string(PokemonType const & pokemon) -> containers::string

#define TECHNICALMACHINE_EXPLICIT_INSTANTIATION(generation) \
	TECHNICALMACHINE_EXPLICIT_INSTANTIATION_IMPL(Pokemon<generation>); \
	TECHNICALMACHINE_EXPLICIT_INSTANTIATION_IMPL(KnownPokemon<generation>); \
	TECHNICALMACHINE_EXPLICIT_INSTANTIATION_IMPL(SeenPokemon<generation>); \
	template auto pokemon_from_string<generation>(std::string_view const str) -> Pokemon<generation>

TECHNICALMACHINE_EXPLICIT_INSTANTIATION(Generation::one);
TECHNICALMACHINE_EXPLICIT_INSTANTIATION(Generation::two);
TECHNICALMACHINE_EXPLICIT_INSTANTIATION(Generation::three);
TECHNICALMACHINE_EXPLICIT_INSTANTIATION(Generation::four);
TECHNICALMACHINE_EXPLICIT_INSTANTIATION(Generation::five);
TECHNICALMACHINE_EXPLICIT_INSTANTIATION(Generation::six);
TECHNICALMACHINE_EXPLICIT_INSTANTIATION(Generation::seven);
TECHNICALMACHINE_EXPLICIT_INSTANTIATION(Generation::eight);

} // namespace technicalmachine
