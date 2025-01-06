// Copyright David Stone 2024.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.clients.nb.read_team_file;

import tm.clients.nb.file_version;
import tm.clients.nb.game_version_to_generation;
import tm.clients.nb.id_to_ability;
import tm.clients.nb.id_to_gender;
import tm.clients.nb.id_to_item;
import tm.clients.nb.id_to_move;
import tm.clients.nb.id_to_species;

import tm.move.initial_move;
import tm.move.max_moves_per_pokemon;
import tm.move.move_name;

import tm.pokemon.initial_pokemon;
import tm.pokemon.level;
import tm.pokemon.max_pokemon_per_team;
import tm.pokemon.nickname;
import tm.pokemon.species;

import tm.stat.combined_stats;
import tm.stat.ev;
import tm.stat.evs;
import tm.stat.generic_stats;
import tm.stat.ingame_id_to_nature;
import tm.stat.iv;
import tm.stat.nature;
import tm.stat.stat_style;

import tm.ability;
import tm.bit_view;
import tm.initial_team;

import bounded;
import containers;
import tv;
import std_module;

namespace technicalmachine::nb {
using namespace bounded::literal;

using namespace std::string_view_literals;

struct Parser {
	constexpr explicit Parser(std::span<std::byte const> const bytes):
		m_view(bytes),
		m_file_version(pop_header())
	{
	}

	template<typename T>
	constexpr auto pop_integer(auto const bits) {
		if (bits > m_view.remaining_bits()) {
			throw std::runtime_error("Attempted to pop too many bits.");
		}
		return bounded::check_in_range<T>(m_view.pop_integer(bits));
	}
	constexpr auto pop_integer(auto const bits) {
		return pop_integer<bounded::integer<0, bounded::normalize<(1_bi << bits) - 1_bi>>>(bits);
	}
	constexpr auto pop_string(auto const size) {
		if (size * bounded::char_bit > m_view.remaining_bits()) {
			throw std::runtime_error("Attempted to pop too many bytes.");
		}
		return m_view.pop_string(size);
	}
	constexpr auto ignore_sized_string() {
		return m_view.skip_bytes(pop_integer(8_bi));
	}
	constexpr auto skip_bits(auto const bits) -> void {
		m_view.skip_bits(bits);
	}

