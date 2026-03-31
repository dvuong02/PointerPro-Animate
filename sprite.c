#include "animate_struct.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

struct bitmap_header {
    uint8_t  magic[2];
    uint32_t size_bytes;
    uint16_t reserved[2];
    uint32_t pixel_offset;
} __attribute__((packed));

struct bitmapv5_header {
    uint32_t bV5Size;
    uint32_t bV5Width;
    uint32_t bV5Height;
    uint16_t bV5Planes;
    uint16_t bV5BitCount;
    uint32_t bV5Compression;
    uint32_t bV5SizeImage;
    uint32_t bV5XPelsPerMeter;
    uint32_t bV5YPelsPerMeter;
    uint32_t bV5ClrUsed;
    uint32_t bV5ClrImportant;
    uint32_t bV5RedMask;
    uint32_t bV5GreenMask;
    uint32_t bV5BlueMask;
    uint32_t bV5AlphaMask;
    uint32_t bV5CSType;
    uint8_t  bV5Endpoints[36];
    uint32_t bV5GammaRed;
    uint32_t bV5GammaGreen;
    uint32_t bV5GammaBlue;
    uint32_t bV5Intent;
    uint32_t bV5ProfileData;
    uint32_t bV5ProfileSize;
    uint32_t bV5Reserved;
};

struct sprite* animate_create_sprite(const char* file) {
    // open bitmap file - "rb" read binary
    FILE* bitmap_file = fopen(file, "rb");
    // validate file open
    if(!bitmap_file) return NULL;

    // read bitmap header
    // declare bitmap header struct instance
    struct bitmap_header bmp_header;
    // store data in bmp_header, read element of size bmp_header, read 1 element, from bitmap_file
    if(fread(&bmp_header, sizeof(bmp_header), 1, bitmap_file) != 1){
        // validates that the file starts with a header of size bitmap_header
        fclose(bitmap_file);
        return NULL;
    }

    // read bitmap v5 header
    struct bitmapv5_header bmpv5_header;
    // store data in bmpv5_header, read element of size bmpv5_header, read 1 element, from bitmap_file
    if(fread(&bmpv5_header, sizeof(bmpv5_header), 1, bitmap_file) != 1) {
        // validate that a header of size of bitmapv5_header follows bitmap_header
        fclose(bitmap_file);
        return NULL;
    }

    // allocate memory for new sprite
    struct sprite* new_sprite = malloc(sizeof(struct sprite));
    // validate new_sprite memory allocation
    if(!new_sprite) return NULL;

    // allocate basic properties
    new_sprite->width = bmpv5_header.bV5Width;
    new_sprite->height = bmpv5_header.bV5Height;
    new_sprite->reference_count = 1; // initialised at 1, not 0, because creation is 1st reference to sprite

    // allocate pixel buffer for sprite
    new_sprite->pixels = malloc(bmpv5_header.bV5Width * bmpv5_header.bV5Height * sizeof(color_t));
    // validate pixels memory allocation
    if(!new_sprite->pixels){
        free(new_sprite); // if error allocating pixel buffer, free sprite
        return NULL;
    }

    // move (seek) the fread cursor to the start of the pixel data in the bitmap file
    fseek(bitmap_file, bmp_header.pixel_offset, SEEK_SET); // seek_set -> move cursor to this byte from START of file

    // convert each row of data from BGRA to ARGB (read backwards)
    // create buffer to store temporary row data to flip
    int row_bytes = new_sprite->width * 4; 
    uint8_t temp_row[row_bytes]; // temporary row buffer of size (width of sprite * bytes per pixel (color_T))

