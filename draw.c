#include <termbox.h>

#include "draw.h"

#include "entity.h"
#include "map.h"
#include "message.h"

void draw_init()
{
	tb_init();
	tb_set_output_mode(TB_OUTPUT_256);
}

void draw_free()
{
	tb_shutdown();
}

static void draw_cell(coord_t y, coord_t x, char ch, color_t fg, color_t bg)
{
	tb_set_cell(x, y, ch, fg, bg);
}

static void draw_tile(coord_t y, coord_t x, struct tile *tile)
{
	const struct tile_type *type = &TILE_TYPES[tile->type];
	draw_cell(y, x, type->ch, type->fg, type->bg);
}

static void draw_item(coord_t y, coord_t x, struct item *item)
{
	const struct item_type *type = &ITEM_TYPES[item->type];
	draw_cell(y, x, type->ch, type->fg, type->bg);
}

static void draw_entity(coord_t y, coord_t x, struct entity *entity)
{
	const struct entity_type *type = &ENTITY_TYPES[entity->type];
	draw_cell(y, x, type->ch, type->fg, type->bg);
}

static void draw_coord(coord_t cy, coord_t cx, coord_t y, coord_t x)
{
	struct tile *tile = &TILE_MAP[y][x];
	if (tile->last_visible_type == TILE_INVALID) {
		return;
	}

	const struct tile_type *tile_type;
	if (!tile->visible) {
		tile_type = &TILE_TYPES[tile->last_visible_type];
		draw_cell(cy, cx, tile_type->ch, FG_MEMORY, BG_MEMORY);
		return;
	}

	if (tile->entity != NULL) {
		draw_entity(cy, cx, tile->entity);
		return;
	}

	if (tile->items.head != NULL) {
		draw_item(cy, cx, tile->items.head->item);
		return;
	}

	draw_tile(cy, cx, tile);
}

static void draw_map()
{
	int max_y = tb_height();
	int max_x = tb_width();
	max_y -= MESSAGE_COUNT;
	int off_y = PLAYER.y - max_y / 2;
	int off_x = PLAYER.x - max_x / 2;
	for (int cy = 0; cy < max_y; cy++) {
		for (int cx = 0; cx < max_x; cx++) {
			int y = cy + off_y;
			int x = cx + off_x;
			if (map_in_bounds(y, x)) {
				draw_coord(cy, cx, y, x);
			}
		}
	}
}

static void draw_messages()
{
	int y = tb_height() - MESSAGE_COUNT;
	int x = 0;
	int i = MESSAGE_INDEX;
	do {
		tb_print(x, y, TB_DEFAULT, TB_DEFAULT, MESSAGES[i]);
		y++;
		i++;
		i %= MESSAGE_COUNT;
	} while (i != MESSAGE_INDEX);
}

void draw()
{
	draw_map();
	draw_messages();
}
