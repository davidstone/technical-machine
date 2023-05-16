// Copyright David Stone 2021.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

module;

#include <std_module/prelude.hpp>
#include <string_view>
#include <span>

#include <operators/forward.hpp>

export module tm.clients.sb.read_team_file;

import tm.move.max_moves_per_pokemon;
import tm.move.move;
import tm.move.move_name;
import tm.move.pp;
import tm.move.regular_moves;

import tm.pokemon.known_pokemon;
import tm.pokemon.level;
import tm.pokemon.max_pokemon_per_team;
import tm.pokemon.nickname;
import tm.pokemon.species;

import tm.stat.combined_stats;
import tm.stat.ev;
import tm.stat.generic_stats;
import tm.stat.iv;
import tm.stat.nature;
import tm.stat.stat_names;

import tm.string_conversions.ability;
import tm.string_conversions.item;
import tm.string_conversions.move_name;
import tm.string_conversions.species;

import tm.ability;
import tm.buffer_view;
import tm.bytes_in_file;
import tm.gender;
import tm.generation;
import tm.item;
import tm.team;

import bounded;
import containers;
import numeric_traits;
import tv;
import std_module;

// The Shoddy Battle team file was written using Java's Serializable
// https://docs.oracle.com/javase/8/docs/platform/serialization/spec/protocol.html
// This implements the subset of that protocol that appears to be necessary to
// parse team files

namespace technicalmachine::sb {
using namespace bounded::literal;
using namespace std::string_view_literals;

constexpr auto generation = Generation::four;

constexpr auto number_of_stats = containers::size(containers::enum_range<SplitSpecialPermanentStat>());


// This does not use `CHAR_BIT` because the data format is independent of the
// number of bits in a byte on my machine
constexpr auto bits_in_byte = 8_bi;

template<auto bytes>
using ByteInteger = bounded::integer<0, bounded::normalize<(1_bi << (bytes * bits_in_byte)) - 1_bi>>;

struct ByteParser {
	constexpr explicit ByteParser(std::span<std::byte const> view):
		m_view(view)
	{
	}

	constexpr auto pop_byte() {
		return m_view.pop(1)[0];
	}
	constexpr auto pop_integer(auto const bytes) {
		auto result = ByteInteger<bytes>(0_bi);
		for (auto const byte : m_view.pop(static_cast<std::size_t>(bytes))) {
			result *= (1_bi << bits_in_byte);
			result += bounded::integer(byte);
		}
		return result;
	}
	auto pop_string() -> std::string_view {
		auto const size = pop_integer(2_bi);
		auto const str = m_view.pop(static_cast<std::size_t>(size));
		return std::string_view(reinterpret_cast<char const *>(str.data()), static_cast<std::size_t>(size));
	}

	constexpr auto ignore(auto const bytes) -> void {
		m_view.pop(static_cast<std::size_t>(bytes));
	}

private:
	BufferView<std::span<std::byte const>> m_view;
};

constexpr auto null_type = std::byte(0x70);
constexpr auto reference_type = std::byte(0x71);
constexpr auto class_description_type = std::byte(0x72);
constexpr auto object_type = std::byte(0x73);
constexpr auto string_type = std::byte(0x74);
constexpr auto array_type = std::byte(0x75);
constexpr auto class_type = std::byte(0x76);
constexpr auto block_data_type = std::byte(0x77);
// constexpr auto end_block_data_type = std::byte(0x78);
// constexpr auto reset_type = std::byte(0x79);
// constexpr auto block_data_long_type = std::byte(0x7A);
// constexpr auto exception_type = std::byte(0x7B);
// constexpr auto long_string_type = std::byte(0x7C);
constexpr auto proxy_class_description_type = std::byte(0x7D);

constexpr auto write_method = std::byte(0x01);

struct Field {
	enum class Type { object, integer, byte };
	Type type;
	std::string_view name;
};

struct ClassDescription {
	std::string_view name;
	std::byte flags;
	containers::vector<Field> fields;
};

struct ParsedData;

using IntegerVector = containers::static_vector<ByteInteger<4_bi>, bounded::max(number_of_stats, max_moves_per_pokemon)>;
using AnyVector = containers::vector<ParsedData, bounded::max(max_pokemon_per_team, max_moves_per_pokemon).value()>;

struct ParsedData {
	using State = tv::variant<
		IntegerVector,
		AnyVector,
		ClassDescription,
		std::string_view,
		Nature,
		KnownPokemon<generation>,
		std::monostate
	>;

