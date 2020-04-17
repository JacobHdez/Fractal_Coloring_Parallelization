#include "headers/rgb.h"
#include "headers/image.h"
#include <iostream>
using namespace std;

int main(int argc, char const *argv[])
{
    cout << "Hello, World!" << endl;

    RGB fondo(0.5, 0.25, 0.25);
    Image test(128, 128, fondo);

    savePPM(test, "../results/test_img.ppm");

    cout << "Hello, World!" << endl;
    return 0;
}
