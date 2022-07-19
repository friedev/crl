#ifndef GAME_H
#define GAME_H

#include "common.h"

#define MIN_Y 0
#define MIN_X 0
#define MAX_Y 100
#define MAX_X 100

extern struct tile TILE_MAP[MAX_Y][MAX_X];

bool map_in_bounds(int y, int x);
void map_init();
void map_next_turn();

#endif
