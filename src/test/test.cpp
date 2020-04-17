#include <iostream>

#include "rgb.h"
#include "image.h"

using namespace std;

int main(int argc, char const *argv[])
{
    cout << "Hello, World!" << endl;

    RGB fondo(0.5, 0.75, 0.25);
    Image test(4096, 2160, fondo);

    savePPM(test, "../test_img.ppm");

    cout << "Hello, World!" << endl;
    return 0;
}
