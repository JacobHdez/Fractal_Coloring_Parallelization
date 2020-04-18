#include "headers/rgb.h"
#include "headers/image.h"
#include "headers/colour_palette.h"
#include <iostream>
#include <vector>
#include <cmath>
using namespace std;

int main(int argc, char const *argv[])
{
    int n_points = 3;
    double r[] = {25, 50, 75, 25};
    double g[] = {75, 50, 25, 75};
    double b[] = {150, 50, 0, 150};
    for (unsigned i = 0; i < n_points; ++i) {
        r[i] /= 255.0;
        g[i] /= 255.0;
        b[i] /= 255.0;
    }

    vector<double> red(begin(r), end(r));
    vector<double> green(begin(g), end(g));
    vector<double> blue(begin(b), end(b));

    colour_palette rgb_colour(0, M_PI / 2.0);
    rgb_colour.set_points(red, green, blue);

    RGB c;
    c = rgb_colour(0.5);
    cout << c << endl;

    rgb_colour.set_points("./../source/colour_palette.txt");

    c = rgb_colour(0.5);
    cout << c << endl;

    return 0;
}
