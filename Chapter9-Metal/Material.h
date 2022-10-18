#pragma once

#include "RTWeekend.h"
#include "Hittable.h"

class Material 
{
    public:
        virtual bool Scatter(
            const Ray& InRay, const HitRecord& Record, Color& Attenuation, Ray& Scattered
        ) const = 0;
};

class Lambertian : public Material 
{
    public:
        Lambertian(const Color& InAlbedo) : Albedo(InAlbedo) {}

        virtual bool Scatter(
            const Ray& InRay, const HitRecord& Record, Color& Attenuation, Ray& Scattered
        ) const override 
        {
            auto ScatterDirection = Record.Normal + RandomUnitVector();

            // Catch degenerate scatter direction
            if (ScatterDirection.NearZero())
            {
                ScatterDirection = Record.Normal;
            }
                
            Scattered = Ray(Record.p, ScatterDirection);
            Attenuation = Albedo;
            return true;
        }

    public:
        Color Albedo;
};

class Metal : public Material {
    public:
        Metal(const Color& InAlbedo, float InFuzz) 
        : Albedo(InAlbedo),
          Fuzz(Clamp(InFuzz, 0.0f, 1.0f))
        {}

         virtual bool Scatter(
            const Ray& InRay, const HitRecord& Record, Color& Attenuation, Ray& Scattered
        ) const override 
        {
            Vector3 Reflected = Reflect(UnitVector(InRay.GetDirection()), Record.Normal);
            Scattered = Ray(Record.p, Reflected + Fuzz * RandomInUnitSphere());
            Attenuation = Albedo;
            return (Dot(Scattered.GetDirection(), Record.Normal) > 0);
        }

    public:
        Color Albedo;
        float Fuzz;
};