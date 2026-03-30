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
    return NULL;
}

struct sprite* animate_create_rectangle(size_t width, size_t height, color_t c, bool filled) {
    return NULL;
}

struct sprite* animate_create_circle(size_t radius, color_t c, bool filled) {
    return NULL;
}

bool animate_destroy_sprite(struct sprite* sprite) {
    return false;
}
