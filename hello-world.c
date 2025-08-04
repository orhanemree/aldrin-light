#include "lib/aldrin-light.c"

#define WIDTH 160
#define HEIGHT 90

static uint32_t pixels[WIDTH*HEIGHT];

int main() {
    Aldrin_Canvas ac = { pixels, WIDTH, HEIGHT };
    aldrin_fill(ac, 0xff00ff);
    aldrin_save_ppm(ac, "out/hello_world.ppm");
    return 0;
}