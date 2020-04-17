#include "rgb.h"

RGB::RGB(void)
    : r(0), g(0), b(0) {}
RGB::RGB(float c)
    : r(c), g(c), b(c) {}
RGB::RGB(float _r, float _g, float _b)
    : r(_r), g(_g), b(_b) {}

bool RGB::operator != (const RGB &c)
{ return c.r != r && c.g != g && c.b != b; }
RGB& RGB::operator += (const RGB &rgb)
{ r += rgb.r, g += rgb.g, b += rgb.b; return *this; }
RGB& RGB::operator *= (const RGB &rgb)
{ r *= rgb.r, g *= rgb.g, b *= rgb.b; return *this; }
