#include "animate_struct.h"
#include <stdlib.h>

struct canvas* animate_create_canvas(size_t height, size_t width, color_t background_color) {
    // allocate memory for new canvas
    struct canvas* new_canvas = malloc(sizeof(struct canvas));
    
    // assign basic properties
    new_canvas->height = height;
    new_canvas->width = width;
    new_canvas->background_color = background_color;
    new_canvas->head = NULL; // initialise head of doubly-linked list of positions
    new_canvas->tail = NULL; // initialise tail of doubly-linked list of positions
    
    return new_canvas;
}

void animate_destroy_canvas(struct canvas* canvas) {
    // validate that canvas exists
    if(!canvas) return;

    // starting from the canvas' placement head, destroy all placements
    while(canvas->head != NULL){
        animate_destroy_placement(canvas->head);
    }
    // once all placements are destroyed, free memory allocated to canvas
    free(canvas);
}
