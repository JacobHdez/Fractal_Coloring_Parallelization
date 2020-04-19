#include "colour_palette.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <cassert>
#include <exception>
#include "rgb.h"

colour_palette::colour_palette(void)
    : left(0), right(1) {}
colour_palette::colour_palette(double _left, double _right)
    : left(_left), right(_right) {}

void colour_palette::set_points(const std::vector<double> &r, const std::vector<double> &g, const std::vector<double> &b, bool cubic_spline)
{
    assert(r.size() >= 2);
    assert(r.size() == g.size());
    assert(r.size() == b.size());

    std::vector<double> x(r.size());

    unsigned lenght = x.size();

    double jump = (right - left) / (lenght - 1);

    for (unsigned i = 0; i < lenght; ++i)
        x[i] = i * jump;

    s_r.set_points(x, r, cubic_spline);
    s_g.set_points(x, g, cubic_spline);
    s_b.set_points(x, b, cubic_spline);
}
void colour_palette::set_points(const char *filename, bool cubic_spline)
{
    std::ifstream ifs(filename);
    try {
        if (ifs.fail()) { throw("Can't open input file"); }

        unsigned n;
        ifs >> n;
        assert(n >= 2);

        std::vector<double> r, g, b;

        double aux_r, aux_g, aux_b;
        while (n--) {
            ifs >> aux_r >> aux_g >> aux_b;
            r.push_back(aux_r / 255.0);
            g.push_back(aux_g / 255.0);
            b.push_back(aux_b / 255.0);
        }
        assert(r.size() == g.size());
        assert(r.size() == b.size());

        ifs.close();

        std::vector<double> x(r.size());

        unsigned lenght = x.size();

        double jump = (right - left) / (lenght - 1);

        for (unsigned i = 0; i < lenght; ++i)
            x[i] = i * jump;

        s_r.set_points(x, r, cubic_spline);
        s_g.set_points(x, g, cubic_spline);
        s_b.set_points(x, b, cubic_spline);
    }
    catch (std::exception &e) {
        std::cout << e.what() << "\n";
        ifs.close();
    }
}

RGB colour_palette::operator () (double x) const
{
    unsigned div = x / (right - left);
    if (div != 0)
        x -= div * (right - left);

    RGB colour(s_r(x), s_g(x), s_b(x));
    return colour;
}

void colour_palette::plot_RGBspace(const char *filename, unsigned partition)
{
    assert(partition >= 2);
    std::ofstream ofs(filename);
    try {
        if (ofs.fail()) throw("Can't open output file");

        double jump = (right - left) / (partition - 1), x;
        for (unsigned i = 0; i < partition; ++i) {
            x = i * jump;
            ofs << x << " " << s_r(x)
                     << "  " << s_g(x)
                     << "  " << s_b(x) << "\n";
        }
        ofs.close();
    }
    catch (std::exception &e) {
        std::cout << e.what() << "\n";
        ofs.close();
    }
}
