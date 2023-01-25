// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.operators;

import bounded;
import containers;

namespace technicalmachine {

export using containers::begin;
export using containers::end;
export using containers::operator+;
export using containers::operator-;
export using bounded::operator+=;
export using bounded::operator-=;
export using bounded::operator++;
export using bounded::operator--;

} // namespace technicalmachine
