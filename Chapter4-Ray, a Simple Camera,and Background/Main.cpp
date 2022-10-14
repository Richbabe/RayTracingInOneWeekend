#include "Color.h"
#include "Ray.h"
#include "Vector3.h"

#include <iostream>

Color RayColor(const Ray& r) {
    Vector3 UnitDirection = UnitVector(r.GetDirection());
    auto t = 0.5f * (UnitDirection.Y() + 1.0f); // [-1.0, 1.0] -> [0.0, 1.0]
    return (1.0f - t) * Color(1.0f, 1.0f, 1.0f) + t * Color(0.5f, 0.7f, 1.0f); // t:[0.0, 1.0] -> [White, Blue]
}

int main()
{
    // Image

    const auto AspectRation = 16.0f / 9.0f;
    const int ImageWidth = 400;
    const int ImageHeight = static_cast<int>(ImageWidth / AspectRation);

    // Camera

    auto ViewportHeight = 2.0f;
    auto ViewportWidth = AspectRation * ViewportHeight;
    auto FocalLength = 1.0f;

    auto Origin = Point3(0.0f, 0.0f, 0.0f);
    auto Horizontal = Vector3(ViewportWidth, 0.0f, 0.0f);
    auto Vertical = Vector3(0.0f, ViewportHeight, 0.0f);
    auto LowerLeftCorner = Origin - Horizontal / 2.0f - Vertical / 2.0f - Vector3(0.0f, 0.0f, FocalLength);

    // Render

    std::cout << "P3\n" << ImageWidth << ' ' << ImageHeight << "\n255\n";

    for (int j = ImageHeight - 1; j >= 0; --j) 
    {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < ImageWidth; ++i) 
        {
            auto u = float(i) / (ImageWidth - 1);
            auto v = float(j) / (ImageHeight - 1);
            Ray r(Origin, LowerLeftCorner + u * Horizontal + v * Vertical - Origin);
            Color PixelColor = RayColor(r);

            WriteColor(std::cout, PixelColor);
        }
    }

    std::cerr << "\nDone.\n";
}