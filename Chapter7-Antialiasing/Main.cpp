#include "RTWeekend.h"

#include "Color.h"
#include "HittableList.h"
#include "Sphere.h"
#include "Camera.h"

Color RayColor(const Ray& InRay, const HittableList& World) 
{
    // Object
    HitRecord Record;
    if(World.Hit(InRay, 0.0f, Infinity, Record))
    {
        return 0.5f * (Record.Normal + Color(1.0f, 1.0f, 1.0f));
    }

    // Background
    Vector3 UnitDirection = UnitVector(InRay.GetDirection());
    auto t = 0.5f * (UnitDirection.Y() + 1.0f); // [-1.0, 1.0] -> [0.0, 1.0]
    return (1.0f - t) * Color(1.0f, 1.0f, 1.0f) + t * Color(0.5f, 0.7f, 1.0f); // t:[0.0, 1.0] -> [White, Blue]
}

int main()
{
    // Image

    const auto AspectRation = 16.0f / 9.0f;
    const int ImageWidth = 400;
    const int ImageHeight = static_cast<int>(ImageWidth / AspectRation);
    const int SamplesPerPixel = 100;

    // World

    HittableList World;
    World.Add(make_shared<Sphere>(Point3(0.0f, 0.0f, -1.0f), 0.5f));
    World.Add(make_shared<Sphere>(Point3(0.0f, -100.5f, -1.0f), 100.0f));

    // Camera

    Camera Cam;

    // Render

    std::cout << "P3\n" << ImageWidth << ' ' << ImageHeight << "\n255\n";

    for (int j = ImageHeight - 1; j >= 0; --j) 
    {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < ImageWidth; ++i) 
        {
            Color PixelColor(0.0f, 0.0f, 0.0f);
            // Do antialiasing by random super sampling
            for (int s = 0; s < SamplesPerPixel; ++s) 
            {
                auto u = (i + RandomFloat()) / (ImageWidth - 1);
                auto v = (j + RandomFloat()) / (ImageHeight - 1);
                Ray r = Cam.GetRay(u, v);
                PixelColor += RayColor(r, World);
            }
            WriteColor(std::cout, PixelColor, SamplesPerPixel);
        }
    }

    std::cerr << "\nDone.\n";
}