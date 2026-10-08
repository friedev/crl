#define TB_IMPL
#include <stdlib.h>
#include <termbox2.h>
#include <time.h>

#include "draw.h"
#include "entity.h"
#include "fov.h"
#include "item.h"
#include "map.h"

static void init_all() {
    draw_init();
    map_init();
    item_init_all();
    entity_init_all();
    player_init();
    fov_update();
}

static void next_turn() {
    entity_act_all();
    fov_update();
}

static void free_all() {
    entity_free_all();
    player_free();
    item_free_all();
    draw_free();
}

static bool handle_input(struct tb_event event) {
    bool playing = true;
    bool end_turn = false;
    struct item_node *item_node;
    switch (event.ch) {
    case 'k':
        end_turn = entity_move_attack_delta(&PLAYER, -1, 0);
        break;
    case 'j':
        end_turn = entity_move_attack_delta(&PLAYER, +1, 0);
        break;
    case 'h':
        end_turn = entity_move_attack_delta(&PLAYER, 0, -1);
        break;
    case 'l':
        end_turn = entity_move_attack_delta(&PLAYER, 0, +1);
        break;
    case 'y':
        end_turn = entity_move_attack_delta(&PLAYER, -1, -1);
        break;
    case 'u':
        end_turn = entity_move_attack_delta(&PLAYER, -1, +1);
        break;
    case 'b':
        end_turn = entity_move_attack_delta(&PLAYER, +1, -1);
        break;
    case 'n':
        end_turn = entity_move_attack_delta(&PLAYER, +1, +1);
        break;
    case 'g':
        item_node = TILE_MAP[PLAYER.y][PLAYER.x].items.head;
        if (item_node != NULL) {
            entity_pick_up_item(&PLAYER, item_node);
            end_turn = true;
        }
        break;
    case 'd':
        item_node = PLAYER.items.head;
        if (item_node != NULL) {
            entity_drop_item(&PLAYER, item_node);
            end_turn = true;
        }
        break;
    case '.':
        end_turn = true;
        break;
    case 'q':
        playing = false;
        break;
    default:
        end_turn = false;
        break;
    }
    if (end_turn) {
        next_turn();
    }
    return playing;
}

int main() {
    srand(time(NULL));
    init_all();

    struct tb_event event;
    bool playing = true;
    while (playing) {
        draw();
        tb_present();
        tb_poll_event(&event);
        playing = handle_input(event);
        tb_clear();
    }

    free_all();
    return 0;
}
