# Aldrin Light
* Very basic 2D computer graphics library in C. A lightweight version of [orhanemree/aldrin](https://github.com/orhanemree/aldrin).

## Quick Start
* Just copy and paste `lib/aldrin-light.c` file to your project.
```c
#include "lib/aldrin-light.c" // that's it!
```

## Hello World!
```c
#include "lib/aldrin-light.c"

#define WIDTH 160
#define HEIGHT 90

static uint32_t pixels[WIDTH*HEIGHT];

int main() {
    Aldrin_Canvas ac = { pixels, WIDTH, HEIGHT };
    aldrin_fill(ac, 0xff00ff); // paint the whole canvas pink!
    aldrin_save_ppm(ac, "out/hello_world.ppm");
    return 0;
}
```
Output (after `.ppm` converted to `.jpg`):
<img src="out/hello_world.jpg">

## Documentation
* [DOCS.md](./DOCS.md)

## License
* MIT