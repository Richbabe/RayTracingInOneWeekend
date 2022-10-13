#include "Color.h"
#include "Vector3.h"

#include <iostream>

int main()
{
    // Image

    const int ImageWidth = 256;
    const int ImageHeight = 256;

    // Render

    std::cout << "P3\n" << ImageWidth << ' ' << ImageHeight << "\n255\n";

    for (int j = ImageHeight - 1; j >= 0; --j) 
    {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < ImageWidth; ++i) 
        {
            Color PixelColor(float(i) / (ImageWidth - 1), float(j) / (ImageHeight - 1), 0.25f);
            WriteColor(std::cout, PixelColor);
        }
    }

    std::cerr << "\nDone.\n";
}