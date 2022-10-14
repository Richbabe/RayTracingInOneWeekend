#pragma once

#include <cmath>

using std::sqrt;

class Vector3 
{
    public:
        Vector3() : Elements{0,0,0} {}
        Vector3(float e0, float e1, float e2) : Elements{e0, e1, e2} {}

        inline float X() const { return Elements[0]; }
        inline float Y() const { return Elements[1]; }
        inline float Z() const { return Elements[2]; }

        inline Vector3 operator-() const { return Vector3(-Elements[0], -Elements[1], -Elements[2]); }
        inline float operator[](int i) const { return Elements[i]; }
        inline float& operator[](int i) { return Elements[i]; }

        Vector3& operator+=(const Vector3& v) 
        {
            Elements[0] += v.Elements[0];
            Elements[1] += v.Elements[1];
            Elements[2] += v.Elements[2];
            return *this;
        }

        Vector3& operator*=(float t) 
        {
            Elements[0] *= t;
            Elements[1] *= t;
            Elements[2] *= t;
            return *this;
        }

        Vector3& operator/=(float t) 
        {
            return *this *= 1/t;
        }

        float Length() const 
        {
            return sqrt(LengthSquared());
        }

        float LengthSquared() const 
        {
            return Elements[0]*Elements[0] + Elements[1]*Elements[1] + Elements[2]*Elements[2];
        }

    public:
        float Elements[3];
};

// Type aliases for vec3

using Point3 = Vector3;   // 3D point
using Color = Vector3;    // RGB color

// vec3 Utility Functions

inline std::ostream& operator<<(std::ostream& out, const Vector3& v) 
{
    return out << v.Elements[0] << ' ' << v.Elements[1] << ' ' << v.Elements[2];
}

inline Vector3 operator+(const Vector3& u, const Vector3& v) 
{
    return Vector3(u.Elements[0] + v.Elements[0], u.Elements[1] + v.Elements[1], u.Elements[2] + v.Elements[2]);
}

inline Vector3 operator-(const Vector3& u, const Vector3& v) 
{
    return Vector3(u.Elements[0] - v.Elements[0], u.Elements[1] - v.Elements[1], u.Elements[2] - v.Elements[2]);
}

inline Vector3 operator*(const Vector3& u, const Vector3& v) 
{
    return Vector3(u.Elements[0] * v.Elements[0], u.Elements[1] * v.Elements[1], u.Elements[2] * v.Elements[2]);
}

inline Vector3 operator*(float t, const Vector3& v) 
{
    return Vector3(t * v.Elements[0], t * v.Elements[1], t * v.Elements[2]);
}

inline Vector3 operator*(const Vector3& v, float t) 
{
    return t * v;
}

inline Vector3 operator/(const Vector3& v, float t) 
{
    return (1/t) * v;
}

inline float Dot(const Vector3& u, const Vector3& v) 
{
    return u.Elements[0] * v.Elements[0]
         + u.Elements[1] * v.Elements[1]
         + u.Elements[2] * v.Elements[2];
}

inline Vector3 Cross(const Vector3& u, const Vector3& v) 
{
    return Vector3(u.Elements[1] * v.Elements[2] - u.Elements[2] * v.Elements[1],
                u.Elements[2] * v.Elements[0] - u.Elements[0] * v.Elements[2],
                u.Elements[0] * v.Elements[1] - u.Elements[1] * v.Elements[0]);
}

inline Vector3 UnitVector(const Vector3& v) 
{
    return v / v.Length();
}