    // loop through rows starting at the top of the data, corresponding to BOTTOM row of sprite image
    for(int row = bmpv5_header.bV5Height - 1; row >= 0; row--){
        // read one row from bitmap_file, store in temp_row
        fread(temp_row, row_bytes, 1, bitmap_file);
        
        // read each entry (column) of the row
        for(int col = 0; col < bmpv5_header.bV5Width; col++){
            uint8_t B = temp_row[col*4 + 0]; // read first 8 bits of current entry
            uint8_t G = temp_row[col*4 + 1]; // read next 8 bits of current entry
            uint8_t R = temp_row[col*4 + 2]; // read next 8 bits of current entry
            uint8_t A = temp_row[col*4 + 3]; // read last 8 bits of current entry

            // pass A, R, G and B values to static inline function to assign color_t value to pixel
            color_t pixel = animate_color_argb(A, R, G, B);

            // store new pixel of type color_t into the sprite pixel buffer, starting from the BOTTOM row
            new_sprite->pixels[row * bmpv5_header.bV5Width + col] = pixel;
        }
    }

    // close file
    fclose(bitmap_file);
    return new_sprite;
}

struct sprite* animate_create_rectangle(size_t width, size_t height, color_t c, bool filled) {
    // allocate memory for new sprite
    struct sprite* new_sprite = malloc(sizeof(struct sprite));
    // validate new_sprite memory allocation
    if(!new_sprite) return NULL;
    
    // allocate basic properties
    new_sprite->width = width;
    new_sprite->height = height;
    new_sprite->reference_count = 1;

    // allocate pixel buffer
    new_sprite->pixels = malloc(width*height*sizeof(color_t));
    // validate pixels memory allocation
    if(!new_sprite->pixels){
        free(new_sprite); // if error allocating pixel buffer, free sprite
        return NULL; 
    }

    // fill each pixel according to bool filled
    for(int y = 0; y < height; y++){
        for(int x = 0; x < width; x++){
            if(filled){ // if rectangle is filled...
                new_sprite->pixels[y*width + x] = c;
            } else if (!filled) { // if rectangle is not filled (only fill borders)
                if(y == 0 || y == height - 1 || x == 0 || x == width - 1){ // filled pixel
                    new_sprite->pixels[y*width + x] = c;
                } else { // transparent pixel
                    new_sprite->pixels[y*width + x] = animate_color_argb(0, 0, 0, 0);
                }
            }
        }
    }

    return new_sprite;
}

struct sprite* animate_create_circle(size_t radius, color_t c, bool filled) {
    // allocate memory for new sprite
    struct sprite* new_sprite = malloc(sizeof(struct sprite));
    // validate new_sprite memory allocation
    if(!new_sprite) return NULL;

    // assign basic properties
    size_t diameter = radius * 2;
    new_sprite->width = diameter;
    new_sprite->height = diameter;
    new_sprite->reference_count = 1;

    // allocate memory for pixel buffer
    new_sprite->pixels = malloc(diameter*diameter*sizeof(color_t));
    // validate pixels memory allocation
    if(!new_sprite->pixels){
        free(new_sprite); // if error allocating pixel buffer, free sprite
        return NULL;
    }

    // fill pixels using circle equation
    for(int y = 0; y < diameter; y++){
        for(int x = 0; x < diameter; x++){
            // calculate distance from circle centre using circle equation
            int circle_eqn = (x-radius)*(x-radius) + (y-radius)*(y-radius);
            // for any points inside the circle, colour c
            if(circle_eqn <= radius*radius){
                new_sprite->pixels[y*diameter + x] = c;
            // for any points outside of the circle, transparent
            } else {
                new_sprite->pixels[y*diameter + x] = animate_color_argb(0, 0, 0, 0);
            }
        }
    }

    return new_sprite;
}

bool animate_destroy_sprite(struct sprite* sprite) {
    // validate that sprite exists
    if(!sprite) return false;

    // if only 1 reference to the sprite exists (from its creation), decrement and free sprite pixel buffer and sprite
    if(sprite->reference_count == 1){
        sprite->reference_count--; // not technically necessary, but follows proper referencing
        free(sprite->pixels); 
        free(sprite);
    }
    // successful operation returns true (even if sprite isnt freed)
    return true;
}
