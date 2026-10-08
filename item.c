#include <assert.h>
#include <stdlib.h>

#include "item.h"

#include "map.h"

void item_list_add_node(struct item_list *list, struct item_node *node) {
    node->prev = list->tail;
    if (list->head == NULL) {
        list->head = node;
    } else if (list->tail != NULL) {
        list->tail->next = node;
    }
    list->tail = node;
    list->size++;
}

void item_list_add(struct item_list *list, struct item *item) {
    struct item_node *node = malloc(sizeof(struct item_node));
    *node = (struct item_node) {
        .item = item,
        .next = NULL,
        .prev = NULL,
    };
    item_list_add_node(list, node);
}

struct item_node *item_list_find(struct item_list *list, struct item *item) {
    struct item_node *current = list->head;
    while (current != NULL && current->item != item) {
        current = current->next;
    }
    return current;
}

void item_list_free_node(struct item_list *list, struct item_node *node) {
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

void item_list_remove(struct item_list *list, struct item *item) {
    item_list_free_node(list, item_list_find(list, item));
}

void item_list_clear(struct item_list *list) {
    while (list->head != NULL) {
        item_list_free_node(list, list->head);
    }
}

void item_free(struct item *item) { free(item); }

void item_list_free(struct item_list *list) {
    while (list->head != NULL) {
        item_free(list->head->item);
        item_list_free_node(list, list->head);
    }
}

static void item_place(struct item *item) {
    coord_t y = rand() % MAX_Y;
    coord_t x = rand() % MAX_X;
    while (!TILE_TYPES[TILE_MAP[y][x].type].walkable) {
        y = rand() % MAX_Y;
        x = rand() % MAX_X;
    }
    item_list_add(&TILE_MAP[y][x].items, item);
}

void item_init_all() {
    // TODO spawn tables or something
    for (int i = 0; i < 30; i++) {
        struct item *item = malloc(sizeof(struct item));
        *item = (struct item) {
            .type = ITEM_GOLD,
        };
        item_place(item);
    }
}

void item_free_all() {
    for (coord_t y = MIN_Y; y < MAX_Y; y++) {
        for (coord_t x = MIN_X; x < MAX_X; x++) {
            item_list_free(&TILE_MAP[y][x].items);
        }
    }
}
