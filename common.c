#include <termbox2.h>

#include "common.h"

const color_t FG_MEMORY = BRIGHT(TB_BLACK);
const color_t BG_MEMORY = TB_DEFAULT;

const struct tile_type TILE_TYPES[] = {
    {
        .ch = '.',
        .fg = TB_DEFAULT,
        .bg = TB_DEFAULT,
        .walkable = true,
        .transparent = true,
        .name = "floor",
    },
    {
        .ch = '#',
        .fg = TB_DEFAULT,
        .bg = TB_DEFAULT,
        .walkable = false,
        .transparent = false,
        .name = "wall",
    },
};

const struct item_type ITEM_TYPES[] = {
    {
        .ch = '*',
        .fg = DARK(TB_YELLOW),
        .bg = TB_DEFAULT,
        .name = "gold",
    },
};

const struct entity_type ENTITY_TYPES[] = {
    {
        .ch = '@',
        .fg = DARK(TB_RED),
        .bg = TB_DEFAULT,
        .name = "rogue",
    },
    {
        .ch = 'g',
        .fg = DARK(TB_GREEN),
        .bg = TB_DEFAULT,
        .name = "goblin",
    },
};
