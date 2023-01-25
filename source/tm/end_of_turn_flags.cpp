// Copyright David Stone 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.end_of_turn_flags;

namespace technicalmachine {

export struct EndOfTurnFlags {
	constexpr EndOfTurnFlags(bool const shed_skin_, bool const lock_in_ends_, bool const thaws_):
		shed_skin(shed_skin_),
		lock_in_ends(lock_in_ends_),
		thaws(thaws_)
	{
	}
	bool shed_skin;
	bool lock_in_ends;
	bool thaws;
};

} // namespace technicalmachine
