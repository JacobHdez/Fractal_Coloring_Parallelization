#include <iostream>

#include "TestConfig.h"
#include <string>

#ifdef USE_HEADERS
    #include "rgb.h"
    #include "image.h"
#endif

using namespace std;

int main(int argc, char const *argv[])
{
    if (argc < 2) {
        cout << argv[0] << " Version " << Test_VERSION_MAJOR << "."
             << Test_VERSION_MINOR << endl;
        cout << "Usage: " << argv[0] << " number" << endl;
        return 1;
    }
    const double inputValue = stod(argv[1]);
    cout << inputValue << endl;

    cout << "Hello, World!" << endl;


    #ifdef USE_HEADERS
        RGB fondo(0.5, 0.75, 0.25);
        Image test(4096, 2160, fondo);

        savePPM(test, "../test_img.ppm");
    #endif

    cout << "Hello, World!" << endl;
    return 0;
}
