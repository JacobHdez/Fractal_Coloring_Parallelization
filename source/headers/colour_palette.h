#ifndef __COLOUR_PALETTE_H__
#define __COLOUR_PALETTE_H__

#include "spline.h"
#include <vector>
#include "rgb.h"

struct colour_palette
{
    colour_palette(void);
    colour_palette(double _left, double _right);

    void set_points(const std::vector<double> &r, const std::vector<double> &g, const std::vector<double> &b, bool cubic_spline = true);
    void set_points(const char *filename, bool cubic_spline = true);

    RGB operator () (double x) const;

    void plot_RGBspace(const char *filename, unsigned partition = 100);

    double left, right;
    tk::spline s_r, s_g, s_b;
};

#endif
