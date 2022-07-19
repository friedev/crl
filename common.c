#include "common.h"

const struct tile_type TILE_TYPES[] = {
	{
		.symbol = '.',
		.walkable = true,
		.transparent = true,
		.pair = PAIR_DEFAULT,
		.name = "floor",
	},
	{
		.symbol = '#',
		.walkable = false,
		.transparent = false,
		.pair = PAIR_DEFAULT,
		.name = "wall",
	},
};

const struct item_type ITEM_TYPES[] = {
	{
		.symbol = '*',
		.pair = PAIR_YELLOW,
		.name = "gold",
	}
};

const struct entity_type ENTITY_TYPES[] = {
	{
		.symbol = '@',
		.pair = PAIR_RED,
		.name = "rogue",
	},
	{
		.symbol = 'g',
		.pair = PAIR_GREEN,
		.name = "goblin",
	},
};
