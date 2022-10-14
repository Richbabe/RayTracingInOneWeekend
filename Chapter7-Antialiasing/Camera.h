#pragma once

#include "RTWeekend.h"

class Camera 
{
    public:
        Camera() 
        {
            const auto AspectRatio = 16.0f / 9.0f;
            const auto ViewportHegiht = 2.0f;
            auto ViewportWidth = AspectRatio * ViewportHegiht;
            auto FocalLength = 1.0f;

            Origin = Point3(0.0f, 0.0f, 0.0f);
            Horizontal = Vector3(ViewportWidth, 0.0f, 0.0f);
            Vertical = Vector3(0.0f, ViewportHegiht, 0.0f);
            LowerLeftCorner = Origin - Horizontal / 2.0f - Vertical / 2.0f - Vector3(0.0f, 0.0f, FocalLength);
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