	constexpr explicit ParsedData(bounded::explicitly_convertible_to<State> auto && value):
		state(OPERATORS_FORWARD(value))
	{
	}
	State state;
};

template<typename T>
constexpr auto check_value_type = [](auto const value) -> T {
	return T(bounded::check_in_range<typename T::value_type>(value));
};

constexpr auto to_gender(ByteInteger<4_bi> const id) -> Gender {
	switch (id.value()) {
		case 0: return Gender::genderless;
		case 1: return Gender::male;
		case 2: return Gender::female;
		default: throw std::runtime_error(containers::concatenate<std::string>("Invalid Gender ID: "sv, containers::to_string(id)));
	}
}

constexpr auto to_nature(ByteInteger<4_bi> const id) -> Nature {
	switch (id.value()) {
		case 0: return Nature::Lonely;
		case 1: return Nature::Brave;
		case 2: return Nature::Adamant;
		case 3: return Nature::Naughty;
		case 4: return Nature::Bold;
		case 5: return Nature::Relaxed;
		case 6: return Nature::Impish;
		case 7: return Nature::Lax;
		case 8: return Nature::Timid;
		case 9: return Nature::Hasty;
		case 10: return Nature::Jolly;
		case 11: return Nature::Naive;
		case 12: return Nature::Modest;
		case 13: return Nature::Mild;
		case 14: return Nature::Quiet;
		case 15: return Nature::Rash;
		case 16: return Nature::Calm;
		case 17: return Nature::Gentle;
		case 18: return Nature::Sassy;
		case 19: return Nature::Careful;
		case 20: return Nature::Quirky;
		case 21: return Nature::Hardy;
		case 22: return Nature::Serious;
		case 23: return Nature::Bashful;
		case 24: return Nature::Docile;
		default: throw std::runtime_error(containers::concatenate<std::string>("Invalid Nature ID: "sv, containers::to_string(id)));
	}
}

template<typename Optional>
constexpr auto emplace_once(Optional & optional, auto && initializer) {
	if (optional) {
		throw std::runtime_error("Found data for the same data point twice");
	}
	optional.emplace([&] { return typename Optional::value_type(OPERATORS_FORWARD(initializer)); });
}

template<typename T>
constexpr auto add_stat(tv::optional<GenericStats<T>> & optional, auto const & raw) {
	if (containers::size(raw) != number_of_stats) {
		throw std::runtime_error("Incorrect number of stats");
	}
	auto values = containers::transform(raw, check_value_type<T>);
	emplace_once(optional, GenericStats(values[0_bi], values[1_bi], values[2_bi], values[4_bi], values[5_bi], values[3_bi]));
}

struct Parser {
	constexpr explicit Parser(std::span<std::byte const> const bytes):
		m_byte_parser(bytes)
	{
	}
	auto parse_and_validate_header() & -> void {
		auto const file_magic_bytes = m_byte_parser.pop_integer(2_bi);
		constexpr auto expected_magic_bytes = bounded::constant<0xACED>; // 44269
		if (file_magic_bytes != expected_magic_bytes) {
			throw std::runtime_error("Incorrect magic bytes at start of file");
		}

		auto const file_version = m_byte_parser.pop_integer(2_bi);
		constexpr auto expected_file_version = 5_bi;
		if (file_version != expected_file_version) {
			throw std::runtime_error("Incorrect file version");
		}
	}
	auto parse_any() & -> ParsedData {
		auto const type = m_byte_parser.pop_byte();
		switch (type) {
			case object_type:
				return parse_object();
			case class_type:
				return ParsedData(std::monostate());
			case array_type:
				return parse_array();
			case string_type:
				return parse_string();
			case class_description_type:
				return parse_new_class_description();
			case proxy_class_description_type:
				return ParsedData(std::monostate());
			case reference_type:
				return parse_reference();
			case null_type:
				return ParsedData(std::monostate());
			case block_data_type:
				m_byte_parser.ignore(m_byte_parser.pop_integer(1_bi));
				return ParsedData(std::monostate());
			default:
				throw std::runtime_error(containers::concatenate<std::string>(
					"Unknown data type "sv,
					containers::to_string(bounded::integer(type))
				));
		}
	}

private:
	auto parse_string() & -> ParsedData {
		return containers::push_back(m_objects, ParsedData(m_byte_parser.pop_string()));
	}
	auto parse_array() & -> ParsedData {
		auto const description = parse_class_description();
		auto const size = m_byte_parser.pop_integer(4_bi);
		auto const code = tv::visit(description.state, []<typename T>(T const & value) -> char {
			if constexpr (std::same_as<T, ClassDescription>) {
				return containers::at(value.name, 1_bi);
			} else {
				throw std::runtime_error("Expected class description");
			}
		});
		auto & parsed = containers::push_back(m_objects, ParsedData(std::monostate()));
		switch (code) {
			case 'L':
				if (size > numeric_traits::max_value<AnyVector::size_type>) {
					throw std::runtime_error("Tried to add too many objects");
				}
				parsed = ParsedData(AnyVector(containers::generate_n(size, [&] { return parse_any(); })));
				break;
			case 'I':
				if (size > IntegerVector::capacity()) {
					throw std::runtime_error("Tried to add too many integers");
				}
				parsed = ParsedData(IntegerVector(containers::generate_n(size, [&] {
					return m_byte_parser.pop_integer(4_bi);
				})));
				break;
			default:
				throw std::runtime_error(containers::concatenate<std::string>("Unknown array code "sv, containers::array{code}));
		}
		return parsed;
	}

