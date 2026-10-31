export module tm.operators;

import bounded;
import containers;
import operators;

namespace technicalmachine {

export using containers::begin;
export using containers::end;
export using containers::operator+;
export using containers::operator-;
export using operators::operator+=;
export using operators::operator-=;
export using bounded::operator++;
export using bounded::operator--;

} // namespace technicalmachine
