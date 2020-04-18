#include "image.h"
#include "rgb.h"

#include <cstdio>
#include <fstream>
#include <cstring>
#include <cassert>


Image::Image(void)
    : w(0), h(0), pixels(nullptr)
{ /* empty image */ }
Image::Image(const unsigned &_w, const unsigned &_h, const RGB &c)
    : w(_w), h(_h), pixels(nullptr)
{
    pixels = new RGB[w * h];
    for (int i = 0; i < w * h; ++i) pixels[i] = c;
}
Image::Image(const Image &img)
    : w(img.w), h(img.h), pixels(nullptr)
{
    pixels = new RGB[w * h];
    memcpy(pixels, img.pixels, sizeof(RGB) * w * h);
}
Image::Image(Image &&img)
    : w(0), h(0), pixels(nullptr)
{
    w = img.w;
    h = img.h;
    pixels = img.pixels;
    img.pixels = nullptr;
    img.w = img.h = 0;
}
Image::~Image(void)
{ if (pixels != nullptr) delete[] pixels; }

Image& Image::operator = (Image &&img)
{
    if (this != &img) {
        if (pixels != nullptr) delete[] pixels;
        w = img.w, h = img.h;
        pixels = img.pixels;
        img.pixels = nullptr;
        img.w = img.h = 0;
    }
    return *this;
}
RGB& Image::operator () (const unsigned &x, const unsigned &y) const
{
    assert(x < w && y < h);
    return pixels[y * w + x];
}
const RGB& Image::operator [] (const unsigned &i) const
{ return pixels[i]; }
RGB& Image::operator [] (const unsigned &i)
{ return pixels[i]; }


void savePPM(const Image &img, const char *filename)
{
    if (img.w == 0 || img.h == 0) { fprintf(stderr, "Can't save an empty image\n"); return; }
    std::ofstream ofs;
    try {
        ofs.open(filename, std::ios::binary);
        if (ofs.fail()) throw("Can't open output file");
        ofs << "P6\n" << img.w << " " << img.h << "\n255\n";
        unsigned char r, g, b;
        for (int i = 0; i < img.w * img.h; ++i) {
            r = static_cast<unsigned char>(std::min(1.0, img.pixels[i].r) * 255);
            g = static_cast<unsigned char>(std::min(1.0, img.pixels[i].g) * 255);
            b = static_cast<unsigned char>(std::min(1.0, img.pixels[i].b) * 255);
            ofs << r << g << b;
        }
        ofs.close();
    }
    catch (const char *err) {
        fprintf(stderr, "%s\n", err);
        ofs.close();
    }
}
