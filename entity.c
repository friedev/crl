#include <stdlib.h>
#include <stdio.h>

#include "entity.h"

#include "item.h"
#include "map.h"
#include "message.h"

struct entity *ENTITY_HEAD = NULL;

struct entity PLAYER = {
	.item_head = NULL,
	.y = 0, // Overwritten in init_player()
	.x = 0, // Overwritten in init_player()
	.type = ENTITY_PLAYER,
};

static void entity_add(struct entity *entity)
{
	entity->prev = NULL;
	entity->next = ENTITY_HEAD;
	if (ENTITY_HEAD != NULL) {
		ENTITY_HEAD->prev = entity;
	}
	ENTITY_HEAD = entity;
}

static void entity_del(struct entity *entity)
{
	if (ENTITY_HEAD == entity) {
		ENTITY_HEAD = entity->next;
	}
	if (entity->prev != NULL) {
		entity->prev->next = entity->next;
	}
	if (entity->next != NULL) {
		entity->next->prev = entity->prev;
	}
	if (map_in_bounds(entity->y, entity->x)) {
		TILE_MAP[entity->y][entity->x].entity = NULL;
	}
	item_list_del(entity->item_head);
	free(entity);
}

static bool entity_move(struct entity *entity, coord y, coord x)
{
	if (!map_in_bounds(y, x)) {
		return false;
	}

	struct tile *tile = &TILE_MAP[y][x];
	if (tile->entity != NULL) {
		return false;
	}

	const struct tile_type *tile_type = &TILE_TYPES[tile->type];
	if (!tile_type->walkable) {
		return false;
	}

	if (entity->y == y && entity->x == x) {
		return true;
	}

	if (map_in_bounds(entity->y, entity->x)) {
		TILE_MAP[entity->y][entity->x].entity = NULL;
	}
	entity->y = y;
	entity->x = x;
	tile->entity = entity;

	return true;
}

static bool entity_attack(struct entity *entity, coord y, coord x)
{
	if (!map_in_bounds(y, x)) {
		return false;
	}

	struct entity *target = TILE_MAP[y][x].entity;
	if (target == NULL) {
		return false;
	}

	// TODO HP
	char message[MESSAGE_SIZE];
	snprintf(
		message,
		MESSAGE_SIZE,
		"The %s kills the %s.",
		ENTITY_TYPES[entity->type].name,
		ENTITY_TYPES[target->type].name
	);
	message_add(message);
	entity_del(target);

	return true;
}

static bool entity_move_attack(struct entity *entity, coord y, coord x)
{
	return entity_move(entity, y, x) || entity_attack(entity, y, x);
}

static bool entity_move_delta(struct entity *entity, int dy, int dx)
{
	return entity_move(entity, entity->y + dy, entity->x + dx);
}

static bool entity_attack_delta(struct entity *entity, int dy, int dx)
{
	return entity_attack(entity, entity->y + dy, entity->x + dx);
}

bool entity_move_attack_delta(struct entity *entity, int dy, int dx)
{
	return entity_move_attack(entity, entity->y + dy, entity->x + dx);
}

static void entity_place(struct entity *entity)
{
	while (!entity_move(entity, rand() % MAX_Y, rand() % MAX_X)) {}
}

void entity_init_all()
{
	// TODO spawn tables or something
	for (int i = 0; i < 30; i++) {
		struct entity *entity = malloc(sizeof(struct entity));
		*entity = (struct entity) {
			.item_head = NULL,
			.next = NULL,
			.prev = NULL,
			.type = ENTITY_GOBLIN,
			.y = INVALID_COORD,
			.x = INVALID_COORD,
		};
		entity_place(entity);
		entity_add(entity);
	}
}

void player_init()
{
	entity_place(&PLAYER);
}

static void entity_act(struct entity *entity)
{
	// TODO A* pathfinding
	// http://roguebasin.com/index.php/Pathfinding
	// Try until move succeeds
	for (int i = 0; i < 30; i++) {
		int dy = rand() % 3 - 1;
		int dx = rand() % 3 - 1;
		if (entity_move_delta(entity, dy, dx)) {
			return;
		}
	}
}

void entity_act_all()
{
	struct entity *head = ENTITY_HEAD;
	while (head != NULL) {
		entity_act(head);
		head = head->next;
	}
}

void entity_free_all()
{
	while (ENTITY_HEAD != NULL) {
		entity_del(ENTITY_HEAD);
	}
}
