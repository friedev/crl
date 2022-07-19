#include <stdbool.h>
#include <stdlib.h>

#include "fov.h"

#include "common.h"
#include "entity.h"
#include "map.h"

// Shadowcasting multipliers
static int MULTIPLIERS[4][8] = {
	{1, 0, 0, -1, -1, 0, 0, 1},
	{0, 1, -1, 0, 0, -1, 1, 0},
	{0, 1, 1, 0, 0, -1, -1, 0},
	{1, 0, 0, 1, -1, 0, 0, -1},
};

// http://roguebasin.com/index.php/C++_shadowcasting_implementation
static void fov_cast_light(
	coord x,
	coord y,
	coord radius,
	coord row,
	double start_slope,
	double end_slope,
	int xx,
	int xy,
	int yx,
	int yy
)
{
	if (start_slope < end_slope) {
		return;
	}
	double next_start_slope = start_slope;
	for (coord i = row; i <= radius; i++) {
		bool blocked = false;
		for (int dx = -i, dy = -i; dx <= 0; dx++) {
			double l_slope = (dx - 0.5) / (dy + 0.5);
			double r_slope = (dx + 0.5) / (dy - 0.5);
			if (start_slope < r_slope) {
				continue;
			}
			if (end_slope > l_slope) {
				break;
			}

			int sax = dx * xx + dy * xy;
			int say = dx * yx + dy * yy;
			if ((sax < 0 && (coord)abs(sax) > x)
				|| (say < 0 && (coord)abs(say) > y)
			) {
				continue;
			}
			coord ax = x + sax;
			coord ay = y + say;
			if (!map_in_bounds(ay, ax)) {
				continue;
			}

			struct tile *tile = &TILE_MAP[ay][ax];

			unsigned int radius2 = radius * radius;
			if ((unsigned int)(dx * dx + dy * dy) < radius2) {
				tile->last_visible_type = tile->type;
				tile->visible = true;
			}

			bool opaque = !TILE_TYPES[tile->type].transparent;
			if (blocked) {
				if (opaque) {
					next_start_slope = r_slope;
					continue;
				}
				blocked = false;
				start_slope = next_start_slope;
			} else if (opaque) {
				blocked = true;
				next_start_slope = r_slope;
				fov_cast_light(
					x,
					y,
					radius,
					i + 1,
					start_slope,
					l_slope,
					xx,
					xy,
					yx,
					yy
				);
			}
		}
		if (blocked) {
			break;
		}
	}
}

void fov_update()
{
	for (coord y = 0; y < MAX_Y; y++) {
		for (coord x = 0; x < MAX_X; x++) {
			TILE_MAP[y][x].visible = false;
		}
	}

	struct tile *player_tile = &TILE_MAP[PLAYER.y][PLAYER.x];
	player_tile->visible = true;
	player_tile->last_visible_type = player_tile->type;

	for (coord i = 0; i < 8; i++) {
		fov_cast_light(
			PLAYER.x,
			PLAYER.y,
			FOV_RADIUS,
			1,
			1.0,
			0.0,
			MULTIPLIERS[0][i],
			MULTIPLIERS[1][i],
			MULTIPLIERS[2][i],
			MULTIPLIERS[3][i]
		);
	}
}