	auto parse_pokemon(ClassDescription const & description) & -> ParsedData {
		auto gender = tv::optional<Gender>();
		auto level = tv::optional<Level>();
		auto shiny = tv::optional<bool>();
		auto nature = tv::optional<Nature>();
		auto species = tv::optional<Species>();
		auto ability = tv::optional<Ability>();
		auto item = tv::optional<Item>();
		auto nickname = tv::optional<std::string_view>();
		auto ivs = tv::optional<IVs>();
		auto evs = tv::optional<EVs>();
		auto moves = tv::optional<containers::static_vector<MoveName, max_moves_per_pokemon>>();
		// TODO: Use an array?
		auto pp_ups = tv::optional<containers::static_vector<PP::pp_ups_type, max_moves_per_pokemon>>();
		for (auto const field : description.fields) {
			switch (field.type) {
				case Field::Type::object: {
					auto const parsed = parse_any();
					tv::visit(parsed.state, [&]<typename State>(State const & state) {
						if constexpr (std::same_as<State, std::string_view>) {
							if (field.name == "m_name") {
								emplace_once(species, from_string<Species>(state));
							} else if (field.name == "m_abilityName") {
								emplace_once(ability, from_string<Ability>(state));
							} else if (field.name == "m_itemName") {
								emplace_once(item, from_string<Item>(state));
							} else if (field.name == "m_nickname") {
								emplace_once(nickname, state);
							} else {
								throw std::runtime_error(containers::concatenate<std::string>("Found unexpected field name for string data: "sv, field.name));
							}
						} else if constexpr (std::same_as<State, IntegerVector>) {
							if (field.name == "m_iv") {
								add_stat(ivs, state);
							} else if (field.name == "m_ev") {
								add_stat(evs, state);
							} else if (field.name == "m_ppUp") {
								if (containers::size(state) != max_moves_per_pokemon) {
									throw std::runtime_error("Expected exactly 4 PP ups");
								}
								emplace_once(pp_ups, containers::transform(state, [](ByteInteger<4_bi> const x) {
									return bounded::check_in_range<PP::pp_ups_type>(x);
								}));
							} else {
								throw std::runtime_error(containers::concatenate<std::string>("Unknown array type"sv, field.name));
							}
						} else if constexpr (std::same_as<State, AnyVector>) {
							auto parse_moves = [](ParsedData const & inner_parsed) {
								return from_string<MoveName>(inner_parsed.state[bounded::type<std::string_view>]);
							};
							auto filter_moves = [](ParsedData const & inner_parsed) {
								if (inner_parsed.state.index() == bounded::type<std::string_view>) {
									return true;
								} else if (inner_parsed.state.index() == bounded::type<std::monostate>) {
									return false;
								} else {
									throw std::runtime_error(containers::concatenate<std::string>("Bad array type "sv, containers::to_string(inner_parsed.state.index().integer())));
								}
							};
							emplace_once(moves, containers::transform(containers::filter(state, filter_moves), parse_moves));
						} else if constexpr (std::same_as<State, Nature>) {
							if (field.name == "m_nature") {
								emplace_once(nature, state);
							} else {
								throw std::runtime_error("Found a Nature in the wrong place");
							}
						} else if constexpr (std::same_as<State, std::monostate>) {

						} else {
							throw std::runtime_error("Unhandled type");
						}
					});
					break;
				}
				case Field::Type::integer: {
					auto const value = m_byte_parser.pop_integer(4_bi);
					if (field.name == "m_gender") {
						emplace_once(gender, to_gender(value));
					} else if (field.name == "m_level") {
						emplace_once(level, check_value_type<Level>(value));
					}
					break;
				}
				case Field::Type::byte: {
					auto const value = m_byte_parser.pop_integer(1_bi);
					if (field.name == "m_shiny") {
						emplace_once(shiny, value != 0_bi);
					}
					break;
				}
			}
		}
		if (!gender) {
			throw std::runtime_error("Did not receive gender");
		}
		if (!level) {
			throw std::runtime_error("Did not receive level");
		}
		if (!shiny) {
			throw std::runtime_error("Did not receive shiny");
		}
		if (!nature) {
			throw std::runtime_error("Did not receive nature");
		}
		if (!species) {
			throw std::runtime_error("Did not receive species");
		}
		if (!ability) {
			throw std::runtime_error("Did not receive ability");
		}
		if (!item) {
			item = Item::None;
		}
		if (!nickname) {
			throw std::runtime_error("Did not receive nickname");
		}
		if (!ivs) {
			throw std::runtime_error("Did not receive ivs");
		}
		if (!evs) {
			throw std::runtime_error("Did not receive evs");
		}
		if (!moves) {
			throw std::runtime_error("Did not receive moves");
		}
		if (!pp_ups) {
			throw std::runtime_error("Did not receive pp_ups");
		}
		return ParsedData(KnownPokemon<generation>(
			*species,
			Nickname(*nickname),
			*level,
			*gender,
			*item,
			*ability,
			CombinedStats<generation>{
				*nature,
				*ivs,
				*evs
			},
			RegularMoves(containers::transform(containers::integer_range(containers::size(*moves)), [=](auto const index) {
				return Move(generation, (*moves)[index], (*pp_ups)[index]);
			}))
		));
	}

