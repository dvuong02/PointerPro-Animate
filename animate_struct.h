#ifndef ANIMATE_STRUCT_H
#define ANIMATE_STRUCT_H

#include "animate.h"
#include <stdint.h>
#include <sys/types.h>
//#include <stddef.h>

struct sprite {
    size_t width; // width of sprite
    size_t height; // height of sprite
    color_t* pixels; // pixel buffer of sprite
    size_t reference_count; // count of all references to sprite
};

struct sprite_placement {
    struct sprite* sprite; // pointer to sprite being placed
    struct canvas* canvas; // pointer to canvas which sprite is being placed on
    ssize_t x; // x-coord placement of sprite in frame
    ssize_t y; // y-coord placement of sprite in frame
    ssize_t vx; // x-velocity of sprite animation in frame
    ssize_t vy; // y-velocity of sprite animation in frame
    ssize_t ax; // x-acceleration of sprite animation in frame
    ssize_t ay; // y-acceleration of sprite animation in frame
    animate_fn fn; // optional animation function
    void* fn_priv; // optional animation function data
    struct sprite_placement* prev; // pointer to previous sprite in canvas placement
    struct sprite_placement* next; // pointer to next sprite in canvas placement
};

struct canvas {
    size_t width; // width of canvas
    size_t height; // height of canvas
    color_t background_color; // background colour of canvas
    struct sprite_placement* head; // pointer to head node of sprite placements
    struct sprite_placement* tail; // pointer to tail node of sprite placements
};

#endif
