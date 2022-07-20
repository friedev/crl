#ifndef ITEM_H
#define ITEM_H

#include "common.h"

void item_list_add_node(struct item_list *list, struct item_node *node);
void item_list_add(struct item_list *list, struct item *item);
struct item_node *item_list_find(struct item_list *list, struct item *item);
void item_list_free_node(struct item_list *list, struct item_node *node);
void item_list_remove(struct item_list *list, struct item *item);
void item_list_clear(struct item_list *list);
void item_free(struct item *item);
void item_list_free(struct item_list *list);
void item_init_all();
void item_free_all();

#endif
