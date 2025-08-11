#ifndef ALDRIN_H_
#define ALDRIN_H_

#ifndef PLATFORM_WASM
#include <stdio.h>
#include <math.h>
#endif // PLATFORM_WASM

#ifdef PLATFORM_WASM
// import from JavaScript side
extern double round(double) __attribute__((import_module("env"), import_name("round")));
extern double sqrt(double) __attribute__((import_module("env"), import_name("sqrt")));
extern double pow(double, double) __attribute__((import_module("env"), import_name("pow")));
#endif // PLATFORM_WASM

#include <stdint.h>

#define ac_abs(n) ((int)(n)>=0?(n):(-1*(n)))

double ac_round(double x) { return round(x); }
double ac_sqrt(double x) { return sqrt(x); }
double ac_pow(double x, double y) { return pow(x, y); }


typedef struct Aldrin_Canvas {
    uint32_t *pixels;
    uint32_t width;
    uint32_t height;
} Aldrin_Canvas;


void aldrin_put_pixel(Aldrin_Canvas ac, uint32_t x, uint32_t y, uint32_t color) {
    if (y >= ac.height) return;
    if (x >= ac.width) return;
    ac.pixels[(y*ac.width) + x] = color;
}


void aldrin_fill(Aldrin_Canvas ac, uint32_t color) {
    for (int p = 0; p < ac.width * ac.height; ++p) {
        ac.pixels[p] = color;
    }
}


void ac_swap(uint32_t *x0, uint32_t *y0, uint32_t *x1, uint32_t *y1) {
    // swap x
    uint32_t temp = *x0;
    *x0 = *x1;
    *x1 = temp;

    // swap y
    temp = *y0;
    *y0 = *y1;
    *y1 = temp;
}


int ac_calc_y_on_line(int x0, int y0, int x1, int y1, int x) {
    // if the line is horizontal-ish: y = mx+c
    int dy = y0-y1;
    int dx = x0-x1;
    double m = (double)dy/(double)dx;
    double c = y0-m*x0;
    return ac_round(m*x+c);
}


int ac_calc_x_on_line(int x0, int y0, int x1, int y1, int y) {
    // if the line is vertical-ish or perfect horizontal: x = my+c
    int dy = y0-y1;
    int dx = x0-x1;
    double m = (double)dy/(double)dx;
    double c = y0-m*x0;
    return (dx == 0 || dy == 0) ? x0 : ac_round((y-c)/m);
}


void aldrin_draw_line(Aldrin_Canvas ac, uint32_t x0, uint32_t y0,
        uint32_t x1, uint32_t y1, uint32_t color) {
    if (ac_abs(y0-y1) < ac_abs(x0-x1)) {
        // horizontal-ish line
        if (x0 > x1) ac_swap(&x0, &y0, &x1, &y1);
        for (int xi = x0; xi <= x1; ++xi) {
            int yi = ac_calc_y_on_line(x0, y0, x1, y1, xi);
            aldrin_put_pixel(ac, xi, yi, color);
        }

    } else {
        // vertical-ish line
        if (y0 > y1) ac_swap(&x0, &y0, &x1, &y1);
        for (int yi = y0; yi <= y1; ++yi) {
            int xi = ac_calc_x_on_line(x0, y0, x1, y1, yi);
            aldrin_put_pixel(ac, xi, yi, color);
        }
    }
}


void aldrin_fill_triangle(Aldrin_Canvas ac, uint32_t x0, uint32_t y0,
        uint32_t x1, uint32_t y1, uint32_t x2, uint32_t y2, uint32_t color) {
    // scanline triangle fill algorthim
    // make sure y0 < y1 < y2
    if (y1 > y2) ac_swap(&x1, &y1, &x2, &y2);
    if (y0 > y1) ac_swap(&x0, &y0, &x1, &y1);
    if (y1 > y2) ac_swap(&x1, &y1, &x2, &y2);

    // scan from y0 to y1
    for (int yi = y0; yi <= y1; ++yi) {
        int xi0 = ac_calc_x_on_line(x0, y0, x2, y2, yi);
        int xi1 = ac_calc_x_on_line(x0, y0, x1, y1, yi);
        aldrin_draw_line(ac, xi0, yi, xi1, yi, color);
    }

    // scan from y1 to y2
    for (int yi = y1; yi <= y2; ++yi) {
        int xi0 = ac_calc_x_on_line(x0, y0, x2, y2, yi);
        int xi1 = ac_calc_x_on_line(x1, y1, x2, y2, yi);
        aldrin_draw_line(ac, xi0, yi, xi1, yi, color);
    }
}


