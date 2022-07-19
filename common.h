#ifndef COMMON_H
#define COMMON_H

#include <stdbool.h>
#include <stdint.h>

#include "colors.h"

#define INVALID_COORD (-1)

typedef uint8_t coord;

struct tile_type {
	// TODO make a glyph struct
	const short pair;
	const char symbol;
	const bool walkable;
	const bool transparent;
	const char *name;
};

enum {
	TILE_FLOOR,
	TILE_WALL,
	TILE_TYPE_COUNT,
	TILE_INVALID = TILE_TYPE_COUNT,
};

extern const struct tile_type TILE_TYPES[];

struct tile {
	uint8_t type; // Index into TILE_TYPES
	uint8_t last_visible_type; // Index into TILE_TYPES
	bool visible;
	struct entity *entity;
	struct item *item_head;
};

struct item_type {
	const short pair;
	const char symbol;
	const char *name;
};

enum {
	ITEM_GOLD,
	ITEM_TYPE_COUNT,
};

extern const struct item_type ITEM_TYPES[];

struct item {
	uint8_t type; // Index into ITEM_TYPES
	struct item *prev;
	struct item *next;
};

struct entity_type {
	const short pair;
	const char symbol;
	const char *name;
};

enum {
	ENTITY_PLAYER,
	ENTITY_GOBLIN,
	ENTITY_TYPE_COUNT,
};

extern const struct entity_type ENTITY_TYPES[];

struct entity {
	struct item *item_head;
	uint8_t type; // Index into ENTITY_TYPES
	coord y;
	coord x;
	struct entity *prev;
	struct entity *next;
};

#endif
