#include "RTWeekend.h"

#include "Color.h"
#include "HittableList.h"
#include "Sphere.h"
#include "Camera.h"
#include "Material.h"

Color RayColor(const Ray& InRay, const HittableList& World, int Depth) 
{
    // If we've exceeded the ray bounce limit, no more light is gathered.
    if (Depth <= 0)
    {
        return Color(0.0f, 0.0f, 0.0f);
    }
        
    // Object
    HitRecord Record;
    if(World.Hit(InRay, 0.001f, Infinity, Record)) // Use 0.001f as t to instead of 0.0f to avoid shadow acne cause by super near intersection
    {
        Ray Scattered;
        Color Attenuation;
        if(Record.Material->Scatter(InRay, Record, Attenuation, Scattered))
        {
            return Attenuation * RayColor(Scattered, World, Depth - 1);
        }
        return Color(0.0f, 0.0f, 0.0f);
    }

    // Background
    Vector3 UnitDirection = UnitVector(InRay.GetDirection());
    auto t = 0.5f * (UnitDirection.Y() + 1.0f); // [-1.0, 1.0] -> [0.0, 1.0]
    return (1.0f - t) * Color(1.0f, 1.0f, 1.0f) + t * Color(0.5f, 0.7f, 1.0f); // t:[0.0, 1.0] -> [White, Blue]
}

int main()
{
    // Image

    const auto AspectRatio = 16.0f / 9.0f;
    const int ImageWidth = 400;
    const int ImageHeight = static_cast<int>(ImageWidth / AspectRatio);
    const int SamplesPerPixel = 100;
    const int MaxDepth = 50;

    // World
    auto R = cos(PI / 4.0f);
    HittableList World;

    auto MaterialGround = make_shared<Lambertian>(Color(0.8f, 0.8f, 0.0f));
    auto MaterialCenter = make_shared<Lambertian>(Color(0.1f, 0.2f, 0.5f));
    auto MaterialLeft = make_shared<Dielectric>(1.5f);
    auto MaterialRight = make_shared<Metal>(Color(0.8f, 0.6f, 0.2f), 0.0f);

    World.Add(make_shared<Sphere>(Point3(0.0f, -100.5f, -1.0f), 100.0f, MaterialGround));
    World.Add(make_shared<Sphere>(Point3(0.0f, 0.0f, -1.0f), 0.5f, MaterialCenter));
    World.Add(make_shared<Sphere>(Point3(-1.0f, 0.0f, -1.0f), 0.5f, MaterialLeft));
    World.Add(make_shared<Sphere>(Point3(-1.0f, 0.0f, -1.0f), -0.4f, MaterialLeft)); // make a hollow glass sphere in the big glass sphere
    World.Add(make_shared<Sphere>(Point3(1.0f, 0.0f, -1.0f), 0.5f, MaterialRight));

    // Camera

    Camera Cam(Point3(-2.0f, 2.0f, 1.0f), Point3(0.0f, 0.0f, -1.0f), Vector3(0.0f, 1.0f, 0.0f), 20.0f, AspectRatio);

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
                PixelColor += RayColor(r, World, MaxDepth);
            }
            WriteColor(std::cout, PixelColor, SamplesPerPixel);
        }
    }

    std::cerr << "\nDone.\n";
}