#include "animate_struct.h"
#include <stdlib.h>

struct sprite_placement* animate_place_sprite(struct canvas* canvas, struct sprite* sprite, ssize_t x, ssize_t y) {
    // validate that canvas and sprite exist
    if (!canvas || !sprite) return NULL;
    
    // allocate memory for new placement
    struct sprite_placement* new_placement = malloc(sizeof(struct sprite_placement));
    // validate new_placement memory allocation
    if(!new_placement) return NULL;

    // assign basic properties
    new_placement->sprite = sprite;
    new_placement->canvas = canvas;
    new_placement->x = x;
    new_placement->y = y;
    new_placement->vx = 0;
    new_placement->vy = 0;
    new_placement->ax = 0;
    new_placement->ay = 0;
    new_placement->prev = NULL;
    new_placement->next = NULL;
    new_placement->sprite->reference_count++;

    // 'place' node into linked list and assign/reassign all prev and next pointers of neighbours
    // assign newly placed node to tail position (top-most)
    new_placement->prev = canvas->tail;
    new_placement->next = NULL;
    // if list has an existing tail node
    if(canvas->tail != NULL){
        canvas->tail->next = new_placement;
    } else { // if list is empty, assign new_placement to head
        canvas->head = new_placement;
    }
    // assign new node to linked list's tail node
    canvas->tail = new_placement;

    return new_placement;
}

// optional
void animate_placement_up(struct sprite_placement* sprite_placement) {
}

// optional
void animate_placement_down(struct sprite_placement* sprite_placement) {
}

void animate_placement_top(struct sprite_placement* sprite_placement) {
    // validate that placement and its canvas exist
    if(!sprite_placement || !sprite_placement->canvas){
        return;
    }

    // check if sprite_placement is already at top (tail)
    if(sprite_placement->canvas->tail == sprite_placement){
        return;
    }

    // redirect sprite_placement's previous node's next pointer
    if(sprite_placement->prev != NULL){
        sprite_placement->prev->next = sprite_placement->next;
    } else { // if sprite_placement was the head node
        sprite_placement->canvas->head = sprite_placement->next;
    }
    // redirect sprite_placement's next node's prev pointer
    if(sprite_placement->next != NULL){
        sprite_placement->next->prev = sprite_placement->prev;
    }

    // redirect sprite_placement's prev and next pointers
    sprite_placement->prev = sprite_placement->canvas->tail;
    sprite_placement->next = NULL;

    // redirect current canvas tail's next pointer
    sprite_placement->canvas->tail->next = sprite_placement;

    // replace canvas's tail pointer to sprite_placement
    sprite_placement->canvas->tail = sprite_placement;
}

void animate_placement_bottom(struct sprite_placement* sprite_placement) {
    // validate that placement and its canvas exist
    if(!sprite_placement || !sprite_placement->canvas){
        return;
    }

    // check if sprite placement is already at bottom (head)
    if(sprite_placement->canvas->head == sprite_placement){
        return;
    }

    // redirect sprite_placement's previous node's next pointer
    if(sprite_placement->prev != NULL){
        sprite_placement->prev->next = sprite_placement->next;
    }
    // redirect sprite_placement's next node's prev pointer
    if(sprite_placement->next != NULL){
        sprite_placement->next->prev = sprite_placement->prev;
    } else { // if sprite_placement was the tail node
        sprite_placement->canvas->tail = sprite_placement->prev;
    }

    // redirect sprite_placement's prev and next pointers
    sprite_placement->prev = NULL;
    sprite_placement->next = sprite_placement->canvas->head;
    // redirect current canvas head's prev pointer
    sprite_placement->canvas->head->prev = sprite_placement;
    // redirect canvas's head pointer to sprite_placement
    sprite_placement->canvas->head = sprite_placement;
}

void animate_destroy_placement(struct sprite_placement* sprite_placement) {
    // case 1: node has prev and next (middle of list)
    if(sprite_placement->next != NULL && sprite_placement->prev != NULL){
        sprite_placement->prev->next = sprite_placement->next;
        sprite_placement->next->prev = sprite_placement->prev;
    // case 2: node has prev only (tail)
    } else if(sprite_placement->prev != NULL){
        sprite_placement->prev->next = NULL;
        sprite_placement->canvas->tail = sprite_placement->prev;
    // case 3: node has next only (head)
    } else if(sprite_placement->next != NULL){
        sprite_placement->next->prev = NULL;
        sprite_placement->canvas->head = sprite_placement->next;
    // case 4: node has neither prev or next
    } else {
        sprite_placement->canvas->head = NULL;
        sprite_placement->canvas->tail = NULL;
    }

    // free placement memory
    sprite_placement->sprite->reference_count--;
    free(sprite_placement);
}

void animate_set_animation_params(struct sprite_placement* sprite_placement, ssize_t vx, ssize_t vy, ssize_t ax, ssize_t ay) {
    // assign basic properties
    sprite_placement->vx = vx;
    sprite_placement->vy = vy;
    sprite_placement->ax = ax;
    sprite_placement->ay = ay;
}

// optional
void animate_set_animation_function(struct sprite_placement* sprite_placement, animate_fn fn, void* priv) {
}
