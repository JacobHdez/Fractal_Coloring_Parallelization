#ifndef __RGB_H__
#define __RGB_H__

struct RGB
{
    RGB(void);
    RGB(double c);
    RGB(double _r, double _g, double _b);

    bool operator != (const RGB &c);
    RGB& operator += (const RGB &rgb);
    RGB& operator *= (const RGB &rgb);

    double r, g, b;
};

static const RGB kBlack = RGB(0);
static const RGB kWhite = RGB(1);
static const RGB kRed = RGB(1, 0, 0);
static const RGB kGreen = RGB(0, 1, 0);
static const RGB kBlue = RGB(0, 0, 1);

#endif // __RGB_H__
