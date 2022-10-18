#pragma once

#include "RTWeekend.h"

class Camera 
{
    public:
        Camera(
            Point3 LookFrom,
            Point3 LookAt,
            Vector3 Up,
            float FOV,
            float AspectRatio
        ) 
        {
            auto Theta = DegreesToRadians(FOV);
            auto H = tan(Theta / 2.0f);
            auto ViewportHeight = 2.0f * H;
            auto ViewportWidth = AspectRatio * ViewportHeight;

            auto W = UnitVector(LookFrom - LookAt);
            auto U = UnitVector(Cross(Up, W));
            auto v = Cross(W, U);

            Origin = LookFrom;
            Horizontal = ViewportWidth * U;
            Vertical = ViewportHeight * v;
            LowerLeftCorner = Origin - Horizontal / 2.0f - Vertical / 2.0f - W;
        }

        Ray GetRay(float u, float v) const 
        {
            return Ray(Origin, LowerLeftCorner + u * Horizontal + v * Vertical - Origin);
        }

    private:
        Point3 Origin;
        Point3 LowerLeftCorner;
        Vector3 Horizontal;
        Vector3 Vertical;
};