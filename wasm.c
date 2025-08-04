#include "lib/aldrin-light.c"

#define WIDTH 200
#define HEIGHT 200

static uint32_t pixels[WIDTH*HEIGHT];
Aldrin_Canvas ac = { pixels, WIDTH, HEIGHT };