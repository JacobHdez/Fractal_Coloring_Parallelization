#include "rgb.h"

RGB::RGB(void)
    : r(0), g(0), b(0) {}
RGB::RGB(double c)
    : r(c), g(c), b(c) {}
RGB::RGB(double _r, double _g, double _b)
    : r(_r), g(_g), b(_b) {}

bool RGB::operator != (const RGB &c)
{ return c.r != r && c.g != g && c.b != b; }
RGB& RGB::operator += (const RGB &rgb)
{ r += rgb.r, g += rgb.g, b += rgb.b; return *this; }
RGB& RGB::operator *= (const RGB &rgb)
{ r *= rgb.r, g *= rgb.g, b *= rgb.b; return *this; }
