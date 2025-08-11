# 🔆 Aldrin Light
* Very basic 2D computer graphics library in C. A lightweight version of [orhanemree/aldrin](https://github.com/orhanemree/aldrin).

## Quick Start
* Just copy and paste `lib/aldrin-light.c` file to your project.
```c
#include "lib/aldrin-light.c" // that's it!
```

## Hello World!
```c
// hello-world.c
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

## Build `WASM`
* In order to use the library on platform `wasm`, you first need to build a blank `Aldrin_Canvas`. So in JS, you can manipulate pixels in the memory.
```c
// wasm.c
#include "lib/aldrin-light.c"

#define WIDTH 200 // pre-set width and height!
#define HEIGHT 200

static uint32_t pixels[WIDTH*HEIGHT];
Aldrin_Canvas ac = { pixels, WIDTH, HEIGHT };
```
* Build file using `clang` and `wasm-ld`, with `PLATFORM_WASM` defined.
```console
$ clang -DPLATFORM_WASM --target=wasm32 -c -o aldrin-light.o wasm.c
$ wasm-ld --no-entry --allow-undefined --export-all -o aldrin-light.wasm aldrin-light.o
```
* You will see `aldrin-light.wasm` file.

## Documentation
* [DOCS.md](./DOCS.md)

## License
* MIT