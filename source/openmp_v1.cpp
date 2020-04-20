#include "headers/rgb.h"
#include "headers/image.h"
#include "headers/colour_palette.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cmath>
#include <ctime>
#include <omp.h>
using namespace std;

bool init(
    const char *filename,
    string &colour_palette_filename,
    double &colour_palette_min, double &colour_palette_max,
    bool &cubic_spline,
    string &RGBspace_filename, unsigned &RGBspace_partition,
    string &RGBpalette_filename,
    unsigned &img_width, unsigned &img_height, string &img_filename,
    double &real_min, double &real_max, double &imaginary_min, double &imaginary_max,
    unsigned &max_iteration
);

int main(int argc, char const *argv[])
{
    if (argc < 2) {
        cout << "Usage: " << argv[0] << " init_params.file" << endl;
        return 1;
    }

    clock_t global_begin, global_end;
    clock_t fractal_begin, fractal_end;
    double global_secs, fractal_secs;

    global_begin = clock();

    // Colour palette parameters
    string colour_palette_filename;
    double colour_palette_min, colour_palette_max;
    bool cubic_spline;
    string RGBspace_filename;
    unsigned RGBspace_partition;
    string RGBpalette_filename;

    // Image parameters
    unsigned img_width, img_height;
    string img_filename;
    double real_min, real_max, imaginary_min, imaginary_max;
    unsigned max_iteration;

    if (!init(
        argv[1],
        colour_palette_filename,
        colour_palette_min, colour_palette_max,
        cubic_spline,
        RGBspace_filename, RGBspace_partition,
        RGBpalette_filename,
        img_width, img_height, img_filename,
        real_min, real_max, imaginary_min, imaginary_max,
        max_iteration
    )) return 1;

    // Definimos el espacio RGB
    colour_palette RGBspace(colour_palette_min, colour_palette_max);
    RGBspace.set_points(colour_palette_filename.c_str(), cubic_spline);
    // RGBspace.plot_RGBspace(RGBspace_filename.c_str(), RGBspace_partition);
    // RGBspace.plot_palette(RGBpalette_filename.c_str());

    Image fractal(img_width, img_height);

    unsigned x, y, iteration;
    double range_width = (real_max - real_min) / (double)(img_width - 1);
    double range_height = (imaginary_max - imaginary_min) / (double)(img_height - 1);
    double zr, zi, cr, ci, aux;

    fractal_begin = clock();

    #pragma omp parallel for ordered schedule(dynamic)
    for (y = 0; y < img_height; ++y) {
        for (x = 0;  x < img_width; ++x) {
            zr = zi = 0.0;
            cr = x * range_width + real_min;
            ci = y * range_height + imaginary_min;

            iteration = 0;
            while (iteration < max_iteration && (zr * zr + zi * zi) < 4.0) {
                aux = zr * zr - zi * zi + cr;
                zi = fabs(2 * zr * zi + ci);
                zr = fabs(aux);
                iteration++;
            }

            #pragma omp ordered
            {
                if (iteration == max_iteration)
                    fractal.pixels[y * img_width + x] = kWhite;
                else
                    fractal.pixels[y * img_width + x] = RGBspace((double)iteration);
            }
        }
    }

    fractal_end = clock();

    // savePPM(fractal, img_filename.c_str());
    global_end = clock();

    fractal_secs = (fractal_end - fractal_begin) / (double)CLOCKS_PER_SEC;
    global_secs = (global_end - global_begin) / (double)CLOCKS_PER_SEC;

    cout << global_secs << "\t" << fractal_secs << endl;
    return 0;
}

bool init(
    const char *filename,
    string &colour_palette_filename,
    double &colour_palette_min, double &colour_palette_max,
    bool &cubic_spline,
    string &RGBspace_filename, unsigned &RGBspace_partition,
    string &RGBpalette_filename,
    unsigned &img_width, unsigned &img_height, string &img_filename,
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

        getline(ifs, RGBspace_filename);
        getline(ifs, line); RGBspace_partition = stoi(line);
        getline(ifs, RGBpalette_filename);

        getline(ifs, line); img_width = stoi(line);
        getline(ifs, line); img_height = stoi(line);
        getline(ifs, img_filename);

        getline(ifs, line); real_min = stod(line);
        getline(ifs, line); real_max = stod(line);
        getline(ifs, line); imaginary_min = stod(line);
        getline(ifs, line); imaginary_max = stod(line);

        getline(ifs, line); max_iteration = stoi(line);

        img_filename = img_filename + "fractal_" + to_string(img_width) + 'x' + to_string(img_height) + '_' + to_string(max_iteration) + ".ppm";

        ifs.close();
        return true;
    }
    catch (const char *err) {
        cout << err << "\n";
        ifs.close();
        return false;
    }
}
