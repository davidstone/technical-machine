// Class to properly do integer multiplication / division
// Copyright (C) 2012 David Stone
//
// This file is part of Technical Machine.
//
// Technical Machine is free software: you can redistribute it and / or modify
// it under the terms of the GNU Affero General Public License as
// published by the Free Software Foundation, either version 3 of the
// License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.

#ifndef RATIONAL_HPP_
#define RATIONAL_HPP_

namespace technicalmachine {

class Rational {
	public:
		// Intentionally implicit
		explicit Rational(unsigned const n, unsigned const d = 1):
			numerator(n),
			denominator(d) {
		}
		template<typename Integer>
		friend Integer operator*=(Integer & number, Rational const rational) {
			number *= rational.numerator;
			number /= rational.denominator;
			return number;
		}
		template<typename Integer>
		friend Integer operator*(Integer number, Rational const rational) {
			return number *= rational;
		}
		template<typename Integer>
		friend Integer operator*(Rational const rational, Integer number) {
			return rational * number;
		}
	private:
		unsigned numerator;
		unsigned denominator;
};

}	// namespace technicalmachine

#endif	// RATIONAL_HPP_
