#include "headers/rgb.h"
#include "headers/image.h"
#include "headers/colour_palette.h"
#include <iostream>
#include <vector>
#include <cmath>
#include <ctime>
using namespace std;



int main(int argc, char const *argv[])
{
    clock_t begin, end;
    double elapsed_secs;

    unsigned orbit_trap = 2;
    unsigned max_iteration = 512;
    double X_intv[] = {-2.5, 1.0};
    double Y_intv[] = {-1.0, 1.0};
    unsigned pixels_X = 15360; // 240 480 960 1920 3840 7680 15360
    unsigned pixels_Y = 8640; // 135 270 540 1080 2160 4320 8640

    colour_palette rgb_colour(0, max_iteration);//(0, M_PI / 2.0);
    rgb_colour.set_points("./../source/colour_palette.txt", true);
    // rgb_colour.plot_RGBspace("./../results/RGB_points.txt", 1000);
    // rgb_colour.plot_palette("./../results/RGB_palette.ppm");

    Image fractal(pixels_X, pixels_Y);
    double jump_x = (X_intv[1] - X_intv[0]) / (double)(pixels_X - 1);
    double jump_y = (Y_intv[1] - Y_intv[0]) / (double)(pixels_Y - 1);

    double z_re, z_im, aux;
    double c_re, c_im;
    unsigned i, j, iteration;

    begin = clock();
    for (i = 0; i < pixels_Y; ++i) {
        for (j = 0; j < pixels_X; ++j) {
            z_re = z_im = 0;
            c_re = X_intv[0] + jump_x * j;
            c_im = Y_intv[0] + jump_y * i;
            iteration = 0;

            while ((z_re*z_re + z_im*z_im) <= orbit_trap && iteration < max_iteration) {
                aux = z_re*z_re - z_im*z_im + c_re;
                z_im = 2*z_re*z_im + c_im;
                z_re = aux;

                iteration++;
            }

            if (iteration == max_iteration)
                fractal.pixels[i * pixels_X + j] = kBlack;
            else
                fractal.pixels[i * pixels_X + j] = rgb_colour((double)iteration);
        }
    }
    end = clock();
    elapsed_secs = (double)(end - begin) / CLOCKS_PER_SEC;
    cout << elapsed_secs << " sec" << endl;

    // savePPM(fractal, "./../results/fractal.ppm");
    return 0;
}
