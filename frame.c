#include "animate_struct.h"
#include <stdlib.h>
#include <math.h>

size_t animate_frame_size_bytes(struct canvas* canvas) {
    size_t height = canvas-> height; // number of pixels in canvas height
    size_t width = canvas-> width; // number of pixels in canvas width
    return height * width * sizeof(color_t); // multiply amount of pixels by size of pixel (color_t is 4 bytes)
}

void animate_generate_frame(const struct canvas* canvas, size_t frame, size_t frame_rate, void* buf) {
    // cast pixel buffer type to color_t -> pixels references same address of buf
    color_t* pixels = (color_t*)buf;

    // colour each pixel in buffer to canvas colour
    for(size_t i = 0; i < canvas->height * canvas->width; i++){
        pixels[i] = canvas->background_color;
    }

    // calculate time t from provided frame number and frame rate
    double time = (double)frame / frame_rate; // parse to double to perform double arithmetic

    // initialise pointer to current node
    struct sprite_placement* current_node = canvas->head;
    // loop through all placements in the canvas
    while(current_node != NULL){
        // initialise all motion variables of node
        // initial positions
        ssize_t initial_x = current_node->x;
        ssize_t initial_y = current_node->y;
        // velocity
        ssize_t velocity_x = current_node->vx;
        ssize_t velocity_y = current_node->vy;
        // acceleration
        ssize_t accel_x = current_node->ax;
        ssize_t accel_y = current_node->ay;
        // final positions based off of motion equations -> round to nearest integer using llround
        ssize_t final_x = (ssize_t)llround(initial_x + (velocity_x * time) + ((accel_x * time * time) / 2));
        ssize_t final_y = (ssize_t)llround(initial_y + (velocity_y * time) + ((accel_y * time * time) / 2));

        // loop through the sprite's pixels and draw them to pixel buffer (frame)
        size_t width = current_node->sprite->width;
        size_t height = current_node->sprite->height;
        for(size_t y = 0; y < height; y++){
            for(size_t x = 0; x < width; x++){
                // check if alpha channel of pixel is 0
                color_t sprite_pixel = current_node->sprite->pixels[y * width + x];
                if((sprite_pixel >> 24) == 0){ // when right shifted by 24 bits, is the remaining 8-bit alpha channel = 0
                    continue; // do not overwrite canvas background, go to next pixel
                } 
                // replace canvas pixel in buffer with sprite pixel if alpha > 0
                // compute placement x and y of sprite in frame
                ssize_t placement_x = final_x + x; // final position of sprite in frame after motion
                ssize_t placement_y = final_y + y; // signed int ssize_t is used because placement may be negative (out of frame)
                // check if placement is out of frame
                if(placement_x < 0 || placement_x >= (ssize_t)canvas->width || placement_y < 0 || placement_y >= (ssize_t)canvas->height){
                    continue;
                }
                // convert to type size_t to align with indexing type of pixel buffer
                size_t frame_x = (size_t)placement_x;
                size_t frame_y = (size_t)placement_y;
                // write pixel to buffer (frame)
                pixels[frame_y * canvas->width + frame_x] = sprite_pixel;
            }
        }
        // go to next node in list, if not NULL
        current_node = current_node->next;
    }
}
