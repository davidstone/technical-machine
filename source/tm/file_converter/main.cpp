// Copyright David Stone 2021.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

import tm.clients.load_team_from_file;

import tm.stat.stat_style;

import tm.string_conversions.initial_team;

import tm.files_in_path;
import tm.initial_team;
import tm.open_file;

import bounded;
import containers;
import tv;
import std_module;

namespace {
using namespace bounded::literal;

using namespace technicalmachine;
using namespace std::string_view_literals;

constexpr auto invalid_args_message =
	"Usage is file_converter output_type output_location files...\n"
	"Valid output types are: print, text\n"
	"If output_type is \"print\", there is no \"output_location\"\n"
	"If \"files...\" includes a directory, all files in that directory will be parsed.";

enum class OutputType { print, text };

constexpr auto parse_output_type(std::string_view const str) -> OutputType {
	if (str == "print") {
		return OutputType::print;
	} else if (str == "text") {
		return OutputType::text;
	} else {
		throw std::runtime_error(invalid_args_message);
	}
}

struct AsStringPrinted {
	template<SpecialInputStyle style>
	static auto operator()(InitialTeam<style> const & team, std::filesystem::path const &) -> void {
		std::cout << to_string(team) << '\n';
	}
};

struct AsStringFile {
	explicit AsStringFile(std::filesystem::path base_path):
		m_base_path(std::move(base_path))
	{
	}
	template<SpecialInputStyle style>
	auto operator()(InitialTeam<style> const & team, std::filesystem::path const & trailing_path) const -> void {
		auto path = m_base_path / trailing_path;
		path.replace_extension("txt");
		auto stream = open_text_file_for_writing(path);
		stream << to_string(team) << '\n';
	}

private:
	std::filesystem::path m_base_path;
};

using Outputter = tv::variant<AsStringPrinted, AsStringFile>;
struct ParsedArgs {
	Outputter outputter;
	containers::dynamic_array<std::filesystem::path> paths;
};

auto parse_args(int argc, char const * const * argv) -> ParsedArgs {
	if (argc < 2) {
		throw std::runtime_error(invalid_args_message);
	}
	auto const output_type = parse_output_type(argv[1]);
	if (output_type != OutputType::print and argc < 3) {
		throw std::runtime_error(invalid_args_message);
	}
	auto output_location = [=] {
		auto result = std::filesystem::path(argv[2]);
		if (std::filesystem::exists(result) and !std::filesystem::is_directory(result)) {
			throw std::runtime_error("Output location must be a directory");
		}
		return result;
	};
	auto get_paths = [=](int const initial) {
		return containers::dynamic_array<std::filesystem::path>(containers::subrange(argv + initial, argv + argc));
	};
	switch (output_type) {
		case OutputType::print:
			return ParsedArgs{Outputter(AsStringPrinted()), get_paths(2)};
		case OutputType::text:
			return ParsedArgs{Outputter(AsStringFile(output_location())), get_paths(3)};
	}
}

auto unique_path_component(std::filesystem::path const & base_path, std::filesystem::path const & full_path) -> std::filesystem::path {
	auto const & base_str = base_path.string();
	auto const & full_str = full_path.string();
	if (!full_str.starts_with(base_str)) {
		throw std::runtime_error(containers::concatenate<std::string>("Could not extract trailing part of file name with a base of "sv, base_str, " and a filename of "sv, full_str));
	}
	return std::filesystem::path(full_str.substr(base_str.size()));
}

} // namespace

auto main(int argc, char ** argv) -> int {
	auto const args = parse_args(argc, argv);
	auto error_count = bounded::integer<0, bounded::builtin_max_value<std::uint64_t>>(0_bi);
	for (auto const & source : args.paths) {
		for (auto const & path : files_in_path(source)) {
			try {
				auto const visitor = [&](auto const & team, auto const & function) {
					function(team, unique_path_component(source, path));
				};
				tv::visit(load_team_from_file(path), args.outputter, visitor);
			} catch (std::exception const & ex) {
				std::cerr << ex.what() << '\n';
				++error_count;
			}
		}
	}
	if (error_count != 0_bi) {
		std::cerr << "Completed with " << error_count << " errors\n";
	}
	return error_count == 0_bi ? 0 : 1;
}
