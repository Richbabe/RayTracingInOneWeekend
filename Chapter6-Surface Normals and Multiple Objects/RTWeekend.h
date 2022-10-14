#pragma once

#include <cmath>
#include <limits>
#include <memory>
#include <iostream>
#include <vector>


// Usings

using std::shared_ptr;
using std::make_shared;
using std::sqrt;

// Constants

const float Infinity = std::numeric_limits<float>::infinity();
const float PI = 3.1415926535897932385;

// Utility Functions

inline float DegreesToRadians(float Degrees) 
{
    return Degrees * PI / 180.0f;
}

// Common Headers

#include "Ray.h"
#include "Vector3.h"