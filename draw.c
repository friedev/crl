#include <curses.h>

#include "draw.h"

#include "colors.h"
#include "entity.h"
#include "map.h"
#include "message.h"

/*
static void draw_init_custom_colors()
{
	for (short i = COLOR_FIRST; i <= (short) COLOR_LAST; i++) {
		struct custom_color color = CUSTOM_COLORS[i - COLOR_FIRST];
		init_color(i, color.r, color.g, color.b);
	}
}
*/

static void draw_init_pairs()
{
	for (short i = 1; i < (short) PAIR_COUNT; i++) {
		struct color_pair pair = PAIRS[i];
		init_pair(i, pair.f, pair.b);
	}
}

static void draw_init_colors()
{
	if (!has_colors()) {
		return;
	}
	start_color();
	use_default_colors();
	//draw_init_custom_colors();
	draw_init_pairs();
}

static void draw_init_curses()
{
	initscr();            // Start curses
	cbreak();             // Use raw mode, but don't block signals
	noecho();             // Don't print what the user types
	keypad(stdscr, true); // Capture non-printing characters
	curs_set(0);          // Hide the cursor
}

void draw_init()
{
	draw_init_curses();
	draw_init_colors();
}

static void draw_char(char symbol, short pair)
{
	attron(COLOR_PAIR(pair));
	addnstr(&symbol, 1);
	attroff(COLOR_PAIR(pair));
}

static void draw_item(struct item *item)
{
	const struct item_type *type = &ITEM_TYPES[item->type];
	draw_char(type->symbol, type->pair);
}

static void draw_entity(struct entity *entity)
{
	const struct entity_type *type = &ENTITY_TYPES[entity->type];
	draw_char(type->symbol, type->pair);
}

static void draw_coord(coord y, coord x)
{
	struct tile *tile = &TILE_MAP[y][x];
	if (tile->last_visible_type == TILE_INVALID) {
		return;
	}

	const struct tile_type *tile_type;
	if (!tile->visible) {
		tile_type = &TILE_TYPES[tile->last_visible_type];
		draw_char(tile_type->symbol, PAIR_MEMORY);
		return;
	}

	if (tile->entity != NULL) {
		draw_entity(tile->entity);
		return;
	}

	if (tile->item_head != NULL) {
		draw_item(tile->item_head);
		return;
	}

	tile_type = &TILE_TYPES[tile->type];
	draw_char(tile_type->symbol, tile_type->pair);
}

static void draw_map()
{
	int maxy;
	int maxx;
	getmaxyx(stdscr, maxy, maxx);
	maxy -= MESSAGE_COUNT;
	int offy = PLAYER.y - maxy / 2;
	int offx = PLAYER.x - maxx / 2;
	for (int cy = 0; cy < maxy; cy++) {
		for (int cx = 0; cx < maxx; cx++) {
			int y = cy + offy;
			int x = cx + offx;
			if (map_in_bounds(y, x)) {
				wmove(stdscr, cy, cx);
				draw_coord(y, x);
			}
		}
	}
}

static void draw_messages()
{
	int y;
	int x;
	getmaxyx(stdscr, y, x);
	y -= MESSAGE_COUNT;
	x = 0;
	int i = MESSAGE_INDEX;
	do {
		mvaddnstr(y, x, MESSAGES[i], MESSAGE_SIZE);
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
