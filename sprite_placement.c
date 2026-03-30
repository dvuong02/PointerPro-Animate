#include "animate_struct.h"
#include <stdlib.h>

struct sprite_placement* animate_place_sprite(struct canvas* canvas, struct sprite* sprite, ssize_t x, ssize_t y) {

}

// optional
void animate_placement_up(struct sprite_placement* sprite_placement) {
}

// optional
void animate_placement_down(struct sprite_placement* sprite_placement) {
}

void animate_placement_top(struct sprite_placement* sprite_placement) {
}

void animate_placement_bottom(struct sprite_placement* sprite_placement) {
}

void animate_destroy_placement(struct sprite_placement* sprite_placement) {
}

void animate_set_animation_params(struct sprite_placement* sprite_placement, ssize_t vx, ssize_t vy, ssize_t ax, ssize_t ay) {
}

// optional
void animate_set_animation_function(struct sprite_placement* sprite_placement, animate_fn fn, void* priv) {
}
