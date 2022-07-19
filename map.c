#include <stdio.h>
#include <stdlib.h>

#include "fov.h"
#include "map.h"
#include "message.h"

struct tile TILE_MAP[MAX_Y][MAX_X];

bool map_in_bounds(int y, int x)
{
	return MIN_Y <= y && y < MAX_Y && MIN_X <= x && x < MAX_X;
}

static void map_clear()
{
	for (coord_t y = 0; y < MAX_Y; y++) {
		for (coord_t x = 0; x < MAX_X; x++) {
			TILE_MAP[y][x] = (struct tile) {
				.type = TILE_FLOOR,
				.visible = false,
				.last_visible_type = TILE_INVALID,
				.entity = NULL,
				.item_head = NULL,
			};
		}
	}
}

static uint8_t map_smooth_tile(coord_t y, coord_t x)
{
	int neighbor_count = 0;
	int floor_count = 0;
	int wall_count = 0;
	for (int dy = -1; dy <= 1; dy++) {
		for (int dx = -1; dx <= 1; dx++) {
			if (dy == 0 && dx == 0) {
				continue;
			}

			if (!map_in_bounds(y + dy, x + dx)) {
				continue;
			}

			coord_t yn = y + dy;
			coord_t xn = x + dx;
			neighbor_count++;
			if (TILE_MAP[yn][xn].type == TILE_FLOOR) {
				floor_count++;
			} else if (TILE_MAP[yn][xn].type == TILE_WALL) {
				wall_count++;
			}
		}
	}

	int threshold = neighbor_count / 2;
	if (floor_count > threshold) {
		return TILE_FLOOR;
	}
	if (wall_count > threshold) {
		return TILE_WALL;
	}
	return TILE_MAP[y][x].type;
}

static void map_smooth()
{
	uint8_t buffer[MAX_Y][MAX_X];
	for (coord_t y = MIN_Y; y < MAX_Y; y++) {
		for (coord_t x = MIN_X; x < MAX_X; x++) {
			buffer[y][x] = map_smooth_tile(y, x);
		}
	}
	for (coord_t y = MIN_Y; y < MAX_Y; y++) {
		for (coord_t x = MIN_X; x < MAX_X; x++) {
			TILE_MAP[y][x].type = buffer[y][x];
		}
	}
}

static void map_randomize()
{
	for (coord_t y = MIN_Y; y < MAX_Y; y++) {
		for (coord_t x = MIN_X; x < MAX_X; x++) {
			TILE_MAP[y][x].type = rand() % 2
				? TILE_FLOOR
				: TILE_WALL;
		}
	}
}

static void map_fill(uint8_t type)
{
	for (coord_t y = MIN_Y; y < MAX_Y; y++) {
		for (coord_t x = MIN_X; x < MAX_X; x++) {
			TILE_MAP[y][x].type = type;
		}
	}
}

static void map_border(uint8_t type)
{
	for (coord_t y = MIN_Y; y < MAX_Y; y++) {
		TILE_MAP[y][MIN_X].type = type;
		TILE_MAP[y][MAX_X - 1].type = type;
	}
	for (coord_t x = MIN_X; x < MAX_X; x++) {
		TILE_MAP[MIN_Y][x].type = type;
		TILE_MAP[MAX_Y - 1][x].type = type;
	}
}

void map_init()
{
	map_clear();
	map_randomize();
	for (int i = 0; i < 3; i++) {
		map_smooth();
	}
	map_border(TILE_WALL);
}
