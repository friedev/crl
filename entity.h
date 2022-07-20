#ifndef ENTITY_H
#define ENTITY_H

#include "common.h"

extern struct entity_list ENTITIES;

extern struct entity PLAYER;

bool entity_move_attack_delta(struct entity *entity, int dy, int dx);
void entity_pick_up_item(struct entity *entity, struct item_node *item_node);
void entity_drop_item(struct entity *entity, struct item_node *item_node);
void entity_init_all();
void player_init();
void entity_act_all();
void entity_free_all();
void player_free();

#endif
