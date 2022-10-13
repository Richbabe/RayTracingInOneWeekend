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
            auto r = float(i) / (ImageWidth - 1);
            auto g = float(j) / (ImageHeight - 1);
            auto b = 0.25f;

            int ir = static_cast<int>(255.999f * r);
            int ig = static_cast<int>(255.999f * g);
            int ib = static_cast<int>(255.999f * b);

            std::cout << ir << ' ' << ig << ' ' << ib << '\n';
        }
    }

    std::cerr << "\nDone.\n";
}