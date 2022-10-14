#pragma once

#include "Ray.h"

struct HitRecord 
{
    Point3 p;
    Vector3 Normal;
    float t;
    bool bFrontFace;

    inline void SetFaceNormal(const Ray& Ray, const Vector3& OutwardNormal)
    {
        bFrontFace =  Dot(Ray.GetDirection(), OutwardNormal) < 0.0f;
        Normal = bFrontFace ? OutwardNormal : -OutwardNormal;
    }
};

class Hittable 
{
    public:
        virtual bool Hit(const Ray& InRay, float tMin, float tMax, HitRecord& Record) const = 0;
};