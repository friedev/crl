#include <stdio.h>
#include <stdlib.h>

#include "entity.h"

#include "item.h"
#include "map.h"
#include "message.h"

struct entity_list ENTITIES = {
    .size = 0,
    .head = NULL,
    .tail = NULL,
};

struct entity PLAYER = {
    .items =
        {
            .size = 0,
            .head = NULL,
            .tail = NULL,
        },
    .y = 0, // Overwritten in init_player()
    .x = 0, // Overwritten in init_player()
    .type = ENTITY_PLAYER,
};

void entity_list_add_node(struct entity_list *list, struct entity_node *node) {
    node->prev = list->tail;
    if (list->head == NULL) {
        list->head = node;
    } else if (list->tail != NULL) {
        list->tail->next = node;
    }
    list->tail = node;
    list->size++;
}

void entity_list_add(struct entity_list *list, struct entity *entity) {
    struct entity_node *node = malloc(sizeof(struct entity_node));
    *node = (struct entity_node){
        .entity = entity,
        .next = NULL,
        .prev = NULL,
    };
    entity_list_add_node(list, node);
}

struct entity_node *
    entity_list_find(struct entity_list *list, struct entity *entity) {
    struct entity_node *current = list->head;
    while (current != NULL && current->entity != entity) {
        current = current->next;
    }
    return current;
}

void entity_list_free_node(struct entity_list *list, struct entity_node *node) {
    if (node == list->head) {
        list->head = node->next;
    }
    if (node == list->tail) {
        list->tail = node->prev;
    }
    if (node->prev != NULL) {
        node->prev->next = node->next;
    }
    if (node->next != NULL) {
        node->next->prev = node->prev;
    }

    list->size--;
    free(node);
}

void entity_list_remove(struct entity_list *list, struct entity *entity) {
    entity_list_free_node(list, entity_list_find(list, entity));
}

void entity_list_clear(struct entity_list *list) {
    while (list->head != NULL) {
        entity_list_free_node(list, list->head);
    }
}

static void entity_free(struct entity *entity) {
    if (map_in_bounds(entity->y, entity->x)) {
        TILE_MAP[entity->y][entity->x].entity = NULL;
    }
    item_list_free(&entity->items);
    free(entity);
}

void entity_list_free(struct entity_list *list) {
    while (list->head != NULL) {
        entity_free(list->head->entity);
        entity_list_free_node(list, list->head);
    }
}

static bool entity_move(struct entity *entity, coord_t y, coord_t x) {
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

static bool entity_attack(struct entity *entity, coord_t y, coord_t x) {
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

    entity_list_remove(&ENTITIES, target);
    entity_free(target);

    return true;
}

static bool entity_move_attack(struct entity *entity, coord_t y, coord_t x) {
    return entity_move(entity, y, x) || entity_attack(entity, y, x);
}

static bool entity_move_delta(struct entity *entity, int dy, int dx) {
    return entity_move(entity, entity->y + dy, entity->x + dx);
}

static bool entity_attack_delta(struct entity *entity, int dy, int dx) {
    return entity_attack(entity, entity->y + dy, entity->x + dx);
}

bool entity_move_attack_delta(struct entity *entity, int dy, int dx) {
    return entity_move_attack(entity, entity->y + dy, entity->x + dx);
}

void entity_pick_up_item(struct entity *entity, struct item_node *item_node) {
    struct item_list *tile_items = &TILE_MAP[entity->y][entity->x].items;
    // TODO assert tile_items contains item_node

    char message[MESSAGE_SIZE];
    snprintf(
        message,
        MESSAGE_SIZE,
        "The %s picks up the %s.",
        ENTITY_TYPES[entity->type].name,
        ITEM_TYPES[item_node->item->type].name
    );

    message_add(message);
    item_list_add(&entity->items, item_node->item);
    item_list_free_node(tile_items, item_node);
}

void entity_drop_item(struct entity *entity, struct item_node *item_node) {
    struct item_list *tile_items = &TILE_MAP[entity->y][entity->x].items;
    // TODO assert entity->items contains item_node

    char message[MESSAGE_SIZE];
    snprintf(
        message,
        MESSAGE_SIZE,
        "The %s drops the %s.",
        ENTITY_TYPES[entity->type].name,
        ITEM_TYPES[item_node->item->type].name
    );
    message_add(message);

    item_list_add(tile_items, item_node->item);
    item_list_free_node(&entity->items, item_node);
}

static void entity_place(struct entity *entity) {
    while (!entity_move(entity, rand() % MAX_Y, rand() % MAX_X)) {}
}

void entity_init_all() {
    // TODO spawn tables or something
    for (int i = 0; i < 30; i++) {
        struct entity *entity = malloc(sizeof(struct entity));
        *entity = (struct entity){
            .type = ENTITY_GOBLIN,
            .y = INVALID_COORD,
            .x = INVALID_COORD,
            .items =
                {
                    .size = 0,
                    .head = NULL,
                    .tail = NULL,
                },
        };
        entity_place(entity);
        entity_list_add(&ENTITIES, entity);
    }
}

void player_init() {
    entity_place(&PLAYER);
}

static void entity_act(struct entity *entity) {
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

void entity_act_all() {
    struct entity_node *current = ENTITIES.head;
    while (current != NULL) {
        entity_act(current->entity);
        current = current->next;
    }
}

void entity_free_all() {
    entity_list_free(&ENTITIES);
}

void player_free() {
    item_list_free(&PLAYER.items);
}
