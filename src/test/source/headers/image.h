#ifndef __IMAGE_H__
#define __IMAGE_H__

#include "rgb.h"

class Image
{
public:

    Image(void);
    Image(const unsigned &_w, const unsigned &_h, const RGB &c = kBlack);
    Image(const Image &img);
    Image(Image &&img); // move constructor
    ~Image(void);

    Image& operator = (Image &&img); // move assignment operator
    RGB& operator () (const unsigned &x, const unsigned &y) const;
    const RGB& operator [] (const unsigned &i) const;
    RGB& operator [] (const unsigned &i);

    unsigned w, h;
    RGB *pixels;
};

void savePPM(const Image &img, const char *filename);

#endif // __IMAGE_H__
