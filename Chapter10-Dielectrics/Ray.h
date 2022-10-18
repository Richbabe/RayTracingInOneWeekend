#pragma once

#include "Vector3.h"

class Ray 
{
    public:
        Ray() {}
        Ray(const Point3& InOrigin, const Vector3& InDir)
            : Origin(InOrigin), Dir(InDir)
        {}

        inline Point3 GetOrigin() const  { return Origin; }
        inline Vector3 GetDirection() const { return Dir; }

        Point3 At(float t) const 
        {
            return Origin + t * Dir;
        }

    public:
        Point3 Origin;
        Vector3 Dir;
};
