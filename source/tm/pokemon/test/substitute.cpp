// Copyright David Stone 2024.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.pokemon.test.substitute;

import tm.pokemon.substitute;

import tm.generation;

import bounded;
import tv;

namespace technicalmachine {
namespace {
using namespace bounded::literal;

static_assert(!Substitute());
static_assert(Substitute().hp() == 0_bi);
static_assert([] {
	auto substitute = Substitute();
	auto const hp_lost = substitute.create(Generation::one, 1_bi, 400_bi);
	return !hp_lost and !substitute;
}());

static_assert([] {
	auto substitute = Substitute();
	auto const hp_lost = substitute.create(Generation::one, 99_bi, 400_bi);
	return !hp_lost and !substitute;
}());

static_assert([] {
	auto substitute = Substitute();
	auto const hp_lost = substitute.create(Generation::one, 100_bi, 400_bi);
	return hp_lost and *hp_lost == 100_bi;
}());

static_assert([] {
	auto substitute = Substitute();
	auto const hp_lost = substitute.create(Generation::one, 101_bi, 400_bi);
	return hp_lost and *hp_lost == 100_bi;
}());

static_assert([] {
	auto substitute = Substitute();
	auto const hp_lost = substitute.create(Generation::two, 100_bi, 400_bi);
	return !hp_lost and !substitute;
}());

static_assert([] {
	auto substitute = Substitute();
	auto const hp_lost = substitute.create(Generation::two, 101_bi, 400_bi);
	return hp_lost and *hp_lost == 100_bi;
}());

static_assert([] {
	auto substitute = Substitute();
	auto const hp_lost = substitute.create(Generation::three, 1_bi, 1_bi);
	return !hp_lost and !substitute;
}());

static_assert([] {
	auto substitute = Substitute();
	substitute.create(Generation::three, 400_bi, 400_bi);
	auto const hp_lost = substitute.create(Generation::three, 300_bi, 400_bi);
	return !hp_lost and substitute;
}());

} // namespace
} // namespace technicalmachine
