#include "headers/rgb.h"
#include "headers/image.h"
#include "headers/colour_palette.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cmath>
#include <ctime>
using namespace std;

bool init(
    const char *filename,
    string &colour_palette_filename,
    double &colour_palette_min, double &colour_palette_max,
    bool &cubic_spline,
    string &RGBspace_file, unsigned &RGBspace_partition,
    string &RGBpalette_file,
    unsigned &img_width, unsigned &img_height, string &img_path,
    double &real_min, double &real_max, double &imaginary_min, double &imaginary_max,
    unsigned &max_iteration
);

int main(int argc, char const *argv[])
{
    if (argc < 2) {
        cout << "Use: " << argv[0] << " init_params.file" << endl;
        return 1;
    }

    // Colour palette parameters
    string colour_palette_filename;
    double colour_palette_min, colour_palette_max;
    bool cubic_spline;
    string RGBspace_file;
    unsigned RGBspace_partition;
    string RGBpalette_file;

    // Image parameters
    unsigned img_width, img_height;
    string img_path;
    double real_min, real_max, imaginary_min, imaginary_max;
    unsigned max_iteration;

    cout << init(argv[1],
         colour_palette_filename,
         colour_palette_min, colour_palette_max,
         cubic_spline,
         RGBspace_file, RGBspace_partition,
         RGBpalette_file,
         img_width, img_height, img_path,
         real_min, real_max, imaginary_min, imaginary_max,
         max_iteration) << endl;

    cout << colour_palette_filename << endl;
    cout << colour_palette_min << endl;
    cout << colour_palette_max << endl;
    cout << cubic_spline << endl;
    cout << RGBspace_file << endl;
    cout << RGBspace_partition << endl;
    cout << RGBpalette_file << endl;

    cout << img_width << endl;
    cout << img_height << endl;
    cout << img_path << endl;
    cout << real_min << endl;
    cout << real_max << endl;
    cout << imaginary_min << endl;
    cout << imaginary_max << endl;
    cout << max_iteration << endl;

    // clock_t begin, end;
    // double elapsed_secs;
    //
    // unsigned orbit_trap = 2;
    // unsigned max_iteration = 512;
    // double X_intv[] = {-2.5, 1.0};
    // double Y_intv[] = {-1.0, 1.0};
    // unsigned pixels_X = 15360; // 240 480 960 1920 3840 7680 15360
    // unsigned pixels_Y = 8640; // 135 270 540 1080 2160 4320 8640
    //
    // colour_palette rgb_colour(0, max_iteration);//(0, M_PI / 2.0);
    // rgb_colour.set_points("./../source/colour_palette.txt", true);
    // // rgb_colour.plot_RGBspace("./../results/RGB_points.txt", 1000);
    // // rgb_colour.plot_palette("./../results/RGB_palette.ppm");
    //
    // Image fractal(pixels_X, pixels_Y);
    // double jump_x = (X_intv[1] - X_intv[0]) / (double)(pixels_X - 1);
    // double jump_y = (Y_intv[1] - Y_intv[0]) / (double)(pixels_Y - 1);
    //
    // double z_re, z_im, aux;
    // double c_re, c_im;
    // unsigned i, j, iteration;
    //
    // begin = clock();
    // for (i = 0; i < pixels_Y; ++i) {
    //     for (j = 0; j < pixels_X; ++j) {
    //         z_re = z_im = 0;
    //         c_re = X_intv[0] + jump_x * j;
    //         c_im = Y_intv[0] + jump_y * i;
    //         iteration = 0;
    //
    //         while ((z_re*z_re + z_im*z_im) <= orbit_trap && iteration < max_iteration) {
    //             aux = z_re*z_re - z_im*z_im + c_re;
    //             z_im = 2*z_re*z_im + c_im;
    //             z_re = aux;
    //
    //             iteration++;
    //         }
    //
    //         if (iteration == max_iteration)
    //             fractal.pixels[i * pixels_X + j] = kBlack;
    //         else
    //             fractal.pixels[i * pixels_X + j] = rgb_colour((double)iteration);
    //     }
    // }
    // end = clock();
    // elapsed_secs = (double)(end - begin) / CLOCKS_PER_SEC;
    // cout << elapsed_secs << " sec" << endl;
    //
    // // savePPM(fractal, "./../results/fractal.ppm");
    return 0;
}

bool init(
    const char *filename,
    string &colour_palette_filename,
    double &colour_palette_min, double &colour_palette_max,
    bool &cubic_spline,
    string &RGBspace_file, unsigned &RGBspace_partition,
    string &RGBpalette_file,
    unsigned &img_width, unsigned &img_height, string &img_path,
    double &real_min, double &real_max, double &imaginary_min, double &imaginary_max,
    unsigned &max_iteration
)
{
    ifstream ifs(filename);
    string line;
    try {
        if (ifs.fail()) throw("Can't open input file");

        getline(ifs, colour_palette_filename);

        getline(ifs, line);
        if (line.compare("true") == 0) {
            getline(ifs, line); getline(ifs, line);
            colour_palette_min = 0;
            colour_palette_max = M_PI / 2.0;
        }
        else if (line.compare("false") == 0) {
            getline(ifs, line); colour_palette_min = stod(line);
            getline(ifs, line); colour_palette_max = stod(line);
        }
        else throw("WoW");

        getline(ifs, line);
        if (line.compare("true") == 0) cubic_spline = true;
        else if (line.compare("false") == 0) cubic_spline = false;
        else throw("WoW");

        getline(ifs, RGBspace_file);
        getline(ifs, line); RGBspace_partition = stoi(line);
        getline(ifs, RGBpalette_file);


        getline(ifs, line); img_width = stoi(line);
        getline(ifs, line); img_height = stoi(line);
        getline(ifs, img_path);

        getline(ifs, line); real_min = stod(line);
        getline(ifs, line); real_max = stod(line);
        getline(ifs, line); imaginary_min = stod(line);
        getline(ifs, line); imaginary_max = stod(line);

        getline(ifs, line); max_iteration = stoi(line);

        ifs.close();
        return true;
    }
    catch (const char *err) {
        cout << err << "\n";
        ifs.close();
        return false;
    }
}
