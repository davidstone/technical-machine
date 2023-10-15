// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.stat.stat_style;

import tm.generation;

namespace technicalmachine {

// Split special for base stat, DV / IV, and EV
export enum class StatStyle { gen1, gen2, current };

// Split special for DV / IV and EV
export struct SpecialStyle {
private:
	enum class Style { combined, split };
	static constexpr auto convert(StatStyle const style) -> Style {
		switch (style) {
			case StatStyle::gen1:
			case StatStyle::gen2:
				return Style::combined;
			case StatStyle::current:
				return Style::split;
		}
	}

	constexpr SpecialStyle(Style const style):
		m_style(style)
	{
	}
public:
	constexpr SpecialStyle(StatStyle const style):
		m_style(convert(style))
	{
	}

	friend constexpr auto operator==(SpecialStyle, SpecialStyle) -> bool = default;

	static SpecialStyle const combined;
	static SpecialStyle const split;

	// Treat as private
	Style m_style;
};
constexpr SpecialStyle SpecialStyle::combined = SpecialStyle(Style::combined);
constexpr SpecialStyle SpecialStyle::split = SpecialStyle(Style::split);

export constexpr auto stat_style_for(Generation const generation) {
	switch (generation) {
		case Generation::one:
			return StatStyle::gen1;
		case Generation::two:
			return StatStyle::gen2;
		case Generation::three:
		case Generation::four:
		case Generation::five:
		case Generation::six:
		case Generation::seven:
		case Generation::eight:
			return StatStyle::current;
	}
}

export constexpr auto special_style_for(Generation const generation) {
	switch (generation) {
		case Generation::one:
		case Generation::two:
			return SpecialStyle::combined;
		case Generation::three:
		case Generation::four:
		case Generation::five:
		case Generation::six:
		case Generation::seven:
		case Generation::eight:
			return SpecialStyle::split;
	}
}

} // namespace technicalmachine