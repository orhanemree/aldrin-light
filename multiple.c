#include "lib/aldrin-light.c"

#define WIDTH 200
#define HEIGHT 200

static uint32_t pixels0[WIDTH*HEIGHT];
Aldrin_Canvas ac0 = { pixels0, WIDTH, HEIGHT };

static uint32_t pixels1[WIDTH*HEIGHT];
Aldrin_Canvas ac1 = { pixels1, WIDTH, HEIGHT };