void aldrin_draw_ellipse(Aldrin_Canvas ac, uint32_t x, uint32_t y,
        uint32_t r0, uint32_t r1, uint32_t color) {
    // scanline ellipse draw algorithm
    // center coordinates
    int xc = x+r0; int yc = y+r1;

    if (r1 == 0) {
        aldrin_put_pixel(ac, x, y, color);
    } else {
        for (int yi = y; yi <= y+r1; ++yi) {
            // solve for (x-xc) from eliipse formula
            double root = sqrt(pow(r0, 2)*(1-pow((double)(yi-yc)/r1, 2)));
            int xi0 = round(root + xc);
            // do for top right quadrant
            aldrin_put_pixel(ac, xi0, yi, color);
            // repeat for other quadrants
            aldrin_put_pixel(ac, 2*xc-xi0, yi, color);
            aldrin_put_pixel(ac, xi0, 2*yc-yi, color);
            aldrin_put_pixel(ac, 2*xc-xi0, 2*yc-yi, color);
        }
    }

    if (r0 == 0) {
        aldrin_put_pixel(ac, x, y, color);
    } else {
        for (int xi = xc; xi <= xc+r0; ++xi) {
            // solve for (y-yc) from eliipse formula
            double root = sqrt(pow(r1, 2)*(1-pow((double)(xi-xc)/r0, 2)));
            int yi1 = round(-1*root + yc);
            // do for top right quadrant
            aldrin_put_pixel(ac, xi, yi1, color);
            // repeat for other quadrants
            aldrin_put_pixel(ac, 2*xc-xi, yi1, color);
            aldrin_put_pixel(ac, xi, 2*yc-yi1, color);
            aldrin_put_pixel(ac, 2*xc-xi, 2*yc-yi1, color);
        }
    }
}


void aldrin_fill_ellipse(Aldrin_Canvas ac, uint32_t x, uint32_t y,
        uint32_t r0, uint32_t r1, uint32_t color) {
    // scanline ellipse fill algorithm
    // center coordinates
    int xc = x+r0; int yc = y+r1;

    if (r1 == 0) {
        aldrin_put_pixel(ac, x, y, color);
    } else {
        for (int yi = y; yi <= y+r1; ++yi) {
            // solve for (x-xc) from eliipse formula
            double root = sqrt(pow(r0, 2)*(1-pow((double)(yi-yc)/r1, 2)));
            int xi0 = round(root + xc);
            // do for top right quadrant
            aldrin_draw_line(ac, xc, yi, xi0, yi, color);
            // repeat for other quadrants
            aldrin_draw_line(ac, 2*xc-xi0, yi, xc, yi, color);
            aldrin_draw_line(ac, xc, 2*yc-yi, xi0, 2*yc-yi, color);
            aldrin_draw_line(ac, 2*xc-xi0, 2*yc-yi, xc, 2*yc-yi, color);
        }
    }

    if (r0 == 0) {
        aldrin_put_pixel(ac, x, y, color);
    } else {
        for (int xi = xc; xi <= xc+r0; ++xi) {
            // solve for (y-yc) from eliipse formula
            double root = sqrt(pow(r1, 2)*(1-pow((double)(xi-xc)/r0, 2)));
            int yi1 = round(-1*root + yc);
            // do for top right quadrant
            aldrin_draw_line(ac, xi, yi1, xi, yc, color);
            // repeat for other quadrants
            aldrin_draw_line(ac, 2*xc-xi, yi1, 2*xc-xi, yc, color);
            aldrin_draw_line(ac, xi, 2*yc-yi1, xi, yc, color);
            aldrin_draw_line(ac, 2*xc-xi, 2*yc-yi1, 2*xc-xi, yc, color);
        }
    }
}


uint32_t *aldrin_get_pixels(Aldrin_Canvas ac) { return ac.pixels; }


uint32_t aldrin_get_width(Aldrin_Canvas ac) { return ac.width; }


uint32_t aldrin_get_height(Aldrin_Canvas ac) { return ac.height; }


#ifndef PLATFORM_WASM
int aldrin_save_ppm(Aldrin_Canvas ac, const char *filename) {
    FILE *f = fopen(filename, "wb");
    if (f == NULL) return 1;

    fprintf(f, "P3\n%d %d 255\n", ac.width, ac.height);

    for (uint32_t p = 0; p < ac.width * ac.height; ++p) {
        const uint8_t r = (ac.pixels[p] >> 16) & 0xff;
        const uint8_t g = (ac.pixels[p] >> 8) & 0xff;
        const uint8_t b = ac.pixels[p] & 0xff;
        fprintf(f, "%d %d %d\n", r, g, b);
    }

    fclose(f);
    return 0;
}
#endif // PLATFORM_WASM

#endif // ALDRIN_H_