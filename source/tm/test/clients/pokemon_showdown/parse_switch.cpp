// Copyright David Stone 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.test.clients.ps.parse_switch;

import tm.clients.ps.in_message;
import tm.clients.ps.parse_switch;
import tm.clients.ps.switch_message;

import tm.clients.party;

import tm.pokemon.level;
import tm.pokemon.nickname;
import tm.pokemon.species;

import tm.status.status_name;

import tm.gender;
import tm.visible_hp;

import bounded;
import std_module;

namespace technicalmachine {
namespace {
using namespace bounded::literal;
using namespace std::string_view_literals;

static_assert(
	ps::parse_switch(ps::InMessage(
		"|switch|p2a: Raikou|Raikou, shiny|100/100"sv
	)) ==
	ps::SwitchMessage(
		Party(1_bi),
		Species::Raikou,
		"Raikou"sv,
		Level(100_bi),
		Gender::genderless,
		VisibleHP(CurrentVisibleHP(100_bi), MaxVisibleHP(100_bi)),
		StatusName::clear
	)
);

} // namespace
} // namespace technicalmachine
