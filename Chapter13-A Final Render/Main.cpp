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

HittableList RandomScene() 
{
    HittableList World;

    auto GroundMaterial = make_shared<Lambertian>(Color(0.5f, 0.5f, 0.5f));
    World.Add(make_shared<Sphere>(Point3(0.0f, -1000.0f, 0.0f), 1000.0f, GroundMaterial));

    for (int a = -11; a < 11; a++) 
    {
        for (int b = -11; b < 11; b++) 
        {
            auto ChooseMat = RandomFloat();
            Point3 Center(a + 0.9f * RandomFloat(), 0.2f, b + 0.9f * RandomFloat());

            if ((Center - Point3(4.0f, 0.2f, 0.0f)).Length() > 0.9f) 
            {
                shared_ptr<Material> SphereMaterial;

                if (ChooseMat < 0.8f) 
                {
                    // diffuse
                    auto Albedo = Color::Random() * Color::Random();
                    SphereMaterial = make_shared<Lambertian>(Albedo);
                    World.Add(make_shared<Sphere>(Center, 0.2f, SphereMaterial));
                } 
                else if (ChooseMat < 0.95f) 
                {
                    // metal
                    auto Albedo = Color::Random(0.5f, 1.0f);
                    auto Fuzz = RandomFloat(0.0f, 0.5f);
                    SphereMaterial = make_shared<Metal>(Albedo, Fuzz);
                    World.Add(make_shared<Sphere>(Center, 0.2f, SphereMaterial));
                } 
                else 
                {
                    // glass
                    SphereMaterial = make_shared<Dielectric>(1.5f);
                    World.Add(make_shared<Sphere>(Center, 0.2f, SphereMaterial));
                }
            }
        }
    }

    auto Material1 = make_shared<Dielectric>(1.5f);
    World.Add(make_shared<Sphere>(Point3(0.0f, 1.0f, 0.0f), 1.0f, Material1));

    auto material2 = make_shared<Lambertian>(Color(0.4f, 0.2f, 0.1f));
    World.Add(make_shared<Sphere>(Point3(-4.0f, 1.0f, 0.0f), 1.0f, material2));

    auto material3 = make_shared<Metal>(Color(0.7f, 0.6f, 0.5f), 0.0f);
    World.Add(make_shared<Sphere>(Point3(4.0f, 1.0f, 0.0f), 1.0f, material3));

    return World;
}

int main()
{
    // Image

    const auto AspectRatio = 3.0f / 2.0f;
    const int ImageWidth = 1200;
    const int ImageHeight = static_cast<int>(ImageWidth / AspectRatio);
    const int SamplesPerPixel = 500;
    const int MaxDepth = 50;

    // World
    auto World = RandomScene();

    // Camera

    Point3 LookFrom(13.0f, 2.0f, 3.0f);
    Point3 LookAt(0.0f, 0.0f, 0.0f);
    Vector3 Up(0.0f, 1.0f, 0.0f);
    auto DistToFocus = 10.0f;
    auto Aperture = 0.1f;
    Camera Cam(LookFrom, LookAt, Up, 20.0f, AspectRatio, Aperture, DistToFocus);

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