	constexpr auto file_version() const -> FileVersion {
		return m_file_version;
	}

private:
	constexpr auto pop_header() -> FileVersion {
		auto const file_version_string = pop_string(7_bi);
		if (file_version_string == " PNB4.0") {
			return FileVersion::four_zero;
		} else if (file_version_string == " PNB4.1") {
			return FileVersion::four_one;
		} else if (file_version_string == " PNB5.0") {
			return FileVersion::five_zero;
		} else {
			throw std::runtime_error(containers::concatenate<std::string>("Version \""sv, file_version_string, "\" not supported"sv));
		}
	}
	BitView m_view;
	FileVersion m_file_version;
};

template<SpecialInputStyle style>
constexpr auto parse_nature(Parser & parser) -> Nature {
	// Nature is preserved but ignored in older generations
	auto const id = parser.pop_integer<IngameNatureID>(5_bi);
	switch (style) {
		case SpecialInputStyle::combined:
			return Nature::Hardy;
		case SpecialInputStyle::split:
			return ingame_id_to_nature(id);
	}
}

template<SpecialInputStyle style>
constexpr auto parse_ability(Parser & parser, Species const species) -> Ability {
	// Ability is preserved but ignored in older generations
	auto const id = parser.pop_integer(1_bi);
	switch (style) {
		case SpecialInputStyle::combined:
			return Ability::Honey_Gather;
		case SpecialInputStyle::split:
			return id_to_ability(species, id);
	}
}

constexpr auto parse_moves(Parser & parser) -> InitialMoves {
	auto pop_move = [&] {
		return id_to_move(parser.pop_integer<MoveID>(9_bi));
	};
	return InitialMoves(containers::transform(
		containers::remove_none(
			containers::generate_n(max_moves_per_pokemon, pop_move)
		),
		[](MoveName const move) {
			return InitialMove(move);
		}
	));
}

template<typename T>
constexpr auto parse_ivs_or_evs(Parser & parser, auto const bits) {
	auto pop = [&] {
		return T(parser.pop_integer<typename T::value_type>(bits));
	};
	auto const hp = pop();
	auto const attack = pop();
	auto const defense = pop();
	auto const speed = pop();
	auto const special_attack = pop();
	auto const special_defense = pop();
	return GenericStats(hp, attack, defense, special_attack, special_defense, speed);
}

template<SpecialInputStyle style>
constexpr auto parse_ivs(Parser & parser) {
	constexpr auto bits = 5_bi;
	if constexpr (style == SpecialInputStyle::combined) {
		return to_dvs_using_spa_as_spc(parse_ivs_or_evs<DV>(parser, bits));
	} else {
		return parse_ivs_or_evs<IV>(parser, bits);
	}
}

template<SpecialInputStyle style>
constexpr auto parse_evs(Parser & parser) {
	auto const stored = parse_ivs_or_evs<EV>(parser, 8_bi);
	if constexpr (style == SpecialInputStyle::combined) {
		// NetBattle technically preserves but ignores the EV values populated
		// in old generations. Given that this matters only when moving Pokemon
		// forward to later generations, I do not think that information needs
		// to able to round trip.
		return OldGenEVs{EV(252_bi), EV(252_bi), EV(252_bi), EV(252_bi), EV(252_bi)};
	} else {
		return stored;
	}
}

template<SpecialInputStyle style>
constexpr auto parse_pokemon(Parser & parser) -> tv::optional<InitialPokemon<style>> {
	constexpr auto nickname_bytes = 15_bi;
	auto const padded_nickname = parser.pop_string(nickname_bytes);
	auto const padded_nickname_view = std::string_view(padded_nickname);
	auto const nickname = Nickname(std::string_view(
		padded_nickname_view.begin(),
		containers::find_if_not(containers::reversed(padded_nickname_view), bounded::equal_to(' ')).base()
	));
	constexpr auto species_bits = 9_bi;
	auto const species_id = parser.pop_integer<SpeciesID>(species_bits);
	if (species_id == 0_bi) {
		constexpr auto total_pokemon_bit_length = 35_bi * bounded::char_bit;
		parser.skip_bits(total_pokemon_bit_length - nickname_bytes * bounded::char_bit - species_bits);
		return tv::none;
	}
	auto const species = id_to_species(bounded::increase_min<1>(species_id), parser.file_version());
	[[maybe_unused]] auto const game_version = parser.pop_integer(3_bi);
	auto const level = Level(parser.pop_integer<Level::value_type>(7_bi));
	auto const item_id = parser.pop_integer<ItemID>(7_bi);
	auto const nature = parse_nature<style>(parser);
	auto const ability = parse_ability<style>(parser, species);
	auto const gender = id_to_gender(species, parser.pop_integer(1_bi));

	[[maybe_unused]] auto const shiny = parser.pop_integer(1_bi);
	[[maybe_unused]] auto const box = parser.pop_integer(4_bi);
	[[maybe_unused]] auto const unown_letter = parser.pop_integer(5_bi);

	auto const moves = parse_moves(parser);
	auto const ivs = parse_ivs<style>(parser);
	auto const evs = parse_evs<style>(parser);

	[[maybe_unused]] auto const unused = parser.pop_integer(1_bi);
	[[maybe_unused]] auto const is_generation_4 = parser.pop_integer(1_bi);
	auto const prefer_generation_4_item = parser.pop_integer(1_bi) == 1_bi ? true : false;
	auto const item = id_to_item(item_id, prefer_generation_4_item);

	return InitialPokemon<style>(
		species,
		nickname,
		level,
		gender,
		item,
		ability,
		CombinedStats<style>(nature, ivs, evs),
		moves
	);
}

template<SpecialInputStyle style>
constexpr auto parse_team(Parser & parser) -> InitialTeam<style> {
	return InitialTeam<style>(containers::remove_none(
		containers::generate_n(
			max_pokemon_per_team,
			[&] { return parse_pokemon<style>(parser); }
		)
	));
}

export constexpr auto read_team_file(std::span<std::byte const> const bytes) -> AnyInitialTeam {
	auto parser = Parser(bytes);
	// username
	parser.ignore_sized_string();
	// extra info
	parser.ignore_sized_string();
	// win message
	parser.ignore_sized_string();
	if (parser.file_version() == FileVersion::five_zero) {
		// tie message
		parser.ignore_sized_string();
	}
	// lose message
	parser.ignore_sized_string();
	auto const generation = game_version_to_generation(parser.pop_integer<GameVersion>(8_bi));
	[[maybe_unused]] auto const avatar = parser.pop_integer(8_bi);
	[[maybe_unused]] auto const sprite_type = parser.pop_integer(8_bi);
	switch (special_input_style_for(generation)) {
		case SpecialInputStyle::combined:
			return parse_team<SpecialInputStyle::combined>(parser);
		case SpecialInputStyle::split:
			return parse_team<SpecialInputStyle::split>(parser);
	}
}

} // namespace technicalmachine::nb