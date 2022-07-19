#ifndef COLORS_H
#define COLORS_H

// Color 8 is already defined as gray, at least in Alacritty
#define COLOR_GRAY 8

/*
struct custom_color {
	const short r;
	const short g;
	const short b;
};

enum {
	COLOR_FIRST = 8,
	COLOR_GRAY = COLOR_FIRST,
	COLOR_LAST = COLOR_GRAY,
};

extern const struct custom_color CUSTOM_COLORS[];
*/

struct color_pair {
	const short f;
	const short b;
};

enum {
	PAIR_DEFAULT,
	PAIR_GRAY,
	PAIR_MEMORY = PAIR_GRAY,
	PAIR_RED,
	PAIR_GREEN,
	PAIR_YELLOW,
	PAIR_COUNT,
};

extern const struct color_pair PAIRS[];

#endif
