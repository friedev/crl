#ifndef COMMON_H
#define COMMON_H

#include <stdbool.h>
#include <stdint.h>

#define INVALID_COORD (-1)

// Adjustments for 256-color mode
// TB_BLACK is 1, but black in 256-color mode is 0
// Bright colors start with bright black at 0x08
#define DARK(COLOR) ((COLOR)-0x01)
#define BRIGHT(COLOR) (DARK(COLOR) + 0x08)

typedef uint8_t coord_t;

// Defined to match uintattr_t without needing to include termbox in common.h
typedef uint16_t color_t;

extern const color_t FG_MEMORY;
extern const color_t BG_MEMORY;

struct item_type {
	const char ch;
	const color_t fg;
	const color_t bg;
	const char *name;
};

enum {
	ITEM_GOLD,
	ITEM_TYPE_COUNT,
};

extern const struct item_type ITEM_TYPES[];

struct item {
	uint8_t type; // Index into ITEM_TYPES
};

struct item_node {
	struct item *item;
	struct item_node *prev;
	struct item_node *next;
};

struct item_list {
	uint8_t size;
	struct item_node *head;
	struct item_node *tail;
};

struct tile_type {
	const char ch;
	const color_t fg;
	const color_t bg;
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
	struct item_list items;
};

struct entity_type {
	const char ch;
	const color_t fg;
	const color_t bg;
	const char *name;
};

enum {
	ENTITY_PLAYER,
	ENTITY_GOBLIN,
	ENTITY_TYPE_COUNT,
};

extern const struct entity_type ENTITY_TYPES[];

struct entity {
	uint8_t type; // Index into ENTITY_TYPES
	coord_t y;
	coord_t x;
	struct item_list items;
};

struct entity_node {
	struct entity *entity;
	struct entity_node *prev;
	struct entity_node *next;
};

struct entity_list {
	uint8_t size;
	struct entity_node *head;
	struct entity_node *tail;
};

#endif