	auto parse_move(ClassDescription const description) & -> ParsedData {
		auto const move = parse_any();
		if ((description.flags & write_method) != std::byte(0x00)) {
			m_byte_parser.ignore(1_bi);
		}
		return move;
	}

	auto parse_class_description() & -> ParsedData {
		auto const type = m_byte_parser.pop_byte();
		switch (type) {
			case class_description_type:
				return parse_new_class_description();
			case proxy_class_description_type:
			case null_type:
				return ParsedData(std::monostate());
			case reference_type:
				return parse_reference();
			default:
				throw std::runtime_error("Invalid class description type");
		}
	}

	auto parse_new_class_description() & -> ParsedData {
		auto & result = containers::push_back(m_objects, ParsedData(std::monostate()));
		auto const name = m_byte_parser.pop_string();
		[[maybe_unused]] auto const uid = m_byte_parser.pop_integer(8_bi);
		auto const flags = m_byte_parser.pop_byte();
		auto const count = m_byte_parser.pop_integer(2_bi);
		auto fields = containers::vector<Field>(containers::generate_n(count, [&] {
			return Field(parse_field());
		}));
		m_byte_parser.ignore(1_bi);
		auto base_class_fields = tv::visit(parse_class_description().state, []<typename T>(T base) {
			if constexpr (std::same_as<T, ClassDescription>) {
				return std::move(base).fields;
			} else {
				return containers::vector<Field>();
			}
		});
		if ((flags & write_method) != std::byte(0x00)) {
			containers::push_back(fields, Field{Field::Type::byte, ""});
		}
		result = ParsedData(ClassDescription{
			name,
			flags,
			containers::concatenate<containers::vector<Field>>(base_class_fields, std::move(fields))
		});
		return result;
	}

