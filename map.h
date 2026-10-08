#ifndef GAME_H
#define GAME_H

#include "common.h"

enum {
    MIN_Y = 0,
    MIN_X = 0,
    MAX_Y = 100,
    MAX_X = 100,
};

extern struct tile TILE_MAP[MAX_Y][MAX_X];

bool map_in_bounds(int y, int x);
void map_init(void);
void map_next_turn(void);

#endif
