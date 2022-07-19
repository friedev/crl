#include <stdlib.h>

#include "item.h"

#include "map.h"

void item_list_del(struct item *item_head)
{
	while (item_head != NULL) {
		struct item *next_item = item_head->next;
		free(item_head);
		item_head = next_item;
	}
}

static void item_add(coord y, coord x, struct item *item)
{
	item->prev = NULL;
	item->next = TILE_MAP[y][x].item_head;
	if (TILE_MAP[y][x].item_head != NULL) {
		TILE_MAP[y][x].item_head->prev = item;
	}
	TILE_MAP[y][x].item_head = item;
}

static void item_place(struct item *item)
{
	coord y = rand() % MAX_Y;
	coord x = rand() % MAX_X;
	while (!TILE_TYPES[TILE_MAP[y][x].type].walkable) {
		y = rand() % MAX_Y;
		x = rand() % MAX_X;
	}
	item_add(y, x, item);
}

void item_init_all()
{
	// TODO spawn tables or something
	for (int i = 0; i < 30; i++) {
		struct item *item = malloc(sizeof(struct item));
		*item = (struct item) {
			.type = ITEM_GOLD,
			.prev = NULL,
			.next = NULL,
		};
		item_place(item);
	}
}

void item_free_all()
{
	for (coord y = MIN_Y; y < MAX_Y; y++) {
		for (coord x = MIN_X; x < MAX_X; x++) {
			item_list_del(TILE_MAP[y][x].item_head);
		}
	}
}