	auto parse_field() & -> Field {
		auto const type_code = char(m_byte_parser.pop_byte());
		auto const name = m_byte_parser.pop_string();
		switch (type_code) {
			case '[':
			case 'L': {
				[[maybe_unused]] auto const parsed = parse_any();
				return Field{Field::Type::object, name};
			}
			case 'I':
				return Field{Field::Type::integer, name};
			default:
				return Field{Field::Type::byte, name};
		}
	}

	auto parse_reference() & -> ParsedData {
		m_byte_parser.ignore(2_bi);
		auto const index = m_byte_parser.pop_integer(2_bi);
		return containers::at(m_objects, index);
	}

	auto parse_object() & -> ParsedData {
		auto const description = tv::visit(parse_class_description().state, []<typename T>(T && value) -> ClassDescription {
			if constexpr (std::same_as<T, ClassDescription>) {
				return value;
			} else {
				throw std::runtime_error("Expected class description");
			}
		});

		auto & result = containers::push_back(m_objects, ParsedData(std::monostate()));
		if (description.name == "shoddybattle.Pokemon") {
			result = parse_pokemon(description);
		} else if (description.name == "mechanics.AdvanceMechanics" or description.name == "mechanics.JewelMechanics") {
			result = parse_any();
		} else if (description.name == "java.util.Random") {
			m_byte_parser.ignore(18_bi);
		} else if (description.name == "mechanics.moves.MoveListEntry") {
			result = parse_move(description);
		} else if (description.name == "mechanics.PokemonNature") {
			auto const id = m_byte_parser.pop_integer(4_bi);
			result = ParsedData(to_nature(id));
		} else {
			throw std::runtime_error(containers::concatenate<std::string>("Unknown string "sv, description.name));
		}
		return result;
	}

	ByteParser m_byte_parser;
	containers::stable_vector<ParsedData, 1000> m_objects;
};

export auto read_team_file(std::filesystem::path const & team_file) -> GenerationGeneric<KnownTeam> {
	try {
		auto const bytes = bytes_in_file(team_file);
		auto parser = Parser(bytes);
		parser.parse_and_validate_header();

		// String or Null
		[[maybe_unused]] auto const uuid = parser.parse_any();
		auto const parsed_all_pokemon = parser.parse_any();
		constexpr auto array_index = bounded::type<AnyVector>;
		if (parsed_all_pokemon.state.index() != array_index) {
			throw std::runtime_error("Expected team to be an array");
		}
		auto const & all_pokemon = parsed_all_pokemon.state[array_index];
		auto transformed = containers::transform(all_pokemon, [](ParsedData const & pokemon) {
			constexpr auto pokemon_index = bounded::type_t<KnownPokemon<generation>>();
			if (pokemon.state.index() != pokemon_index) {
				throw std::runtime_error("Expected team to be an array of Pokemon");
			}
			return pokemon.state[pokemon_index];
		});
		return GenerationGeneric<KnownTeam>(KnownTeam<generation>(transformed));
	} catch (std::exception const & ex) {
		throw std::runtime_error(containers::concatenate<std::string>("Failed to parse Shoddy Battle team file \""sv, team_file.string(), "\" -- "sv, std::string_view(ex.what())));
	}
}

} // namespace technicalmachine::sb