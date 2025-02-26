// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

module tm.clients.pl.read_team_file;

import tm.move.initial_move;
import tm.move.move_name;
import tm.move.pp;

import tm.pokemon.happiness;
import tm.pokemon.initial_pokemon;
import tm.pokemon.level;
import tm.pokemon.max_pokemon_per_team;
import tm.pokemon.nickname;
import tm.pokemon.species;

import tm.stat.combined_stats;
import tm.stat.ev;
import tm.stat.evs;
import tm.stat.iv;
import tm.stat.iv_and_ev;
import tm.stat.nature;
import tm.stat.stat_style;

import tm.string_conversions.ability;
import tm.string_conversions.gender;
import tm.string_conversions.item;
import tm.string_conversions.move_name;
import tm.string_conversions.nature;
import tm.string_conversions.species;

import tm.ability;
import tm.gender;
import tm.generation;
import tm.initial_team;
import tm.item;
import tm.property_tree;

import bounded;
import containers;
import tv;
import std_module;

namespace technicalmachine::pl {
using namespace bounded::literal;
using namespace std::string_view_literals;

constexpr auto parse_species(std::string_view const str) -> Species {
	static constexpr auto converter = containers::basic_flat_map(
		containers::assume_sorted_unique,
		containers::to_array<containers::map_value_type<std::string_view, Species>>({
			{ "Deoxys", Species::Deoxys_Normal },
			{ "Deoxys-e", Species::Deoxys_Speed },
			{ "Deoxys-f", Species::Deoxys_Attack },
			{ "Deoxys-l", Species::Deoxys_Defense },
			{ "Giratina", Species::Giratina_Altered },
			{ "Giratina-o", Species::Giratina_Origin },
			{ "Rotom-c", Species::Rotom_Mow },
			{ "Rotom-f", Species::Rotom_Frost },
			{ "Rotom-h", Species::Rotom_Heat },
			{ "Rotom-s", Species::Rotom_Fan },
			{ "Rotom-w", Species::Rotom_Wash },
			{ "Shaymin", Species::Shaymin_Land },
			{ "Shaymin-s", Species::Shaymin_Sky },
			{ "Wormadam", Species::Wormadam_Plant },
			{ "Wormadam-g", Species::Wormadam_Sandy },
			{ "Wormadam-s", Species::Wormadam_Trash }
		})
	);
	auto const result = containers::lookup(converter, str);
	return result ? *result : from_string<Species>(str);
}

auto parse_moves(property_tree::ptree_reader pt) -> InitialMoves {
	return InitialMoves(containers::transform(
		pt,
		[=](property_tree::ptree_reader::value_type const & value) {
			auto const & mapped = value.second;
			auto const name = from_string<MoveName>(mapped.get_value<std::string>());
			auto const pp_ups = mapped.get<PP::pp_ups_type>("<xmlattr>.pp-up");
			return InitialMove(name, pp_ups);
		}
	));
}

template<SpecialInputStyle stat_style>
auto parse_stats(property_tree::ptree_reader pt) -> CombinedStats<stat_style> {
	constexpr auto is_combined = stat_style == SpecialInputStyle::combined;
	using StatType = std::conditional_t<is_combined, DVAndEV, IVAndEV>;
	auto hp = tv::optional<StatType>();
	auto atk = tv::optional<StatType>();
	auto def = tv::optional<StatType>();
	auto spa = tv::optional<StatType>();
	auto spd = tv::optional<StatType>();
	auto spe = tv::optional<StatType>();

	for (auto const & value : pt.get_child("stats")) {
		auto const & stats = value.second;
		auto const & stat_name = stats.get<std::string>("<xmlattr>.name");
		using IVType = std::conditional_t<is_combined, DV, IV>;
		auto const iv_and_ev = StatType{
			IVType(stats.get<typename IVType::value_type>("<xmlattr>.iv")),
			EV(stats.get<EV::value_type>("<xmlattr>.ev"))
		};
		if (stat_name == "HP") {
			hp = iv_and_ev;
		} else if (stat_name == "Atk") {
			atk = iv_and_ev;
		} else if (stat_name == "Def") {
			def = iv_and_ev;
		} else if (stat_name == "SpAtk") {
			spa = iv_and_ev;
		} else if (stat_name == "SpDef") {
			spd = iv_and_ev;
		} else if (stat_name == "Spd") {
			spe = iv_and_ev;
		} else {
			throw std::runtime_error(containers::concatenate<std::string>(std::string_view("Invalid stat name "), stat_name));
		}
	}
	if (!hp or !atk or !def or !spa or !spd or !spe) {
		throw std::runtime_error("IV or EV is missing");
	}

	if constexpr (is_combined) {
		// I'm not sure if Pokemon Lab ever supported older generations, but if
		// it did this is probably what the file would look like?
		if (*spa != *spd) {
			throw std::runtime_error("Mismatched Special DV / EV");
		}
		auto const dvs = DVs(atk->dv, def->dv, spe->dv, spa->dv);
		if (get_hp(dvs) != hp->dv) {
			throw std::runtime_error("Invalid HP DV");
		}
		return CombinedStats<stat_style>{
			Nature::Hardy,
			dvs,
			OldGenEVs(hp->ev, atk->ev, def->ev, spe->ev, spa->ev)
		};
	} else {
		return CombinedStats<stat_style>{
			from_string<Nature>(pt.get<std::string>("nature")),
			IVs(hp->iv, atk->iv, def->iv, spa->iv, spd->iv, spe->iv),
			EVs(hp->ev, atk->ev, def->ev, spa->ev, spd->ev, spe->ev)
		};
	}
}

template<SpecialInputStyle style>
auto parse_pokemon(property_tree::ptree_reader pt) -> InitialPokemon<style> {
	auto const species_str = pt.get<std::string>("<xmlattr>.species");
	auto const given_nickname = pt.get<std::string>("nickname");
	return InitialPokemon<style>(
		parse_species(species_str),
		Nickname(containers::is_empty(given_nickname) ? species_str : given_nickname),
		Level(pt.get<Level::value_type>("level")),
		Gender(from_string<Gender>(pt.get<std::string>("gender"))),
		from_string<Item>(pt.get<std::string>("item")),
		Ability(from_string<Ability>(pt.get<std::string>("ability"))),
		parse_stats<style>(pt),
		parse_moves(pt.get_child("moveset")),
		Happiness(pt.get<Happiness::value_type>("happiness"))
	);
}

template<SpecialInputStyle style>
auto parse_team(property_tree::ptree_reader ptree) -> InitialTeam<style> {
	return InitialTeam<style>(
		containers::transform(
			containers::filter(
				ptree,
				[](auto const & value) { return value.first == "pokemon"; }
			),
			[](auto const & value) { return parse_pokemon<style>(value.second); }
		)
	);
}

auto read_team_file(std::span<std::byte const> const bytes) -> AnyInitialTeam {
	auto owner = property_tree::ptree();
	auto pt = owner.read_xml(bytes);

	auto const all_pokemon = pt.get_child("shoddybattle");
	using GenerationInteger = decltype(bounded::integer(Generation()));
	// The original format did not include a generation. Allow users to add
	// this field.
	auto const generation = static_cast<Generation>(all_pokemon.get("<xmlattr>.generation", GenerationInteger(4_bi)));
	switch (special_input_style_for(generation)) {
		case SpecialInputStyle::combined:
			return parse_team<SpecialInputStyle::combined>(all_pokemon);
		case SpecialInputStyle::split:
			return parse_team<SpecialInputStyle::split>(all_pokemon);
	}
}

} // namespace technicalmachine::pl